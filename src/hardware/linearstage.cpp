
// Includes
#include "linearstage.h"
#include "../../include/ctb-0.13/serport.h"

using namespace std;

/**
 * @brief Forwards the com port and the baud rate to SerialPort
 * @param comPort com port
 * @param baudrate baudrate
 */
LinearStage::LinearStage(UpdatedValuesReceiver::ValueType type, unsigned int baudrate)
    : SerialInterface(type, baudrate),
      m_MessageHandler(&m_SerialPort, type, &m_ReadingSerialInterfaceMutex),
      m_Stepsize(0.00009921875),                    //Stepsize of Zaber T-LSM025A motor in millimeters
      m_CurrentSpeed(0),
      m_ZeroDistance(0),
    /*
    mExpectedMessagesFlag(0),
    mOldPositionFlag(true),
    mMoveFinishedFlag(true),
    mOscstate(0),
    mStoprequest(false),
    mLMtimerID(0),
    mAmplitude(0),
    DecIncSpeed(0.3),
    m_ZeroDistance(45144/*microsteps=6.39mm offset *///),
    /*
    mPositionPendingFlag(false),
    mStartUpFlag(true)
    */
      STAGE_DEVICE_MODE("\x00\x028"),
      STAGE_SET_MOVE_TRACKING_PERIOD("\x00\x075"),
      STAGE_RESET("\x00\x00"),
      STAGE_RETURN_CURRENT_POSITION("\x00\x03c"),
      STAGE_GO_HOME("\x00\x01"),
      STAGE_MOVE_ABSOLUTE("\x00\x014"),
      STAGE_MOVE_RELATIVE("\x00\x015"),
      STAGE_SET_SPEED("\x00\x02a"),
      STAGE_SET_HOME_SPEED("\x00\x029"),
      STAGE_SET_ACCELERATION("\x00\x02b"),
      STAGE_MOVE_AT_CONSTANT_SPEED("\x00\x016"), //: QObject(parent)
      STAGE_STOP("\x00\x017"),
      MM_PER_MS(0.00009921875)
{
}

/**
 * @brief Sets the device mode, the move tracking period and the speed of the linear stage.
 */
void LinearStage::configure(){
  setDeviceMode();
  setMoveTrackingPeriod();
  setHomeSpeed(10/*mm/s*/);
  setSpeed(10/*mm/s*/);
  setAcceleration(500);
}

LinearStage::~LinearStage()
{
}

/**
 * @brief Disable the knob and enables move tracking.
 */
void LinearStage::setDeviceMode(void){
  char buffer[6];
  char command[6] = "";

  char *settings = transformDecToText(1*8/*disable knob*/ +
                                      1*16/*enable move tracking*/);
  memcpy(command, STAGE_DEVICE_MODE, 2);
  memcpy(command+2, settings, sizeof(settings));

  memcpy(buffer, command, 6);

  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 10/*ms*/);
  }
}

/**
 * @brief Sets the home speed of the linear stage.
 * @param speedinmm Speed in mm/s
 */
