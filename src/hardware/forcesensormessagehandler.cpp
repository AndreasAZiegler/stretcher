
#include <iostream>
#include "forcesensormessagehandler.h"

using namespace std;

/**
 * @brief Forwards the pointer to the serial port to the base class.
 * @param serialport Pointer to the serial port.
 */
ForceSensorMessageHandler::ForceSensorMessageHandler(wxSerialPort *serialport,
                                                     UpdateValues::ValueType type,
                                                     std::mutex *readingSerialInterfaceMutex)
  : MessageHandler(serialport, type, readingSerialInterfaceMutex),
    m_CurrentForce(0)
{
}

/**
 * @brief Updated scale factor parameters with the values given from the GUI.
 * @param scalingfactor Force sensor scaling factor
 * @param zerovalue Force sensor zero value
 */
void ForceSensorMessageHandler::setScaleFactor(double scalingfactor, double zerovalue) {
  m_ScalingFactor = scalingfactor;
  m_ZeroValue = zerovalue;
}

/**
  * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
  */
void ForceSensorMessageHandler::receiver(void){
  size_t answerLen = 0;
  int measforce = 0;
  double force = 0;

  while(m_ExitFlag){
    {
      lock_guard<mutex> lck {*m_ReadingSerialInterfaceMutex};
      //if(true == m_ReadingSerialInterfaceMutex->try_lock()){
        if(1 == m_SerialPort->IsOpen()) {
          answerLen = m_SerialPort->Readv(m_ReceiveBuffer, 5, 5/*ms*/);
          //std::cout << "PressureSensor answerLen: " << answerLen << std::endl;
        }
      //}
      //m_ReadingSerialInterfaceMutex->unlock();
    }

    if(answerLen > 4) {

      for(int i = 0; i < (static_cast<int>(answerLen) - 3); ++i) {
        std::cout << "ForceSensor m_ReceiveBuffer[" << i <<"]: " << static_cast<unsigned int>(m_ReceiveBuffer[i]) << std::endl;
        if((m_ReceiveBuffer[i] == 44 /*sync symbol*/)) { // && (answerBuffer[i + 5] == 44)) {

          //measforce = static_cast<unsigned char>(answerBuffer[2])*256*256 + static_cast<unsigned char>(answerBuffer[3])*256 + static_cast<unsigned char>(answerBuffer[4]);
          measforce = (static_cast<unsigned char>(m_ReceiveBuffer[i+2]) << 16) +
                      (static_cast<unsigned char>(m_ReceiveBuffer[i+3]) << 8) +
                      (static_cast<unsigned char>(m_ReceiveBuffer[i+4]));

          m_CurrentForce = (measforce - m_ZeroValue) / m_ScalingFactor;
          std::cout << "PressureSensor force: " << m_CurrentForce << " at pos: " << i << std::endl;
          for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
            (*i)(m_CurrentForce, m_Type);
          }
        }
      }
    }
  }
}

/**
 * @brief Informs the related objects according to the received message
 * @param message Received message
 */
void ForceSensorMessageHandler::handler(char *message){

  int tmpint = 0;
  /*
  switch(message[1]){
    case ANSWER_SET_DEVICE_MODE:
      break;

    case ANSWER_CURRENT_POSITION:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
    case MESSAGE_CURRENT_POSITION:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
    case ANSWER_GO_HOME:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
    case ANSWER_MOVE_ABSOLUT:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
    case ANSWER_MOVE_RELATIVE:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
    case ANSWER_SET_SPEED:
      break;
    case ANSWER_MOVE_AT_CONSTANT_SPEED:
      break;
    case ANSWER_MOTOR_STOP:
      // Notify updated distance
      //m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentForce, m_Type);
      }
      break;
  }
  */
}
