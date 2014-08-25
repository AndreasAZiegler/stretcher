/**
 * @file experimentvalues.h
 * @brief Experiment values base class.
 * @author Andreas Ziegler
 */


#ifndef EXPERIMENTVALUES_H
#define EXPERIMENTVALUES_H

// Includes
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <chrono>
#include <mathplot.h>
#include "experimentdefinitions.h"
#include "../updatedvaluesreceiver.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"

// Forward declaration
class MyFrame;

/**
 * @class ExperimentValues experimentvalues.h "experiments/experimentvalues.h"
 * @brief Class representing the experiment values.
 */
class ExperimentValues : virtual public UpdatedValuesReceiver
{
	public:

    /**
     * @brief Struct for the measurement values containing the values and the time stamp.
     */
    struct MeasurementValue{
      MeasurementValue(){
        value = 0;
      }

      MeasurementValue(double v, std::chrono::high_resolution_clock::time_point ts){
        value = v;
        timestamp = ts;
      }

      MeasurementValue(const MeasurementValue& mv){
        value = mv.value;
        timestamp = mv.timestamp;
      }

      MeasurementValue(MeasurementValue& mv){
        value = mv.value;
        timestamp = mv.timestamp;
      }

      double value;
      std::chrono::high_resolution_clock::time_point timestamp;
    };

    /**
     * @brief Initializes all the needed variables.
     * @param stageframe Shared pointer to the stage frame object.
     * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
     * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
     * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
     * @param *vectoraccessmutex Pointer to the graph access mutex.
     * @param *maxlimitgraph Pointer to the maximum limit graph.
     * @param *minlimitgraph Pointer to the minimum limit graph.
     * @param *myframe Pointer to the main frame object.
     * @param experimenttype Experiment type.
     * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
     * @param area Cross section area.
     * @param gagelength The gage length.
     */
    ExperimentValues(std::shared_ptr<StageFrame> stageframe,
                     std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                     mpFXYVector *forceStressDistanceGraph,
                     mpFXYVector *forceStressDisplacementGraph,
                     std::mutex *vectoraccessmutex,
                     mpFXYVector *maxforcelimitvector,
                     mpFXYVector *minforcelimitvector,
                     mpFXYVector *maxdistancelimitvector,
                     mpFXYVector *mindistancelimitvector,
                     MyFrame *myframe,

                     ExperimentType experimenttype,
                     DistanceOrStressOrForce distanceOrStressOrForce,
                     double area,
                     long gagelength);

    /**
     * @brief Destructor
     */
    ~ExperimentValues();

    /**
     * @brief Sets the gage lengt.
     * @param gagelength The gage length.
     */
    void setGageLength(long gagelength){
      m_GageLength = gagelength;
    }

    /**
     * @brief Sets the cross section area.
     * @param crosssectionarea The cross section area.
     */
    void setCrossSectionArea(double crosssectionarea){
      m_Area = crosssectionarea;
    }


    /**
     * @brief Registers the update methods to receive the measurement values.
     * @param forcestressvector Vector for the force values.
     * @param distancevector Vector for the distance alues.
     * @param displacementvector Vector for the displacment values
     * @param maxforcelimitvector Vector for the maximum force limit values.
     * @param minforcelimitvector Vector for the minimum force limit values.
     * @param maxdistancelimitvector Vector for the maximum distance limit values.
     * @param mindistancelimitvector Vector for the minimum distance limit values.
     * @param limittimepointvector Vector for the limit timepoints.
     */
    void startMeasurement(std::shared_ptr<std::vector<double>> forcestressvector,
                          std::shared_ptr<std::vector<double>> distancevector,
                          std::shared_ptr<std::vector<double>> displacementvector,
                          std::shared_ptr<std::vector<double>> maxforcelimitvector,
                          std::shared_ptr<std::vector<double>> minforcelimitvector,
                          std::shared_ptr<std::vector<double>> maxdistancelimitvector,
                          std::shared_ptr<std::vector<double>> mindistancelimitvector,
                          std::shared_ptr<std::vector<double>> forcelimittimepointsvector,
                          std::shared_ptr<std::vector<double>> distancelimittimepointsvector);

    /**
     * @brief Stops the measurement. Unregisters the update method and increases the current protocol cycle.
     */
    void stopMeasurement(void);

    /**
     * @brief Reset recorded values, executed from the protocol. Sets the reset protocol flag true.
     */
    void resetProtocol(void);

    /**
     * @brief Defines if experiment is force or stress based.
     * @param forceOrStress
     */
    void setDistanceOrStressOrForce(DistanceOrStressOrForce distanceOrStressOrForce){
      m_DistanceOrStressOrForce = distanceOrStressOrForce;
    }

    /**
     * @brief Sets the diameter of the sample.
     * @param diameter The diameter of the sample.
     */
    void setDiameter(double diameter){
      m_Area = diameter;
    }

    /**
     * @brief Returns the diameter of the sample.
     * @return The diameter of the sample.
     */
    double getDiameter(void){
      return(m_Area);
    }

