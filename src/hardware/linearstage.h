
#ifndef _LINEARMOTOR_H__
#define _LINEARMOTOR_H__

// included dependencies
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
    LinearStage(unsigned int baudrate = 9600);

    ~LinearStage();

    void setSpeed(double speed);

    void stop();

    void move();

    void moveTo(int position);

    void moveSteps(int steps);

    void moveMillimeters(double millimeters);

   private:

    char* transformDecToText(int dec);

    // Defined commands for the Zaber linear motors.
    const char *MOTORS_DEVICE_MODE;								/**< Command to get the device mode */
    const char *MOTORS_RESET;											/**< Command to reset the motor */
    const char *MOTORS_RETURN_CURRENT_POSITION;		/**< Command to get the current position */
    const char *MOTORS_GO_HOME;										/**< Command to move the motor to home distance (0) */
    const char *MOTORS_MOVE_ABSOLUTE;							/**< Cammand to move the motor absolut */
    const char *MOTORS_MOVE_RELATIVE;							/**< Cammand to move the motor relative */
    const char *MOTORS_SET_SPEED;									/**< Command to set the motor speed */
    const char *MOTORS_MOVE_AT_CONSTANT_SPEED;		/**< Command to let the motor move at constant speed */
    const char *MOTORS_STOP;											/**< Command to stop the motor */

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
