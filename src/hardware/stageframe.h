
#ifndef STAGEFRAME_H
#define STAGEFRAME_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include "../updatedvaluesreceiver.h"

// Class forward declaration
class LinearStage;
class LinearStageMessageHandler;

// Method pointer typedef
typedef void (UpdatedValuesReceiver::*updateValue)(long, UpdatedValuesReceiver::ValueType);
typedef std::function<void(long, UpdatedValuesReceiver::ValueType)> mp;

/**
 * @brief Represents the two linear stages as a stage frame.
 */
class StageFrame : virtual public UpdatedValuesReceiver
{
	public:
    StageFrame();

    /**
     * @brief Registers the linear stages and get pointer for the message handlers of the linear stages and register update method at the message handlers.
     * @param linearstages Pointer to the vector containing the pointer to the linear stages objects.
     */
    void registerLinearStages(std::vector<LinearStage*> *linearstages);

    /**
     * @brief Registers the wait condition variable for the wait for stop.
     * @param waitcond Pointer to the wait condition variable.
     * @param mutex Mutex for the wait stop.
     */
    void registerStagesStopped(bool *stagesstopped, std::mutex *mutex){
      m_StagesStoppedFlag = stagesstopped;
      m_StagesStoppedMutex = mutex;
    }

     /**
     * @brief Registers the update methods, which will be called, when the value changes.
     * @param updateMethod Method which updates a value.
     * @param updateClass Pointer to the object, to which the method belongs.
     * @return Id of the callback method.
     */
    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass);

    /**
     * @brief Unregisters the update methods, which will be called, when the value changes.
     * @param id Id of the callback method
     */
    void unregisterUpdateMethod(std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator id);
    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    void updateValues(long value, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Sets the speed of the linear stage.
     * @param speedinmm Speed in mm/s
     */
    void setSpeed(double speedinmm);

    /**
     * @brief Moves the stage forward frame at constant speed
     */
    void moveForward(double speed = 0);

    /**
     * @brief Moves the stage backward frame at constant speed
     */
    void moveBackward(double speed = 0);

    /**
     * @brief Moves the stage the amount of millimeters.
     * @param milimeters Amount of milimeters
     */
    void moveMM(double millimeters);

    /**
     * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
     *        and start the motors.
     * @param distance Desired clamping distance in mm from the GUI
     */
    void gotoMMDistance(int mmDistance);

    /**
     * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
     *        and start the motors.
     * @param distance Desired clamping distance in micro steps from the GUI
     */
    void gotoStepsDistance(long stepsDistance);

    /**
     * @brief Move the stage to the absolute position
     * @param position Absolute stage position.
     */
    void gotoToAbsolute(long position);

    /**
     * @brief Stops the stage frame.
     */
    void stop();

    /**
     * @brief Is executed by a linear stage messsage handler to indicate, that one motor stopped.
     */
    void stopped();

  private:

    /**
     * @todo Implementation
     * @brief Get the current distance.
     */
    long getCurrentDistance(void);

    std::vector<LinearStage*> *m_LinearStages;
    std::vector<LinearStageMessageHandler*> m_LinearStagesMessageHandlers;

    const double MM_PER_MS;               						/**< milimeter per microstep */

    double m_Stepsize;			     				    					/**< Stepsize of the stepper motor in millimeters */
    long m_ZeroDistance;	 	 		   			           	  /**< Distance when the motors are on max position (resulting in smallest distance) */

    bool m_Pos1ChangedFlag;														/**< Indicates an updated position 1 if true */
    bool m_Pos2ChangedFlag;														/**< Indicates an updated position 2 if true */
    std::mutex m_PosChangedMutex;											/**< Mutex to protect m_PosXChangedFlag */

    std::vector<long> m_CurrentPositions;							/**< Vector containing the current positions of the linear stages */
    long m_CurrentDistance;														/**< Current distance */

    bool *m_StagesStoppedFlag;														/**< Flag to indicate if the stages stopped or not. */
    std::mutex *m_StagesStoppedMutex;									/**< Mutex for m_StagesStopped */

    int m_AmStopped;																	/**< Number of stopped motors */
    std::mutex m_AmStoppedMutex;											/**< Mutex to protect m_AmStopped */
};

#endif // STAGEFRAME_H