    /**
     * @brief Method which will be calles by the message handlers to update the values.
     * @param value Position of linear stage 1 or 2 or the force.
     * @param type Type of value.
     */
    virtual void updateValues(UpdatedValues::MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Returns a pointer to the vector containing the stress/force values.
     * @return Pointer to the vector.
     */
    std::vector<std::vector<ExperimentValues::MeasurementValue>>* getStressForceValues(void);

    /**
     * @brief Returns a pointer to the vector containing the distance values.
     * @return Pointer to the vector.
     */
    std::vector<std::vector<MeasurementValue>> *getDistanceValues(void);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void) = 0;

    /**
     * @brief Returns the current experiment type as a string.
     * @return The current experiment type as a string.
     */
    std::string experimentTypeToString(void);

    /**
     * @brief Returns the experiment settings in a short form, usable for the experiment name.
     * @return The experiment settings in a short form.
     */
    virtual std::string experimentSettingsForName(void) = 0;

    /**
     * @brief Returns the measurement unit (stress/force).
     * @return The unit as std::string.
     */
    std::string getStressOrForce(void);

    /**
     * @brief Returns the the measurement type (distance/stressForce).
     * @return The type as std::string.
     */
    std::string getDistanceOrForceOrStress(void);

  protected:

    /**
     * @brief Normalizes the value according to the experiment.
     * @param value The value.
     * @return The normalized value.
     */
    double normalizeValue(double value);

    /**
     * @brief A wrapper of std::to_string to set precission.
     * @param a_value Variable which will be converted.
     * @param n Precision
     * @return Return value.
     */
    template <typename T>
    std::string to_string_wp(const T a_value, const int n = 6){
      std::ostringstream outstr;
      outstr.precision(n);
      outstr << a_value;
      return(outstr.str());
    }

    double m_Area;																																		/**< Area size of the sample. */
    DistanceOrStressOrForce m_DistanceOrStressOrForce;										  					/**< Defines if the experiment is distance of stress/force based. */

	private:

    ExperimentType m_ExperimentType;																									/**< Type of the experiment */
    std::shared_ptr<StageFrame> m_StageFrame;																					/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;						/**< Pointer to the message handler object */
    mpFXYVector *m_ForceStressDistanceGraph;																					/**< Pointer to the force/stress - distance graph */
    mpFXYVector *m_ForceStressDisplacementGraph;																			/**< Pointer to the force/stress - displacement graph. */
    std::mutex *m_VectorLayerMutex;																										/**< Pointer to the mutex to protect m_VectorLayer */
    mpFXYVector *m_MaxForceLimitVectorLayer;																					/**< Pointer to the maximal force limit graph. */
    mpFXYVector *m_MinForceLimitVectorLayer;																					/**< Pointer to the minimal force limit graph. */
    mpFXYVector *m_MaxDistanceLimitVectorLayer;																				/**< Pointer to the maximal distance limit graph. */
    mpFXYVector *m_MinDistanceLimitVectorLayer;																				/**< Pointer to the minimal distance limit graph. */
    MyFrame *m_MyFrame;																																/**< Pointer to the main frame object. */
    std::vector<std::vector<ExperimentValues::MeasurementValue>> m_StressForceValues;	/**< Vector containing structs with stress/force values and their time stamps */
    std::vector<std::vector<ExperimentValues::MeasurementValue>> m_DistanceValues;		/**< Vector containing structs with distance values and their time stamps */
    std::shared_ptr<std::vector<double>> m_ForceStressGraphValues;										/**< Vector containing only the stress/force - distance values */
    std::shared_ptr<std::vector<double>> m_DistanceGraphValues;												/**< Vector containing only the distance values */
    std::shared_ptr<std::vector<double>> m_DisplacementGraphValues;										/**< Vector containing only the displacement values */
    std::shared_ptr<std::vector<double>> m_GraphMaxForceLimitValues;									/**< Pointer to the vector containing the max force limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphMinForceLimitValues;									/**< Pointer to the vector containing the min force limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphForceLimitXAxisPoints;								/**< Pointer to the vector containing the values for the x-axis for the force limits. */
    std::shared_ptr<std::vector<double>> m_GraphMaxDistanceLimitValues;								/**< Pointer to the vector containing the max distance limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphMinDistanceLimitValues;								/**< Pointer to the vector containing the min distance limit graph vlaues. */
    std::shared_ptr<std::vector<double>> m_GraphDistanceLimitYAxisPoints;							/**< Pointer to the vector containing the values for the x-axis for the force limits. */
    std::mutex m_AccessValuesMutex;																										/**< Mutex to protect the values vectors. */
    int m_CurrentProtocolCycle;																												/**< The current cycle numer. */
    bool m_ResetProtocolFlag;																													/**< Indicate it the protocol stops and the recorded values should be deleted. */
    int m_DisplayGraphDelay;																													/**< Variable used that the graph is not updated with every value update */
    double m_GageLength;																															/**< The gage length. */

};

#endif // EXPERIMENTVALUES_H
