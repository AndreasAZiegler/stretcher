/**
 * @file messagehandler.h
 * @brief The message handler base class.
 * @author Andreas Ziegler
 */

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include <functional>
#include <memory>
#include <condition_variable>
#include <list>
#include <../updatedvaluessender.h>

/**
 * @class MessageHandler messagehandler.h "hardware/messagehandler.h"
 * @brief Abstract class for the message handler
 */
class MessageHandler : virtual public UpdatedValuesSender
{
  public:

    /**
     * @brief Initializes all the needed variables.
     * @param serialPort Pointer to the serial port owned by the serial interface class
     * @param type Type
     * @param readingSerialInterfaceMutex Pointer to the mutex protecting the serial port access.
     * @param waitmessagehandler Pointer to the condition variable to wait for message handler.
     * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
     * @param messagehandlerfinishednr Pointer to the amount of finished message handlers.
     */
    MessageHandler(wxSerialPort *serialPort,
                   UpdatedValuesReceiver::ValueType type,
                   std::mutex *readingSerialInterfaceMutex,
                   std::shared_ptr<std::condition_variable> waitmessagehandler,
                   std::shared_ptr<std::mutex> waitmessagehandlermutex,
                   std::shared_ptr<int> messagehandlerfinishednr);

    /**
     * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
     */
    virtual void receiver(void) = 0;

    /**
     * @brief Sets the exit flag, that the receiver thread can stop.
     * @param flag flag value.
     */
    void setExitFlag(bool flag);

  protected:

    /**
     * @brief Informs the according method.
     * @param message Received message from the serial interface.
     */
    virtual void handler(char *message) = 0;

    char m_ReceiveBuffer[18];																													/**< Buffer for the received messages */

    wxSerialPort *m_SerialPort;																												/**< Pointer to the serial port object */
    UpdatedValuesReceiver::ValueType m_Type;																					/**< Defines if serial interface represents linear stage 1, 2 or the force sensor */
    std::mutex *m_ReadingSerialInterfaceMutex;																				/**< Pointer to the mutex to protect serial interface of simultanously writing. */
    bool m_ExitFlag;																																	/**< Flag which indicates that the thread should stop */
    std::shared_ptr<std::condition_variable> m_WaitMessageHandler;										/**< Pointer to the condition variable to indicate, that the message handler is finished. */
    std::shared_ptr<std::mutex> m_WaitMessageHandlerMutex;														/**< Pointer to the mutex to protect m_WaitMessageHandler. */
    std::shared_ptr<int> m_MessageHandlersFinishedNumber;															/**< Pointer to the amount of finished message handlers. */

};

#endif // MESSAGEHANDLER_H
