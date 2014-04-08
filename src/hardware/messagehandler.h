
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include <functional>
#include <list>
#include <../updatevalues.h>

// Method pointer typedef
typedef void (UpdateValues::*updateValue)(int, UpdateValues::ValueType);
typedef std::function<void(int, UpdateValues::ValueType)> mp;

/**
 * @brief Abstract class for the message handler
 */
class MessageHandler
{
  public:

    /**
     * @brief Initializes the pointer to the serial port.
     * @param serialPort Pointer to the serial port owned by the serial interface class
     */
    MessageHandler(wxSerialPort *serialPort, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex);

    /**
     * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
     */
    virtual void receiver(void) = 0;

    /**
     * @brief Registers the update methods, which will be called, when the value changes.
     * @param updateMethod Method which updates a value.
     * @param updateClass Pointer to the object, to which the method belongs.
     * @return Id of the callback method.
     */
    std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass);

    /**
     * @brief Unregisters the update methods, which will be called, when the value changes.
     * @param id Id of the callback method
     */
    void unregisterUpdateMethod(std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator id);

    void setExitFlag(bool flag);

  protected:

    /**
     * @brief Informs the according method.
     * @param message Received message from the serial interface.
     */
    virtual void handler(char *message) = 0;

    char m_ReceiveBuffer[18];																													/**< Buffer for the received messages */

    wxSerialPort	*m_SerialPort;																											/**< Pointer to the serial port object */
    UpdateValues::ValueType m_Type;																										/**< Defines if serial interface represents linear stage 1, 2 or the force sensor */
    std::mutex *m_ReadingSerialInterfaceMutex;																				/**< Pointer to the mutex to protect serial interface of simultanously writing. */
    std::list<std::function<void(int, UpdateValues::ValueType)>> m_UpdateMethodList;	/**< List containing struct with pointers to the MyFrame object and the update method*/
    bool m_ExitFlag;

};

#endif // MESSAGEHANDLER_H
