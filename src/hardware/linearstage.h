
#ifndef _LINEARMOTOR_H__
#define _LINEARMOTOR_H__

// included dependencies
#include <mutex>
#include "serialinterface.h"
#include "linearstagemessagehandler.h"

/**
 * @brief Class that provides access to the hardware module with the zaber linear stepper motor.
 */
class LinearStage : public SerialInterface
{

  public:
    /**
     * @brief Forwards the com port and the baud rate to SerialPort
     * @param comPort com port
     * @param baudrate baudrate
     */
    LinearStage(UpdateValues::ValueType type, unsigned int baudrate = 115200);

    ~LinearStage();

    /**
      * @brief Returns the pointer to the message handler.
      * @return Pointer to the message handler.
      */
    LinearStageMessageHandler* getMessageHandler(void){
      return(&m_MessageHandler);
    }

    /**
     * @brief Sends linear stage to the home positin.
     */
    void home();

    void setSpeed(double speed);

    /**
     * @brief Stops the linear stage.
     */
    void stop();

    /**
     * @brief Moves the stage at constant speed
     */
    void move();

    /**
     * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
     *        and start the motors.
     * @param distance Desired clamping distance in micro steps from the GUI
     */
    void gotoMMDistance(int mmDistance);

    /**
     * @brief Moves the stage the amount of steps.
     * @param steps Amount of steps
     */
    void moveSteps(int steps);

    /**
     * @brief Moves the stage the amount of millimeters.
     * @param milimeters Amount of milimeters
     */
    void moveMillimeters(double millimeters);

   private:

    /**
     * @brief Disable the knob and enables move tracking.
     */
    void setDeviceMode(void);

    /**
     * @brief Configure the interval between each Move Tracking of Manual Move Tracking responses.
     */
    void setMoveTrackingPeriod(void);

    /**
     * @todo Implementation
     * @brief getCurrentDistance
     */
    double getPosition(void);

    /**
     * @brief Transforms a decimal number to a char* variable.
     * @param dec Number in the decimal number system.
     * @return The number as a char* variable.
     */
    char* transformDecToText(int dec);

    // Defined commands for the Zaber linear motors.
    const char *STAGE_DEVICE_MODE;								/**< Command to get the device mode */
    const char *STAGE_SET_MOVE_TRACKING_PERIOD;				/**< Command to set the move tracking period */
    const char *STAGE_RESET;											/**< Command to reset the motor */
    const char *STAGE_RETURN_CURRENT_POSITION;		/**< Command to get the current position */
    const char *STAGE_GO_HOME;										/**< Command to move the motor to home distance (0) */
    const char *STAGE_MOVE_ABSOLUTE;							/**< Cammand to move the motor absolut */
    const char *STAGE_MOVE_RELATIVE;							/**< Cammand to move the motor relative */
    const char *STAGE_SET_SPEED;									/**< Command to set the motor speed */
    const char *STAGE_MOVE_AT_CONSTANT_SPEED;			/**< Command to let the motor move at constant speed */
    const char *STAGE_STOP;												/**< Command to stop the motor */

    const double MM_PER_MS;               				/**< milimeter per microstep */

    //const float DecIncSpeed;              				/**< Amount of speed to increase/decrease */

    double m_Stepsize;			     				    			/**< Stepsize of the stepper motor in millimeters */
    char mbytedata[4];
    //int mOscstate;							          //!< Current state of the oscillation
    //double mAmplitude;						        //!< Amplitude of the oscillation
    //bool mStoprequest;						        //!< stopflag for the oscillation
    //int mLMtimerID;							          //!< Timer ID of the oscillation timer
    //int m_CurrentSpeed;         			           /**< The current speed */
    //int m_CurrentPosMotor[2];   			           /**< Position of the two motors */
    //double m_ZeroDistance;    			             /**< Distance when the motors are on max position (resulting in smallest distance) */
    //bool mOldPositionFlag;                /**< Flag to indicate that the current "old" position is still up to date */
    //bool mMoveFinishedFlag;               /**< Flag to indicate, that the motors finished moving */
    //bool mPositionPendingFlag;            /**< Indicate that a RETURN_CURRENT_POSITION command is executed but the answer not yet arrived */
    //bool mStartUpFlag;                    /**< Indicates that software is starting up or started up */

    //bool mExpectedMessagesFlag;           /**< Indicates that messages are expected */

    LinearStageMessageHandler m_MessageHandler;    /**< Pointer to the MessageHandler object */
};

#endif /* _LINEARMOTOR_H__ */
