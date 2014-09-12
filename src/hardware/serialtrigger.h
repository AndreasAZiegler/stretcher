/**
 * @file serialtrigger.h
 * @brief The serial trigger.
 * @author Andreas Ziegler
 */

#ifndef SERIALTRIGGER_H
#define SERIALTRIGGER_H

// Includes
#include "serialinterface.h"

/**
 * @class SerialTrigger serialtrigger.h "hardware/serialtrigger.h"
 * @brief Class that provides access to a device, which can be triggered by RS-232 interface.
 */
class SerialTrigger : public SerialInterface
{
  public:
    /**
     * @brief Creates a SerialInterface and initializes all needed variables.
     * @param baudrate Baudrate
     */
    SerialTrigger(unsigned int baudrate = 115200);

    /**
     * @brief Sends trigger message to the microscope control software.
     */
    void trigger(void);
};

#endif // SERIALTRIGGER_H
