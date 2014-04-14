#include <iostream>
#include "stageframe.h"

StageFrame::StageFrame()
  :	m_Stepsize(0.00009921875),                    //Stepsize of Zaber T-LSM025A motor in millimeters
    m_ZeroDistance(0),
    MM_PER_MS(0.00009921875),
    m_Pos1ChangedFlag(false),
    m_Pos2ChangedFlag(false),
    m_CurrentPositions{0, 0},
    m_CurrentDistance(0)
{
}

/**
 * @brief Registers the linear stages and get pointer for the message handlers of the linear stages.
 * @param linearstages Pointer to the vector containing the pointer to the linear stages objects.
 */
void StageFrame::registerLinearStages(std::vector<LinearStage*> *linearstages){
  m_LinearStages = linearstages;

  m_LinearStagesMessageHandlers.push_back((m_LinearStages->at(0))->getMessageHandler());
  m_LinearStagesMessageHandlers.push_back((m_LinearStages->at(1))->getMessageHandler());

  (m_LinearStagesMessageHandlers.at(0))->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  (m_LinearStagesMessageHandlers.at(1))->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Registers the update methods, which will be called, when the value changes.
 * @param updateMethod Method which updates a value.
 * @param updateClass Pointer to the object, to which the method belongs.
 * @return Id of the callback method.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator StageFrame::registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass){
  return(m_UpdateMethodList.insert(m_UpdateMethodList.end(), std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2)));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void StageFrame::unregisterUpdateMethod(std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator id){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
  m_UpdateMethodList.erase(id);
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void StageFrame::updateValues(long value, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Pos1:
      //std::cout << "Stage frame pos 1 update" << std::endl;
      m_CurrentPositions[0] = value;
      //std::cout << "m_CurrentPositions[0]: " << m_CurrentPositions[0] << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_PosChangedMutex};
        if(m_Pos2ChangedFlag){
          m_Pos1ChangedFlag = false;
          m_Pos2ChangedFlag = false;
          m_CurrentDistance = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0]) +
                               std::abs(771029 - m_CurrentPositions[1]));// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */;
          // notify
          for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
            (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
          }
          //std::cout << "Stage frame notification!" << std::endl;
        }else{
          m_Pos1ChangedFlag = true;
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Pos2:
      //std::cout << "Stage frame pos 2 update" << std::endl;
      m_CurrentPositions[1] = value;
      //std::cout << "m_CurrentPositions[1]: " << m_CurrentPositions[1] << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_PosChangedMutex};
        if(m_Pos1ChangedFlag){
          m_Pos2ChangedFlag = false;
          m_Pos1ChangedFlag = false;
          m_CurrentDistance = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0]) +
                               std::abs(771029 - m_CurrentPositions[1]));// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */; // notify
          for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
            (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
          }
          //std::cout << "Stage frame notification!" << std::endl;
        }else{
          m_Pos2ChangedFlag = true;
        }
      }
      break;
  }
}

/**
 * @brief Moves the stage frame forward at constant speed
 */
void StageFrame::moveForward(double speed){
 (m_LinearStages->at(0))->moveForward(speed);
 (m_LinearStages->at(1))->moveForward(speed);
}

/**
 * @brief Moves the stage frame backward at constant speed
 */
void StageFrame::moveBackward(double speed){
 (m_LinearStages->at(0))->moveBackward(speed);
 (m_LinearStages->at(1))->moveBackward(speed);
}

/**
 * @brief Moves the stage the amount of millimeters.
 * @param milimeters Amount of milimeters
 */
void StageFrame::moveMM(double millimeters){
  int steps=0;
  steps = millimeters / m_Stepsize;//transformation from millimeters to steps
  (m_LinearStages->at(0))->moveSteps(steps);
  (m_LinearStages->at(1))->moveSteps(steps);
}

/**
 * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
 *        and start the motors.
 * @param distance Desired istance in milli meters.
 */
void StageFrame::gotoMMDistance(int mmDistance){
  //int currentDistance = getCurrentDistance();

  long dist = (mmDistance/MM_PER_MS);
  //long amSteps = (currentDistance - (mmDistance/MM_PER_MS)) / 2;
  //long amSteps = (m_CurrentDistance - dist) / 2;
  long position = (771029 /*max. position*/ - (dist / 2));
  (m_LinearStages->at(0))->moveSteps(position);
  (m_LinearStages->at(1))->moveSteps(position);
}

/**
 * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
 *        and start the motors.
 * @param distance Desired clamping distance in micro steps from the GUI
 */
void StageFrame::gotoStepsDistance(long stepsDistance){
  //long amSteps = (m_CurrentDistance - stepsDistance) / 2;
  long position = (771029 /*max. position*/ - (stepsDistance / 2));
  /*
  (m_LinearStages->at(0))->moveSteps(position);
  (m_LinearStages->at(1))->moveSteps(position);
  */
  (m_LinearStages->at(0))->moveToAbsolute(position);
  (m_LinearStages->at(1))->moveToAbsolute(position);
}

/**
 * @brief Move the stage to the absolute position
 * @param position Absolute stage position.
 */
void StageFrame::gotoToAbsolute(long position){
  (m_LinearStages->at(0))->moveToAbsolute(position);
  (m_LinearStages->at(1))->moveToAbsolute(position);
}

/**
 * @brief Stops the stage frame.
 */
void StageFrame::stop(){
  (m_LinearStages->at(0))->stop();
  (m_LinearStages->at(1))->stop();
  std::cout << "Stop stages." << std::endl;
}

long StageFrame::getCurrentDistance(void){
  return(std::abs(771029 /*max. position*/ - m_CurrentPositions[0]) +
         std::abs(771029 /*max. position*/ - m_CurrentPositions[1]) +
         m_ZeroDistance);
}
