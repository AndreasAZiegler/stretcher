
#include "messagehandler.h"

/**
 * @brief Initializes the pointer to the serial port.
 * @param serialPort Pointer to the serial port owned by the serial interface class
 */
MessageHandler::MessageHandler(wxSerialPort *serialPort, UpdatedValuesReceiver::ValueType type, std::mutex *readingSerialInterfaceMutex)
  : m_SerialPort(serialPort),
    m_Type(type),
    m_ReadingSerialInterfaceMutex(readingSerialInterfaceMutex),
    m_ExitFlag(true)
{
}

/**
 * @brief Sets the exit flag, that the receiver thread can stop.
 * @param flag flag value.
 */
void MessageHandler::setExitFlag(bool flag){
  m_ExitFlag = flag;
}
