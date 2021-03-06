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
  char message[] = "trigger";

  {
    std::lock_guard<std::mutex> lck{m_WritingSerialInterfaceMutex};
    // Send trigger message.
    m_SerialPort.Writev(message, 8, 25/*ms*/);
  }
}
