/**
 * @file linearstagemessagehandler.cpp
 * @brief The linear stage message handler.
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <chrono>
#include <wx/log.h>
#include "linearstagemessagehandler.h"

// Namespaces
using namespace std;

/**
 * @brief Creates a message handler for the linear stage and initializes all the needed variables.
 * @param serialport Pointer to the wxSerialPort object.
 * @param type Type
 * @param readingSerialInterfaceMutex Pointer to the mutex protecting the serial port access.
 * @param waitmessagehandler Pointer to the condition variable to wait for message handler.
 * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
 * @param messagehandlerfinishednr Pointer to the amount of finished message handlers.
 */
LinearStageMessageHandler::LinearStageMessageHandler(wxSerialPort *serialport,
                                                     UpdatedValuesReceiver::ValueType type,
                                                     std::mutex *readingSerialInterfaceMutex,
                                                     std::shared_ptr<std::condition_variable> waitmessagehandler,
                                                     std::shared_ptr<std::mutex> waitmessagehandlermutex,
                                                     std::shared_ptr<int> messagehandlerfinishednr)
  : MessageHandler(serialport,
                   type,
                   readingSerialInterfaceMutex,
                   waitmessagehandler,
                   waitmessagehandlermutex,
                   messagehandlerfinishednr)//,
{
}

/**
 * @brief Destructor
 */
LinearStageMessageHandler::~LinearStageMessageHandler(){
  //std::cout << "LinearStageMessageHandler destructor finished." << std::endl;
}

/**
 * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
 */
void LinearStageMessageHandler::receiver(void){
  unsigned int timeout = 5/*ms*/;
  size_t answerLen = 0;
  while(m_ExitFlag){
    // Only receive messages, when there is no other method reading from the serial interface.
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
  // Signaling that the message handler is finished.
  {
    std::lock_guard<std::mutex> lck{*m_WaitMessageHandlerMutex};
    (*m_MessageHandlersFinishedNumber)++;
    m_WaitMessageHandler->notify_all();
    std::cout << "LinearStageMessageHandler finished" << std::endl;
  }
}

/**
 * @brief Informs the related objects according to the received message.
 * @param message Received message
 */
void LinearStageMessageHandler::handler(char *message){

  int tmpint = 0;
  switch(message[1]){
    case ANSWER_SET_DEVICE_MODE:
      break;

    case ANSWER_CURRENT_POSITION:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      break;
    case MESSAGE_CURRENT_POSITION:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      break;
    case ANSWER_GO_HOME:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      break;
    case ANSWER_MOVE_ABSOLUT:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      //std::cout << "ANSWER_MOVE_ABSOLUT m_CurrentPosition: " << m_CurrentPosition * 0.00009921875 << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      break;
    case ANSWER_MOVE_RELATIVE:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      //std::cout << "ANSWER_MOVE_RELATIVE m_CurrentPosition: " << m_CurrentPosition * 0.00009921875 << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      break;
    case ANSWER_SET_SPEED:
      break;
    case ANSWER_MOVE_AT_CONSTANT_SPEED:
      break;
    case ANSWER_MOTOR_STOP:
      // Notify updated distance
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      //std::cout << "ANSWER_MOTOR_STOP m_CurrentPosition: " << m_CurrentPosition * 0.00009921875 << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
          (*i)(m_CurrentPosition, m_Type);
        }
      }
      m_StageFrame->stopped();
      break;
    case ANSWER_RETURN_STORED_POSITION:
      m_CurrentPosition.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentPosition.value = calculatePosition(&message[1]);
      // Forwards the returned stored position to the stage frame.
      m_StageFrame->returnStoredPosition(m_CurrentPosition, m_Type);
      //wxLogMessage(std::string("LinearStageMessageHandler: " + std::to_string(static_cast<int>(m_Type)) + " stored position is: " + std::to_string(m_CurrentPosition.value * 0.00009921875)).c_str());
      wxLogMessage(std::string("Linear stage: " + std::to_string(static_cast<int>(m_Type)) + " stored position is: " + std::to_string(m_CurrentPosition.value * 0.00009921875) + " mm").c_str());
      break;
  }
}

/**
 * @brief Calculates the position from the receiving position data. See manual page. 7
 * @param message Received message.
 */
long LinearStageMessageHandler::calculatePosition(char* message){

 long position = static_cast<unsigned char>(message[4]) * (256*256*256) +
                 static_cast<unsigned char>(message[3]) * (256*256) +
                 static_cast<unsigned char>(message[2]) * 256 +
                 static_cast<unsigned char>(message[1]);

  if(message[5] > 127) {
    position -= 256*256*256*256;
  }
  return(position);
}
