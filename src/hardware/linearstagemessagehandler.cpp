
#include <iostream>
#include "linearstagemessagehandler.h"

using namespace std;

/**
 * @brief Forwards the pointer to the serial port to the base class.
 * @param serialport Pointer to the serial port.
 */
LinearStageMessageHandler::LinearStageMessageHandler(wxSerialPort *serialport, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex)
  : MessageHandler(serialport, type, readingSerialInterfaceMutex),
    m_CurrentPosition(0)
{
}

/**
 * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
 */
void LinearStageMessageHandler::receiver(void){
  unsigned int timeout = 5/*ms*/;
  size_t answerLen = 0;
  while(m_ExitFlag){
    // Only receive messages, when there is no other method reading from the serial interface.
    //if(true == m_ReadingSerialInterfaceMutex->try_lock()){
    {
      lock_guard<mutex> lck {*m_ReadingSerialInterfaceMutex};
      if(1 == m_SerialPort->IsOpen()){
        answerLen = m_SerialPort->Readv(m_ReceiveBuffer, 6, timeout);
        if(0 < answerLen){
          // Read data until 1 message is received
          while(6 > answerLen){
            answerLen += m_SerialPort->Readv(&m_ReceiveBuffer[answerLen], (6 - answerLen) , timeout);
          }
        }
      }
    }
    //}
    //m_ReadingSerialInterfaceMutex->unlock();

    // If messages are received, process the messages.
    if(answerLen > 0){
      // Check the whole buffer in the case, that the "Command" is not the second byte.
      for(int i = 0; i < (static_cast<int>(answerLen) - 5); ++i){
        if(0x01 == m_ReceiveBuffer[i]){
          handler(&m_ReceiveBuffer[i]);
        }
      }
    }
  }
}

/**
 * @brief Informs the related objects according to the received message
 * @param message Received message
 */
void LinearStageMessageHandler::handler(char *message){

  int tmpint = 0;
  switch(message[1]){
    case ANSWER_SET_DEVICE_MODE:
      break;

    case ANSWER_CURRENT_POSITION:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
    case MESSAGE_CURRENT_POSITION:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
    case ANSWER_GO_HOME:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
    case ANSWER_MOVE_ABSOLUT:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
    case ANSWER_MOVE_RELATIVE:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
    case ANSWER_SET_SPEED:
      break;
    case ANSWER_MOVE_AT_CONSTANT_SPEED:
      break;
    case ANSWER_MOTOR_STOP:
      // Notify updated distance
      m_CurrentPosition = calculatePosition(&message[1]);
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (*i)(m_CurrentPosition, m_Type);
      }
      break;
  }
}

/**
 * @brief Calculates the position from the receiving position data. See manual page. 7
 * @param message Received message.
 */
int LinearStageMessageHandler::calculatePosition(char* message){

 int position = static_cast<unsigned char>(message[5]) * (256*256*256) +
                static_cast<unsigned char>(message[4]) * (256*256) +
                static_cast<unsigned char>(message[3]) * 256 +
                static_cast<unsigned char>(message[2]);

  if(message[5] > 127) {
    position -= 256*256*256*256;
  }
  return(position);
}
