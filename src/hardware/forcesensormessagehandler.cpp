/**
 * @file forcesensormessagehandler.cpp
 * @brief The force sensor message handler.
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <cmath>
#include <thread>
#include "forcesensormessagehandler.h"

// Namespaces
using namespace std;

/**
 * @brief Creates a message handler for the force sensor and initializes all the needed variables.
 * @param serialport Pointer to the wxSerialPort object.
 * @param type Type of the message handler.
 * @param readingSerialInterfaceMutex Pointer to the mutex to protect the serial interface access.
 * @param waitmessagehandler Pointer to the condition variable to wait for the message handler.
 * @param waitmessagehandlermutex Pointer to the mutex to protect waitmessagehandler.
 * @param messagehandlerfinishednr Pointer to the amount of finished message handlers.
 */
ForceSensorMessageHandler::ForceSensorMessageHandler(wxSerialPort *serialport,
                                                     UpdatedValuesReceiver::ValueType type,
                                                     std::mutex *readingSerialInterfaceMutex,
                                                     std::shared_ptr<std::condition_variable> waitmessagehandler,
                                                     std::shared_ptr<std::mutex> waitmessagehandlermutex,
                                                     std::shared_ptr<int> messagehandlerfinishednr)
  : MessageHandler(serialport,
                   type,
                   readingSerialInterfaceMutex,
                   waitmessagehandler,
                   waitmessagehandlermutex,
                   messagehandlerfinishednr)//,
    //m_CurrentForce(0)
{
}

/**
 * @brief Destructor
 */
ForceSensorMessageHandler::~ForceSensorMessageHandler(){
  //std::cout << "ForceSensorMessageHandler destructor finished." << std::endl;
}

/**
 * @brief Updated scale factor parameters with the values given from the GUI.
 * @param scalingfactor Force sensor scaling factor
 * @param zerovalue Force sensor zero value
 */
void ForceSensorMessageHandler::setScaleFactor(double scalingfactor, double zerovalue) {
  m_ScalingFactor = scalingfactor;
  m_ZeroValue = zerovalue;
}

/**
 * @brief Sets force zero.
 */
void ForceSensorMessageHandler::setZeroForce(void){
  m_ZeroForceOffset = m_CurrentForce.value;
}

/**
  * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
  */
void ForceSensorMessageHandler::receiver(void){
  size_t answerLen = 0;
  int measforce = 0;

  bool syncReceived;
  int syncPos;

  // Loop until the receiver should stop.
  int lv = 8388608;
  while(m_ExitFlag){

      //measforce = 100 * sin(lv++ * (1/(6.28 * 1000)));
      measforce = lv;
      lv -= 10;
      /*
      measforce = (static_cast<unsigned char>(m_ReceiveBuffer[syncPos+2]) << 16) +
                  (static_cast<unsigned char>(m_ReceiveBuffer[syncPos+3]) << 8) +
                  (static_cast<unsigned char>(m_ReceiveBuffer[syncPos+4]));
                  */

      m_CurrentForce.timestamp = std::chrono::high_resolution_clock::now();
      m_CurrentForce.value = -(measforce - m_ZeroValue) / m_ScalingFactor - m_ZeroForceOffset;
      //std::cout << "PressureSensor force: " << m_CurrentForce << " at pos: " << syncPos << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_AccessListMutex};
        for(auto j = m_UpdateMethodList.begin(); j != m_UpdateMethodList.end(); ++j){
          (*j)(m_CurrentForce, m_Type);
        }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  // Signaling that the message handler is finished.
  {
    std::lock_guard<std::mutex> lck{*m_WaitMessageHandlerMutex};
    (*m_MessageHandlersFinishedNumber)++;
    m_WaitMessageHandler->notify_all();
    std::cout << "ForceSensorMessageHandler finished" << std::endl;
  }
}

/**
 * @brief Informs the related objects according to the received message
 * @param message Received message
 */
void ForceSensorMessageHandler::handler(char *message){
}
