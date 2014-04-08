
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
 * @param updateMethod Method which updates a value.
 * @param updateClass Pointer to the object, to which the method belongs.
 * @return Id of the callback method.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
  m_UpdateMethodList.push_back(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void MessageHandler::unregisterUpdateMethod(std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator id){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
  m_UpdateMethodList.erase(id);
}

void MessageHandler::setExitFlag(bool flag){
  m_ExitFlag = flag;
}
