/**
 * @file serialtrigger.cpp
 * @brief The serial trigger.
 * @author Andreas Ziegler
 */

// Includes
#include "serialtrigger.h"

/**
 * @brief Creates a SerialInterface and initializes all needed variables.
 * @param baudrate Baudrate
 */
SerialTrigger::SerialTrigger(unsigned int baudrate)
  : SerialInterface(baudrate)
{
}

/**
 * @brief Loads the in the stage stored position.
 */
void SerialTrigger::trigger(void){
  char buffer[6];
  char message[] = "trigger";

  //memcpy(buffer, command, 6);

  {
    std::lock_guard<std::mutex> lck{m_WritingSerialInterfaceMutex};
    //m_SerialPort.Writev(buffer, 6, 25/*ms*/);
    m_SerialPort.Writev(message, 6, 25/*ms*/);
  }
}
