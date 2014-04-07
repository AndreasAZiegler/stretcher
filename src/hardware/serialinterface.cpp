
// Includes
#include "serialinterface.h"

using namespace std;

/**
 * @brief Initialises the com port and the baud rate and creates the wxSerialPort
 * @param Port com port
 * @param Baudrate baudrate
 */
SerialInterface::SerialInterface(UpdateValues::ValueType type, unsigned int baudrate)
  : m_Baudrate(baudrate)
{
  //m_SerialPort = new wxSerialPort();
}

/**
 * @brief Closes the port if still open
 */
SerialInterface::~SerialInterface()
{
  lock_guard<mutex> lck{m_AccessSerialInterfaceMutex};
  if(m_SerialPort.IsOpen()){
    m_SerialPort.Close();
  }
  //delete m_SerialPort;
}

/**
 * @brief Opens the connection with the specified baud rate and port
 */
void SerialInterface::connect(const char* comPort)
{
  wxSerialPort_DCS parameters;
  parameters.baud = (wxBaud)m_Baudrate;
  {
    lock_guard<mutex> lck{m_AccessSerialInterfaceMutex};
    m_SerialPort.Open(comPort, &parameters);
  }
}

/**
 * @brief Close the existing connection.
 */
void SerialInterface::disconnect() {
  {
    lock_guard<mutex> lck{m_AccessSerialInterfaceMutex};
    m_SerialPort.Close();
  }
}