void LinearStage::setHomeSpeed(double speedinmm){
  int speed = 0;
  char buffer[6];
  char command[6] = "";
  char* number;

  if(speedinmm != 0) {
    speed = speedinmm * 1.6384 / m_Stepsize; //transformation from mm/s to datavalue
  } else {
    speed = m_CurrentSpeed;
  }

  memcpy(command,STAGE_SET_HOME_SPEED,2);
  number = transformDecToText(speed);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Sets the acceleration of the linear stage
 * @param acceleration Acceleration in mm/s^2
 */
void LinearStage::setAcceleration(double accelerationinmm){
  int acceleration = 0;
  char buffer[6];
  char command[6] = "";
  char* number;

  if(accelerationinmm != 0) {
    acceleration = accelerationinmm * 1.6384 / (10000 * m_Stepsize); //transformation from mm/s to datavalue
  } else {
    acceleration =0;
  }

  memcpy(command,STAGE_SET_ACCELERATION,2);
  number = transformDecToText(acceleration);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Configure the interval between each Move Tracking of Manual Move Tracking responses.
 */
void LinearStage::setMoveTrackingPeriod(void){
  char buffer[6];
  char command[6] = "";

  char *settings = transformDecToText(10/*ms*/);
  //char *settings = transformDecToText(50/*ms*/);
  memcpy(command, STAGE_SET_MOVE_TRACKING_PERIOD, 2);
  memcpy(command+2, settings, 1);
  memcpy(buffer, command, 6);

  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Sends linear stage to the home positin.
 */
void LinearStage::home(){
  char buffer[6];
  char command[6] = "";

  memcpy(command, STAGE_GO_HOME, 2);
  memcpy(buffer, command, 6);

  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Sets the speed of the linear stage.
 * @param speedinmm Speed in mm/s
 */
void LinearStage::setSpeed(double speedinmm){
  int speed = 0;
  char buffer[6];
  char command[6] = "";
  char* number;

  if(speedinmm != 0) {
    speed = speedinmm * 1.6384 / m_Stepsize; //transformation from mm/s to datavalue
    m_CurrentSpeed = speed;
  } else {
    speed = m_CurrentSpeed;
  }

  memcpy(command,STAGE_SET_SPEED,2);
  number = transformDecToText(speed);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Stops the linear stage.
 */
void LinearStage::stop(){
  char buffer[6];
  char command[6] = "";

  memcpy(command, STAGE_STOP, 2);
  memcpy(buffer, command, 6);

  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Moves the stage forwards at constant speed
 */
void LinearStage::moveForward(double speedinmm){
  int speed = 0;
  char buffer[6];
  char command[6] = "";
  char *number;

  if(speedinmm != 0) {
    speed = speedinmm * 1.6384 / m_Stepsize; //transformation from mm/s to datavalue
    m_CurrentSpeed = speed;
  } else {
    speed = m_CurrentSpeed;
  }

  memcpy(command, STAGE_MOVE_AT_CONSTANT_SPEED, 2);
  number = transformDecToText(speed);
  memcpy(command+2, number, 4);
  memcpy(buffer, command, 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Moves the stage backwards at constant speed
 */
void LinearStage::moveBackward(double speedinmm){
  int speed = 0;
  char buffer[6];
  char command[6] = "";
  char *number;

  if(speedinmm != 0) {
    speed = speedinmm * 1.6384 / m_Stepsize; //transformation from mm/s to datavalue
    m_CurrentSpeed = speed;
  } else {
    speed = m_CurrentSpeed;
  }

  memcpy(command, STAGE_MOVE_AT_CONSTANT_SPEED, 2);
  number = transformDecToText(-speed);
  memcpy(command+2, number, 4);
  memcpy(buffer, command, 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 5/*ms*/);
  }
}

/**
 * @brief Moves the stage the amount of steps.
 * @param steps Amount of steps
 */
void LinearStage::moveSteps(long steps){
  char buffer[6];
  char command[6]="";
  char* number;
  memcpy(command,STAGE_MOVE_RELATIVE,2);
  number = transformDecToText(steps);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 50/*ms*/);
  }
}

/**
 * @brief Move the stage to the absolute position
 * @param position Absolute stage position.
 */
void LinearStage::moveToAbsolute(long position){
  char buffer[6];
  char command[6]="";
  char* number;
  memcpy(command,STAGE_MOVE_ABSOLUTE,2);
  number = transformDecToText(position);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);
  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 50/*ms*/);
  }

}

/**
 * @brief Transforms a decimal number to a char* variable.
 * @param dec Number in the decimal number system.
 * @return The number as a char* variable.
 */
char* LinearStage::transformDecToText(int dec){
  char bytes[4];
  unsigned int decnumber;
  decnumber=dec;
  if (decnumber<0) {
    decnumber=decnumber+(256*256*256*256);
  }

  bytes[3]=decnumber/(256*256*256);
  decnumber=decnumber-256*256*256*bytes[3];

  bytes[2] =decnumber/(256*256);
  decnumber=decnumber-256*256*bytes[2];

  bytes[1] =decnumber/256;
  decnumber=decnumber-256*bytes[1];

  bytes[0] =decnumber;
  memcpy(mbytedata,bytes,4);
  return(mbytedata);
}
