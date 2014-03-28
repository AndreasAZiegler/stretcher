
// Includes
#include "linearstage.h"
#include "../../include/ctb-0.13/serport.h"

/**
 * @brief Forwards the com port and the baud rate to SerialPort
 * @param comPort com port
 * @param baudrate baudrate
 */
LinearStage::LinearStage(unsigned int baudrate)
    : SerialInterface(baudrate),
    m_Stepsize(0.000047625),                    //Stepsize of Zaber T-LSM025A motor in millimeters
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
    MOTORS_DEVICE_MODE("\x00\x028"),
    MOTORS_RESET("\x00\x00"),
    MOTORS_RETURN_CURRENT_POSITION("\x00\x03c"),
    MOTORS_GO_HOME("\x00\x01"),
    MOTORS_MOVE_ABSOLUTE("\x00\x014"),
    MOTORS_MOVE_RELATIVE("\x00\x015"),
    MOTORS_SET_SPEED("\x00\x02a"),
    MOTORS_MOVE_AT_CONSTANT_SPEED("\x00\x016"), //: QObject(parent)
    MOTORS_STOP("\x00\x017"),
    MM_PER_MS(0.000047625)
{
}

LinearStage::~LinearStage()
{
}

void LinearStage::setSpeed(double speed){

}

void LinearStage::stop(){
  char buffer[6];
  char command[6] = "";

  memcpy(command, MOTORS_STOP, 2);
  memcpy(buffer, command, 6);

  m_SerialPort.Writev(buffer, 6, 5/*ms*/);
}

void LinearStage::move(){

}

void LinearStage::moveTo(int position){

}

void LinearStage::moveSteps(int steps){
  char buffer[6];
  char command[6]="";
  char* number;
  memcpy(command,MOTORS_MOVE_RELATIVE,2);
  number = transformDecToText(steps);
  memcpy(command+2,number,4);
  memcpy(buffer,command , 6);

  test = 0;
  test = m_SerialPort.IsOpen();
  if(test){
    test = m_SerialPort.Writev(buffer, 6, 50/*ms*/);
  }
}

void LinearStage::moveMillimeters(double millimeters){
  int steps=0;
  steps=millimeters/m_Stepsize;//transformation from millimeters to steps
  moveSteps(steps);
}

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
