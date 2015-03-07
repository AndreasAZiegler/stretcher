/**
 * @file linearstage.h
 * @brief The linear stage.
 * @author Andreas Ziegler
 */

#ifndef _LINEARMOTOR_H__
#define _LINEARMOTOR_H__

// included dependencies
#include <mutex>
#include "serialinterface.h"
#include "linearstagemessagehandler.h"

/**
 * @class LinearStage linearstage.h "hardware/linearstage.h"
 * @brief Class that provides access to the hardware module with the zaber linear stepper motor.
 */
class LinearStage : public SerialInterface
{

  public:
    /**
     * @brief Creates a SerialInterface and initializes all needed variables.
     * @param type Type
     * @param waitmessagehandler Pointer to the condition variable to wait for message handlers.
     * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
     * @param messagehandlerfinishednr Pointer to the amount of finishes message handlers.
     * @param baudrate Baudrate
     */
    LinearStage(UpdatedValuesReceiver::ValueType type,
                std::shared_ptr<std::condition_variable> waitmessagehandler,
                std::shared_ptr<std::mutex> waitmessagehandlermutex,
                std::shared_ptr<int> messagehandlerfinishednr,
                unsigned int baudrate = 115200);

    /**
     * @brief Sets the device mode, the move tracking period, the speed and the acceleration of the linear stage.
     */
    void configure();

    /**
     * @brief Sends the command that the stages store their current position.
     */
    ~LinearStage();

    void startGenerateData(void);

    /**
      * @brief Returns the pointer to the message handler.
      * @return Pointer to the message handler.
      */
    std::shared_ptr<LinearStageMessageHandler> getMessageHandler(void);

    /**
     * @brief Sets the maximum position of the stages.
     * @param limit Upper limit.
     */
    void setMaxLimit(long limit);

    /**
     * @brief Sets the minimum position of the stages.
     * @param limit Lower limit.
     */
    void setMinLimit(long limit);

    /**
     * @brief Sends linear stage to the home positin.
     */
    void home(void);

    /**
     * @brief Sets the speed of the linear stage.
     * @param speedinmm Speed in mm/s
     */
    void setSpeed(double speedinmm);

    /**
     * @brief Stops the linear stage.
     */
    void stop(void);

    /**
     * @brief Moves the stage forwards at constant speed
     * @param speedinmm Speed in mm/sec.
     */
    void moveForward(double speedinmm = 0);

    /**
     * @brief Moves the stage backwards at constant speed
     * @param speedinmm Speed in mm/sec.
     */
    void moveBackward(double speedinmm = 0);

    /**
     * @brief Moves the stage the amount of steps.
     * @param steps Amount of steps
     */
    void moveSteps(long steps);

    /**
     * @brief Move the stage to the absolute position
     * @param position Absolute stage position.
     */
    void moveToAbsolute(long position);

    /**
     * @brief Sets the current position.
     * @param position The current position.
     */
    void setCurrentPosition(long position);

    /**
     * @brief Loads the in the stage stored position.
     */
    void loadStoredPosition(void);

   private:

    /**
     * @brief Disable the knob and enables move tracking.
     */
    void setDeviceMode(void);

    /**
     * @brief Sets the home speed of the linear stage.
     * @param speedinmm Speed in mm/s
     */
    void setHomeSpeed(double speedinmm);

    /**
     * @brief Sets the acceleration of the linear stage
     * @param acceleration
     */
    void setAcceleration(double acceleration);

    /**
     * @brief Configure the interval between each Move Tracking of Manual Move Tracking responses.
     */
    void setMoveTrackingPeriod(void);

    /**
     * @brief Stores the current position in the non-volatile memory of the stage.
     */
    void storeCurrentPosition(void);

    /**
     * @brief Transforms a decimal number to a char* variable.
     * @param dec Number in the decimal number system.
     * @return The number as a char* variable.
     */
    char* transformDecToText(int dec);

    // Defined commands for the Zaber linear motors.
    const char *STAGE_DEVICE_MODE;										/**< Command to get the device mode */
    const char *STAGE_SET_MAXIMUM_POSITION;						/**< Command to set the maximum position */
    const char *STAGE_SET_MINIMUM_POSITION;						/**< Command to set the minimum position */
    const char *STAGE_SET_MOVE_TRACKING_PERIOD;				/**< Command to set the move tracking period */
    const char *STAGE_RESET;													/**< Command to reset the motor */
    const char *STAGE_RETURN_CURRENT_POSITION;				/**< Command to get the current position */
    const char *STAGE_GO_HOME;												/**< Command to move the motor to home distance (0) */
    const char *STAGE_MOVE_ABSOLUTE;									/**< Cammand to move the motor absolut */
    const char *STAGE_MOVE_RELATIVE;									/**< Cammand to move the motor relative */
    const char *STAGE_SET_SPEED;											/**< Command to set the motor speed */
    const char *STAGE_SET_HOME_SPEED;									/**< Command to set the motor home speed */
    const char *STAGE_SET_ACCELERATION;								/**< Command to set the motor acceleration */
    const char *STAGE_MOVE_AT_CONSTANT_SPEED;					/**< Command to let the motor move at constant speed */
    const char *STAGE_STOP;														/**< Command to stop the motor */
    const char *STAGE_STORE_CURRENT_POSITION;					/**< Command to store the current position. */
    const char *STAGE_RETURN_STORED_POSITION;					/**< Command to get the stored position. */
    const char *STAGE_SET_CURRENT_POSITION;						/**< Command to set the current position. */

    const double MM_PER_MS;               						/**< milimeter per microstep */


    double m_Stepsize;			     				    					/**< Stepsize of the stepper motor in millimeters */
    char m_ByteData[4];
    int m_CurrentSpeed;         			           			/**< The current speed */

    LinearStageMessageHandler m_MessageHandler;    		/**< The MessageHandler object */
};

#endif /* _LINEARMOTOR_H__ */
