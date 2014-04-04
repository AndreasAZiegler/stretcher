
#include "messagehandler.h"

/**
 * @brief Initializes the pointer to the serial port.
 * @param serialPort Pointer to the serial port owned by the serial interface class
 */
MessageHandler::MessageHandler(wxSerialPort *serialPort, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex)
  : m_SerialPort(serialPort),
    m_Type(type),
    m_ReadingSerialInterfaceMutex(readingSerialInterfaceMutex),
    m_ExitFlag(true)
{
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

void MessageHandler::setExitFlag(bool flag){
  m_ExitFlag = flag;
}
