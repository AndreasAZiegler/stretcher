
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
     * @param stressOrForce Indicates if the experiment is stress or force based.
     * @param stageframe Pointer to the stage frame object.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param graph Pointer to the graph object.
     * @param diameter The diameter of the sample.
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

    //ExperimentValues(const ExperimentValues &experimentvalues);

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
     */
    void startMeasurement(std::shared_ptr<std::vector<double>> forcestressgraph,
                          std::shared_ptr<std::vector<double>> distancegraph,
                          std::shared_ptr<std::vector<double>> displacement,
                          std::shared_ptr<std::vector<double>> maxforcelimitvaluesgraph,
                          std::shared_ptr<std::vector<double>> minforcelimitvaluesgraph,
                          std::shared_ptr<std::vector<double>> maxdistancelimitvaluesgraph,
                          std::shared_ptr<std::vector<double>> mindistancelimitvaluesgraph,
                          std::shared_ptr<std::vector<double>> forcelimittimepointsgraph,
                          std::shared_ptr<std::vector<double>> distancelimittimepointsgraph);

    /**
     * @brief Unregister the update method.
     */
    void stopMeasurement(void);

    /**
     * @brief Reset recorded values, executed from the protocol.
     */
    void resetProtocol(void);

    /**
     * @brief Removes the current graph.
     */
    void removeGraph(void);

    /**
     * @brief Defines if experiment is force or stress based.
     * @param forceOrStress
     */
    void setDistanceOrStressOrForce(DistanceOrStressOrForce distanceOrStressOrForce){
      m_DistanceOrStressOrForce = distanceOrStressOrForce;
    }

    /**
     * @brief Add new stress of force value to the vector.
     * @param force Stress or force value.
     */
    /*
    void newStressForceValue(double force){
      m_StressForceValues.push_back(force);
    }
    */

    /**
     * @brief Add new distance value to the vector.
     * @param distance Distance value.
     */
    /*
    void newDistanceValue(double distance){
      m_DistanceValues.push_back(distance);
    }
    */

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
    std::string getDistanceOrStressForce(void);

  protected:

    /**
     * @brief Normalizes the value according to the experiment.
     * @param value
     * @return
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

    double m_Area;																													/**< Area size of the sample. */
    DistanceOrStressOrForce m_DistanceOrStressOrForce;										  /**< Defines if the experiment is distance of stress/force based. */

	private:

    ExperimentType m_ExperimentType;																				/**< Type of the experiment */
    std::shared_ptr<StageFrame> m_StageFrame;																/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;	/**< Pointer to the message handler object */
    mpFXYVector *m_ForceStressDistanceGraph;																/**< Pointer to the force/stress - distance graph */
    mpFXYVector *m_ForceStressDisplacementGraph;														/**< Pointer to the force/stress - displacement graph. */
    std::mutex *m_VectorLayerMutex;																					/**< Pointer to the mutex to protect m_VectorLayer */
    mpFXYVector *m_MaxForceLimitVectorLayer;
    mpFXYVector *m_MinForceLimitVectorLayer;
    mpFXYVector *m_MaxDistanceLimitVectorLayer;
    mpFXYVector *m_MinDistanceLimitVectorLayer;
    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    std::vector<std::vector<ExperimentValues::MeasurementValue>> m_StressForceValues;		/**< Vector containing structs with stress/force values and their time stamps */
    std::vector<std::vector<ExperimentValues::MeasurementValue>> m_DistanceValues;				/**< Vector containing structs with distance values and their time stamps */
    std::shared_ptr<std::vector<double>> m_ForceStressGraphValues;					/**< Vector containing only the stress/force - distance values */
    std::shared_ptr<std::vector<double>> m_DistanceGraphValues;							/**< Vector containing only the distance values */
    std::shared_ptr<std::vector<double>> m_DisplacementGraphValues;					/**< Vector containing only the displacement values */
    std::shared_ptr<std::vector<double>> m_GraphMaxForceLimitValues;				/**< Pointer to the vector containing the max force limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphMinForceLimitValues;				/**< Pointer to the vector containing the min force limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphForceLimitXAxisPoints;			/**< Pointer to the vector containing the values for the x-axis for the force limits. */
    std::shared_ptr<std::vector<double>> m_GraphMaxDistanceLimitValues;			/**< Pointer to the vector containing the max distance limit graph values. */
    std::shared_ptr<std::vector<double>> m_GraphMinDistanceLimitValues;			/**< Pointer to the vector containing the min distance limit graph vlaues. */
    std::shared_ptr<std::vector<double>> m_GraphDistanceLimitYAxisPoints;		/**< Pointer to the vector containing the values for the x-axis for the force limits. */
    std::mutex m_AccessValuesMutex;																					/**< Mutex to protect the values vectors. */
    int m_CurrentProtocolCycle;																							/**< The current cycle numer. */
    bool m_ResetProtocolFlag;																								/**< Indicate it the protocol stops and the recorded values should be deleted. */
    int m_DisplayGraphDelay;																								/**< Variable used that the graph is not updated with every value update */
    double m_GageLength;																										/**< The gage length. */

};

#endif // EXPERIMENTVALUES_H
