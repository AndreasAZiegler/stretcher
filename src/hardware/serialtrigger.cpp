/**
 * @file serialtrigger.cpp
 * @brief The serial trigger.
 * @author Andreas Ziegler
 */

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
  char command[6] = "";

  memcpy(command, STAGE_RETURN_STORED_POSITION, 2);
  memcpy(buffer, command, 6);

  {
    lock_guard<mutex> lck{m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 6, 25/*ms*/);
  }
}
