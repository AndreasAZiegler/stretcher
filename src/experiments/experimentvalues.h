
#ifndef EXPERIMENTVALUES_H
#define EXPERIMENTVALUES_H
#include <vector>
#include <mathplot.h>
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
     * @brief Force or stress
     */
    enum class StressOrForce{
      Stress = 0,
      Force = 1
    };

    /**
     * @brief Initializes all the needed variables.
     * @param stressOrForce Indicates if the experiment is stress or force based.
     * @param stageframe Pointer to the stage frame object.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param graph Pointer to the graph object.
     * @param diameter The diameter of the sample.
     */
    ExperimentValues(StressOrForce stressOrForce,
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
    void newStressForceValue(double force){
      m_StressForceValues.push_back(force);
    }

    /**
     * @brief Add new distance value to the vector.
     * @param distance Distance value.
     */
    void newDistanceValue(double distance){
      m_DistanceValues.push_back(distance);
    }

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
    virtual void updateValues(long value, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Updates the graph in the GUI.
     */
    void updateGraph(void);

	private:

    StressOrForce m_StressOrForce;
    StageFrame *m_StageFrame;																								/**< Pointer to the stage frame object */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler;									/**< Pointer to the message handler object */
    mpFXYVector *m_VectorLayer;																							/**< Pointer to the vector for the graph */
    std::mutex *m_VectorLayerMutex;																					/**< Pointer to the mutex to protect m_VectorLayer */
    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    std::vector<double> m_StressForceValues;
    std::vector<double> m_DistanceValues;
    std::mutex m_AccessValuesMutex;
		double m_Diameter;
    int m_DisplayGraphDelay;																								/**< Variable used that the graph is not updated with every value update */

};

#endif // EXPERIMENTVALUES_H
