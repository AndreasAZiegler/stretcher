
// Includes
#include "linearmotor.h"
#include "../../include/ctb-0.13/serport.h"

/**
 * @brief Forwards the com port and the baud rate to SerialPort
 * @param comPort com port
 * @param baudrate baudrate
 */
LinearMotor::LinearMotor(std::string comPort, unsigned int baudrate)
    : SerialInterface(comPort, baudrate),
    mExpectedMessagesFlag(0),
    mOldPositionFlag(true),
    mMoveFinishedFlag(true),
    m_Stepsize(0.000047625),                    //Stepsize of Zaber T-LSM025A motor in millimeters
    mOscstate(0),
    mStoprequest(false),
    mLMtimerID(0),
    mAmplitude(0),
    MOTORS_DEVICE_MODE("\x00\x028"),
    MOTORS_RESET("\x00\x00"),
    MOTORS_RETURN_CURRENT_POSITION("\x00\x03c"),
    MOTORS_GO_HOME("\x00\x01"),
    MOTORS_MOVE_ABSOLUTE("\x00\x014"),
    MOTORS_MOVE_RELATIVE("\x00\x015"),
    MOTORS_SET_SPEED("\x00\x02a"),
    MOTORS_MOVE_AT_CONSTANT_SPEED("\x00\x016"), //: QObject(parent)
    MOTORS_STOP("\x00\x017"),
    MM_PER_MS(0.000047625),
    DecIncSpeed(0.3),
    m_ZeroDistance(45144/*microsteps=6.39mm offset */),
    mPositionPendingFlag(false),
    mStartUpFlag(true)
{
}

LinearMotor::~LinearMotor()
{
}
