/**
 * @file messagehandler.cpp
 * @brief The message handler base class.
 * @author Andreas Ziegler
 */

// Includes
#include "messagehandler.h"

/**
 * @brief Initializes all the needed variables.
 * @param serialPort Pointer to the serial port owned by the serial interface class
 * @param type Type
 * @param readingSerialInterfaceMutex Pointer to the mutex protecting the serial port access.
 * @param waitmessagehandler Pointer to the condition variable to wait for message handler.
 * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
 * @param messagehandlerfinishednr Pointer to the amount of finished message handlers.
 */
MessageHandler::MessageHandler(wxSerialPort *serialPort,
                               UpdatedValuesReceiver::ValueType type,
                               std::mutex *readingSerialInterfaceMutex,
                               std::shared_ptr<std::condition_variable> waitmessagehandler,
                               std::shared_ptr<std::mutex> waitmessagehandlermutex,
                               std::shared_ptr<int> messagehandlerfinishednr)
  : m_SerialPort(serialPort),
    m_Type(type),
    m_ReadingSerialInterfaceMutex(readingSerialInterfaceMutex),
    m_ExitFlag(true),
    m_WaitMessageHandler(waitmessagehandler),
    m_WaitMessageHandlerMutex(waitmessagehandlermutex),
    m_MessageHandlersFinishedNumber(messagehandlerfinishednr)
{
}

/**
 * @brief Sets the exit flag, that the receiver thread can stop.
 * @param flag flag value.
 */
void MessageHandler::setExitFlag(bool flag){
  m_ExitFlag = flag;
}
