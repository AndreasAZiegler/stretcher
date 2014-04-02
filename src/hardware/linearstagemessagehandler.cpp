
#include <iostream>
#include "linearstagemessagehandler.h"

using namespace std;

/**
 * @brief Forwards the pointer to the serial port to the base class.
 * @param serialport Pointer to the serial port.
 */
LinearStageMessageHandler::LinearStageMessageHandler(wxSerialPort *serialport, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex)
  : MessageHandler(serialport, type, readingSerialInterfaceMutex)
{
}

/**
 * @brief Informs the related objects according to the received message
 * @param message Received message
 */
void LinearStageMessageHandler::handler(char *message){

  int tmpint = 0;
  switch(message[1]){
    case ANSWER_SET_DEVICE_MODE:
      break;

    case ANSWER_CURRENT_POSITION:
      // Notify updated distance
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      break;
    case MESSAGE_CURRENT_POSITION:
      // Notify updated distance
      /*
      for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      */
      /*
      for(std::list<TestStruct>::iterator i = test.begin(); i != test.end(); ++i){
        std::cout << "Entry is: " << (*i).value << std::endl;
      }
      ((m_UpdateMethodList.front().updatevalues)->*(m_UpdateMethodList.front().method))(calculatePosition(&message[1]), m_Type);
      */
      tmpint = calculatePosition(&message[1]);
      (m_UpdateClass->*m_UpdateMethod)(tmpint, m_Type);

      break;
    case ANSWER_GO_HOME:
      // Notify updated distance
      for(std::list<UpdateValuesStruct>::iterator i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      break;
    case ANSWER_MOVE_ABSOLUT:
      // Notify updated distance
      for(std::list<UpdateValuesStruct>::iterator i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      break;
    case ANSWER_MOVE_RELATIVE:
      // Notify updated distance
      for(std::list<UpdateValuesStruct>::iterator i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      break;
    case ANSWER_SET_SPEED:
      break;
    case ANSWER_MOVE_AT_CONSTANT_SPEED:
      break;
    case ANSWER_MOTOR_STOP:
      // Notify updated distance
      for(std::list<UpdateValuesStruct>::iterator i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
        (((*i).updatevalues)->*((*i).method))(calculatePosition(&message[1]), m_Type);
      }
      break;
  }
}

/**
 * @brief Calculates the position from the receiving position data. See manual page. 7
 * @param message Received message.
 */
int LinearStageMessageHandler::calculatePosition(char* message){

 int position = static_cast<unsigned char>(message[5]) * (256*256*256) +
                static_cast<unsigned char>(message[4]) * (256*256) +
                static_cast<unsigned char>(message[3]) * 256 +
                static_cast<unsigned char>(message[2]);

  if(message[5] > 127) {
    position -= 256*256*256*256;
  }
  return(position);
}
