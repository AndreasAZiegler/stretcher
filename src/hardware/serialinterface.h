
#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <string>

/** Abstract base class for serial communication.
 @details We use wxCTB as serial port library to ease up the handling.
 */
class SerialInterface
{
  public:
    /**
     * @brief Initialises the com port and the baud rate and creates the wxSerialPort
     * @param Port com port
     * @param Baudrate baudrate
     */
    SerialInterface(unsigned int baudrate = 9600);

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
    wxSerialPort	m_SerialPort;  /**< Pointer to the serial port object */
    unsigned int  m_Baudrate;    /**< Serial port baud rate */


    int test;

  private:
    SerialInterface(const SerialInterface&) = delete; /**< Don't use (undefined symbol) */
};

#endif /* SERIALINTERFACE_H */
