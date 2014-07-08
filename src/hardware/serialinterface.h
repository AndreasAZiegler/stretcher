/**
 * @file serialinterface.h
 * @brief The serial interface base class.
 * @author Andreas Ziegler
 */

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include <string>
#include "../updatedvaluesreceiver.h"

/**
 * @class SerialInterface serialinterface.h "hardware/serialinterface.h"
 * @brief Abstract base class for serial communication.
 * @details We use wxCTB as serial port library to ease up the handling.
 */
class SerialInterface
{
  public:
    /**
     * @brief Initialises the baud rate
     * @param type Type
     * @param Baudrate Baudrate
     */
    SerialInterface(UpdatedValuesReceiver::ValueType type, unsigned int baudrate = 115200);

    /**
     * @brief Closes the port if still open
     */
    virtual ~SerialInterface();

    /**
     * @brief Opens the connection with the specified baud rate and port
     */
    virtual void connect(const char *comPort);

    /**
     * @brief Close the existing connection.
     */
    virtual void disconnect();

  protected:
    wxSerialPort	m_SerialPort;  											/**< The serial port object */
    unsigned int  m_Baudrate;    											/**< Serial port baud rate */
    std::mutex m_AccessSerialInterfaceMutex;					/**< Mutex to protect serial interface of simultanously access. */
    std::mutex m_WritingSerialInterfaceMutex;					/**< Mutex to protect serial interface of simultanously writing. */
    std::mutex m_ReadingSerialInterfaceMutex;					/**< Mutex to protect serial interface of simultanously reading. */
    UpdatedValuesReceiver::ValueType m_Type;					/**< Defines if serial interface represents linear stage 1, 2 or the force sensor */

  private:
    SerialInterface(const SerialInterface&) = delete; /**< Don't use (undefined symbol) */
};

#endif /* SERIALINTERFACE_H */
