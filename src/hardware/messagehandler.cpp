
#include "messagehandler.h"

/**
 * @brief Initializes the pointer to the serial port.
 * @param serialPort Pointer to the serial port owned by the serial interface class
 */
MessageHandler::MessageHandler(wxSerialPort *serialPort, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex)
  : m_SerialPort(serialPort),
    m_Type(type),
    m_ReadingSerialInterfaceMutex(readingSerialInterfaceMutex)
{
}

/**
 * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
 */
void MessageHandler::receiver(void){
  unsigned int timeout = 5/*ms*/;
  int answerLen = 0;
  while(true){
    // Only receive messages, when there is no other method reading from the serial interface.
    if(true == m_ReadingSerialInterfaceMutex->try_lock()){
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
    m_ReadingSerialInterfaceMutex->unlock();

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
 * @brief Registers the update methods, which will be called, when the value changes.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
  m_UpdateMethodList.push_back(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void MessageHandler::unregisterUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
}
