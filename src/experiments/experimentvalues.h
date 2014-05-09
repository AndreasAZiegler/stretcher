
#ifndef EXPERIMENTVALUES_H
#define EXPERIMENTVALUES_H
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
    ExperimentValues(ExperimentType experimenttype, StressOrForce stressOrForce,
                     StageFrame *stageframe,
                     ForceSensorMessageHandler *forcesensormessagehandler,
                     mpFXYVector *vector,
                     std::mutex *vectoraccessmutex,
                     MyFrame *myframe,
                     double diameter);

    /**
     * @brief Destructor
     */
    ~ExperimentValues();

    /**
     * @brief Registers the update methods to receive the measurement values.
     */
    void startMeasurement(void);

    /**
     * @brief Unregister the update method.
     */
    void stopMeasurement(void);

    /**
     * @brief Removes the current graph.
     */
    void removeGraph(void);

    /**
     * @brief Defines if experiment is force or stress based.
     * @param forceOrStress
     */
    void setStressOrForce(StressOrForce stressOrForce){
      m_StressOrForce = stressOrForce;
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
      m_Diameter = diameter;
    }

    /**
     * @brief Returns the diameter of the sample.
     * @return The diameter of the sample.
     */
    double getDiameter(void){
      return(m_Diameter);
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
    std::vector<ExperimentValues::MeasurementValue>* getStressForceValues(void);

    /**
     * @brief Returns a pointer to the vector containing the distance values.
     * @return Pointer to the vector.
     */
    std::vector<ExperimentValues::MeasurementValue>* getDistanceValues(void);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    //std::string getExperimentSettings(void) = 0;

    /**
     * @brief Returns the current experiment type as a string.
     * @return The current experiment type as a string.
     */
    std::string experimentTypeToString();

    /**
     * @brief Returns true if the export should be disabled, false otherwise.
     * @return Boolean value.
     */
    bool isExportDisabled(void){
      return(m_DisableExportFlag);
    }

    /**
     * @brief Export the measurement unit (stress/force)
     * @return The unit as std::string.
     */
    std::string getStressOrForce(void);

	private:

    ExperimentType m_ExperimentType;																				/**< Type of the experiment */
    StressOrForce m_StressOrForce;																					/**< Indicates if the experiment is stress or force based */
    StageFrame *m_StageFrame;																								/**< Pointer to the stage frame object */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler;									/**< Pointer to the message handler object */
    mpFXYVector *m_VectorLayer;																							/**< Pointer to the vector for the graph */
    std::mutex *m_VectorLayerMutex;																					/**< Pointer to the mutex to protect m_VectorLayer */
    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    std::vector<ExperimentValues::MeasurementValue> m_StressForceValues;		/**< Vector containing structs with stress/force values and their time stamps */
    std::vector<ExperimentValues::MeasurementValue> m_DistanceValues;				/**< Vector containing structs with distance values and their time stamps */
    std::vector<double> m_GraphStressForceValues;														/**< Vector containing only the stress/force values */
    std::vector<double> m_GraphDistanceValues;															/**< Vector containing only the distance values */
    std::mutex m_AccessValuesMutex;																					/**< Mutex to protect the values vectors. */
    double m_Diameter;																											/**< Diameter size of the sample. */
    int m_DisplayGraphDelay;																								/**< Variable used that the graph is not updated with every value update */
    bool m_DisableExportFlag;																								/**< Flag that indicates if the export should be disabled. */

};

#endif // EXPERIMENTVALUES_H
