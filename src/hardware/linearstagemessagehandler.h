/**
 * @file linearstagemessagehandler.h
 * @brief The linear stage message handler.
 * @author Andreas Ziegler
 */

#ifndef LINEARSTAGEMESSAGEHANDLER_H
#define LINEARSTAGEMESSAGEHANDLER_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include <condition_variable>
#include "messagehandler.h"
#include "stageframe.h"

// Forward declaration
class StageFrame;

/**
 * @class LinearStageMessageHandler linearstagemessagehandler.h "hardware/linearstagemessagehandler.h"
 * @brief Message handler for the linear stages
 */
class LinearStageMessageHandler : virtual public MessageHandler
{
  public:

    /**
     * @brief Creates a message handler for the linear stage and initializes all the needed variables.
     * @param serialport Pointer to the wxSerialPort object.
     * @param type Type
     * @param readingSerialInterfaceMutex Pointer to the mutex protecting the serial port access.
     * @param waitmessagehandler Pointer to the condition variable to wait for message handler.
     * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
     * @param messagehandlerfinishednr Pointer to the amount of finished message handlers.
     */
    LinearStageMessageHandler(wxSerialPort *serialport,
                              UpdatedValuesReceiver::ValueType type,
                              std::mutex *readingSerialInterfaceMutex,
                              std::shared_ptr<std::condition_variable> waitmessagehandler,
                              std::shared_ptr<std::mutex> waitmessagehandlermutex,
                              std::shared_ptr<int> messagehandlerfinishednr);

    /**
     * @brief Destructor
     */
    ~LinearStageMessageHandler();

    void startGenerateData(void);

    /**
     * @brief Registers the stage frame object.
     * @param stageframe Pointer to the stage frame object.
     */
    void registerStageFrame(std::shared_ptr<StageFrame> stageframe){
      m_StageFrame = stageframe;
    }

    /**
     * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
     */
    virtual void receiver(void);

    /**
     * @brief Informs the related objects according to the received message
     * @param message Received message
     */
    void handler(char *message);

    /**
     * @brief Returns the latest position of the linear stage
     * @return latest position of the linear stage.
     */
    MeasurementValue getCurrentPosition(){
      return(m_CurrentPosition);
    }

  private:
    enum { ANSWER_RESET = 0x00 };											/**< Answer from the command "Reset" */
    enum { ANSWER_SET_DEVICE_MODE = 0x28 };						/**< Answer from the command "Set device mode" */
    enum { MESSAGE_CURRENT_POSITION = 0x08 };         /**< Reply only message from the motors during moving */
    enum { ANSWER_CURRENT_POSITION = 0x3c };          /**< Answer from the command "Set Current Position" */
    enum { ANSWER_GO_HOME = 0x01 };                   /**< Answer from the command "Home" */
    enum { ANSWER_MOVE_ABSOLUT = 0x14 };              /**< Answer from the command "Move Absolute" */
    enum { ANSWER_MOVE_RELATIVE = 0x15 };             /**< Answer from the command "Move Relative" */
    enum { ANSWER_SET_SPEED = 0x2a };                 /**< Answer from the command "Set Target Speed" */
    enum { ANSWER_MOVE_AT_CONSTANT_SPEED = 0x16 };    /**< Answer from the command "Move At Constant Speed" */
    enum { ANSWER_MOTOR_STOP = 0x17 };                /**< Answer from the command "Stop" */
    enum { ANSWER_RETURN_STORED_POSITION = 0x11 };

    bool m_GenerateDataFlag;

    /**
     * @brief Calculates the position from the receiving position data. See manual page. 7
     * @param message Received message.
     */
    long calculatePosition(char* message);

    MeasurementValue m_CurrentPosition;								/**< Struct containing the current position and its time stamp */

    std::shared_ptr<StageFrame> m_StageFrame;					/**< Pointer to the stage frame object */

};

#endif // LINEARSTAGEMESSAGEHANDLER_H
