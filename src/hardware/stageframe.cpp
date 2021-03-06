/**
 * @file stageframe.cpp
 * @brief The stage frame.
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <thread>
#include <chrono>
#include <wx/log.h>
#include "stageframe.h"
#include "linearstage.h"
#include "linearstagemessagehandler.h"

/**
 * @brief Initializes all the needed variables.
 */
StageFrame::StageFrame()
  :	m_Stepsize(0.00009921875),                    //Stepsize of Zaber T-LSM025A motor in millimeters
    m_MaxPosDistance(0),
    m_ZeroDistanceOffset(0),
    MM_PER_MS(0.00009921875),
    m_Pos1ChangedFlag(false),
    m_Pos2ChangedFlag(false),
    m_CurrentPositions(2),
    //m_CurrentDistance{0,0},
    m_AmStopped(0)
{
}

/**
 * @brief Unregisters the update method from the message handlers.
 */
StageFrame::~StageFrame(){
  (m_LinearStagesMessageHandlers.at(0))->unregisterUpdateMethod(m_Position1Id);
  (m_LinearStagesMessageHandlers.at(1))->unregisterUpdateMethod(m_Position2Id);
}

/**
 * @brief Registers the linear stages and get pointer for the message handlers of the linear stages.
 * @param linearstages Pointer to the vector containing the pointer to the linear stages objects.
 */
void StageFrame::registerLinearStages(std::vector<std::shared_ptr<LinearStage>> &linearstages){
  m_LinearStages.push_back(linearstages.at(0));
  m_LinearStages.push_back(linearstages.at(1));

  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(0))->getMessageHandler());
  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(1))->getMessageHandler());

  m_Position1Id = (m_LinearStagesMessageHandlers.at(0))->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_Position2Id = (m_LinearStagesMessageHandlers.at(1))->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Registers the update methods, which will be called, when the value changes.
 * @param updateMethod Method which updates a value.
 * @param updateClass Pointer to the object, to which the method belongs.
 * @return Id of the callback method.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
std::list<std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator StageFrame::registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass){
  std::lock_guard<std::mutex> lck{m_AccessListMutex};
  return(m_UpdateMethodList.insert(m_UpdateMethodList.end(), std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2)));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void StageFrame::unregisterUpdateMethod(std::list<std::function<void(MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator id){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
  std::lock_guard<std::mutex> lck{m_AccessListMutex};
  m_UpdateMethodList.erase(id);
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values. Calculates the distance from the two positions and forward it.
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void StageFrame::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Pos1:
      //std::cout << "Stage frame pos 1 update" << std::endl;
      m_CurrentPositions[0].value = measurementValue.value;
      m_CurrentPositions[0].timestamp = measurementValue.timestamp;
      //std::cout << "m_CurrentPositions[0]: " << m_CurrentPositions[0] << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_PosChangedMutex};
        if(m_Pos2ChangedFlag){
          m_Pos1ChangedFlag = false;
          m_Pos2ChangedFlag = false;
          //std::cout << "Stage frame time difference: " << std::chrono::duration_cast<std::chrono::microseconds>(m_CurrentPositions[0].timestamp - m_CurrentPositions[1].timestamp).count() << " msec" << std::endl;
          m_CurrentDistance.timestamp = m_CurrentPositions[0].timestamp;
          m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                                     std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */;
          // notify
          {
            std::lock_guard<std::mutex> lck{m_AccessListMutex};
            for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
              (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
            }
          }
          //std::cout << "Stage frame notification!" << std::endl;
        }else{
          m_Pos1ChangedFlag = true;
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Pos2:
      //std::cout << "Stage frame pos 2 update" << std::endl;
      m_CurrentPositions[1].value = measurementValue.value;
      m_CurrentPositions[1].timestamp = measurementValue.timestamp;
      //std::cout << "m_CurrentPositions[1]: " << m_CurrentPositions[1] << std::endl;
      {
        std::lock_guard<std::mutex> lck{m_PosChangedMutex};
        if(m_Pos1ChangedFlag){
          m_Pos2ChangedFlag = false;
          m_Pos1ChangedFlag = false;
          //std::cout << "Stage frame time difference: " << std::chrono::duration_cast<std::chrono::milliseconds>(m_CurrentPositions[1].timestamp - m_CurrentPositions[0].timestamp).count() << " msec" << std::endl;
          m_CurrentDistance.timestamp = m_CurrentPositions[1].timestamp;
          m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                                     std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */; // notify
          {
            std::lock_guard<std::mutex> lck{m_AccessListMutex};
            for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
              (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
            }
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
 * @brief Takes the stored poition as current position and sets the current stage position.
 * @param position The stored position.
 */
void StageFrame::returnStoredPosition(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  if(UpdatedValuesReceiver::ValueType::Pos1 == type){
    m_CurrentPositions[0].value = measurementValue.value;
    m_CurrentPositions[0].timestamp = measurementValue.timestamp;
    (m_LinearStages.at(0))->setCurrentPosition(measurementValue.value);
  }else if(UpdatedValuesReceiver::ValueType::Pos2 == type){
    m_CurrentPositions[1].value = measurementValue.value;
    m_CurrentPositions[1].timestamp = measurementValue.timestamp;
    (m_LinearStages.at(1))->setCurrentPosition(measurementValue.value);
  }

  m_CurrentDistance.timestamp = m_CurrentPositions[1].timestamp;
  m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                             std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);//  ; //134173 /*microsteps=6.39mm offset */; // notify
  {
    std::lock_guard<std::mutex> lck{m_AccessListMutex};
    for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
      (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
      wxLogMessage(std::string("Current position resetted. Current distance: " + std::to_string(m_CurrentDistance.value * m_Stepsize) + " mm").c_str());
    }
  }
}

/**
 * @brief Sets the speed of the linear stage.
 * @param speedinmm Speed in mm/s
 */
void StageFrame::setSpeed(double speedinmm){
 // Devides the speed by 2 to reach the speed with the 2 stages.
 (m_LinearStages.at(0))->setSpeed(speedinmm / 2);
 (m_LinearStages.at(1))->setSpeed(speedinmm / 2);
}

/**
 * @brief Moves the stage frame forward at constant speed
 */
void StageFrame::moveForward(double speed){
 (m_LinearStages.at(0))->moveForward(speed / 2);
 (m_LinearStages.at(1))->moveForward(speed / 2);
}

/**
 * @brief Moves the stage frame backward at constant speed
 */
void StageFrame::moveBackward(double speed){
 (m_LinearStages.at(0))->moveBackward(speed / 2);
 (m_LinearStages.at(1))->moveBackward(speed / 2);
}

/**
 * @brief Moves the stage the amount of millimeters.
 * @param milimeters Amount of milimeters
 */
void StageFrame::moveMM(double millimeters){
  int steps=0;
  steps = millimeters / m_Stepsize;//transformation from millimeters to steps
  (m_LinearStages.at(0))->moveSteps(steps);
  (m_LinearStages.at(1))->moveSteps(steps);
}

/**
 * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
 *        and start the motors.
 * @param distance Desired istance in milli meters.
 */
void StageFrame::gotoMMDistance(double mmDistance){

  long dist = (mmDistance/MM_PER_MS);
  long position = (771029 /*max. position*/ + (m_MaxPosDistance / 2) - (m_ZeroDistanceOffset / 2) - (dist / 2));
  (m_LinearStages.at(0))->moveToAbsolute(position);
  (m_LinearStages.at(1))->moveToAbsolute(position);
}

/**
 * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
 *        and start the motors.
 * @param distance Desired clamping distance in micro steps from the GUI
 */
void StageFrame::gotoStepsDistance(long stepsDistance){

  long position = (771029 /*max. position*/ + (m_MaxPosDistance / 2) - (m_ZeroDistanceOffset / 2) - (stepsDistance / 2));
  (m_LinearStages.at(0))->moveToAbsolute(position);
  (m_LinearStages.at(1))->moveToAbsolute(position);
}

/**
 * @brief Move the stage to the absolute position
 * @param position Absolute stage position.
 */
void StageFrame::gotoToAbsolute(long position){
  (m_LinearStages.at(0))->moveToAbsolute(position);
  (m_LinearStages.at(1))->moveToAbsolute(position);
}

/**
 * @brief Stops the stage frame.
 */
void StageFrame::stop(void){
  (m_LinearStages.at(0))->stop();
  (m_LinearStages.at(1))->stop();
  //std::cout << "Stop stages." << std::endl;
}

/**
 * @brief Is executed by a linear stage messsage handler to indicate, that one stage stopped.
 */
void StageFrame::stopped(void){

  std::lock_guard<std::mutex> lck{m_AmStoppedMutex};
  m_AmStopped++;

  if(m_AmStopped >= 2){
    m_AmStopped = 0;

    {
      std::lock_guard<std::mutex> lck(*m_StagesStoppedMutex);
      *m_StagesStoppedFlag = true;
    }
  }
}

/**
 * @brief Sends linear stage to the home positin.
 */
void StageFrame::home(void){
  (m_LinearStages.at(0))->home();
  (m_LinearStages.at(1))->home();
}

/**
 * @brief Returns the current distance.
 * @return The current distance.
 */
long StageFrame::getCurrentDistance(void){
  return(std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
         std::abs(771029 /*max. position*/ - m_CurrentPositions[1].value) +
         m_MaxPosDistance - m_ZeroDistanceOffset);
}

/**
 * @brief Sets the maximum position of the stages.
 * @param limit Upper limit in mm.
 */
void StageFrame::setMaxDistanceLimitMM(long limit){
  long dist = (limit/MM_PER_MS);
  setMaxDistanceLimitMS(dist);
}

/**
 * @brief Sets the maximum position of the stages.
 * @param limit Upper limit in micro steps.
 */
void StageFrame::setMaxDistanceLimitMS(long limit){
  long position = (771029 /*max. position*/ + (m_MaxPosDistance / 2) - (m_ZeroDistanceOffset / 2) - (limit / 2));
  //long position = (771029 /*max. position*/ - ((limit / 2) / m_Stepsize));

  (m_LinearStages.at(0))->setMinLimit(position);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(10)));
  (m_LinearStages.at(1))->setMinLimit(position);
  //std::cout << "StageFrame max limit position: " << position << std::endl;
}

/**
 * @brief Sets the minimum position of the stages.
 * @param limit Lower limit in mm.
 */
void StageFrame::setMinDistanceLimitMM(long limit){
  long dist = (limit/MM_PER_MS);
  setMinDistanceLimitMS(dist);
}

/**
 * @brief Sets the minimum position of the stages.
 * @param limit Lower limit in micro steps.
 */
void StageFrame::setMinDistanceLimitMS(long limit){
  long position = (771029 /*max. position*/ + (m_MaxPosDistance / 2) - (m_ZeroDistanceOffset / 2) - (limit / 2));
  //long position = (771029 /*max. position*/ - ((limit / 2) / m_Stepsize));

  (m_LinearStages.at(0))->setMaxLimit(position);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(10)));
  (m_LinearStages.at(1))->setMaxLimit(position);
  //std::cout << "StageFrame min limit position: " << position << std::endl;
}

/**
 * @brief Calculates and sets the maximum position distance. Updates the current distance
 * @param distance Distance in microsteps.
 * @return maximum position distance.
 */
long StageFrame::setDistanceWActuatorCollision(double distance){
  //m_MaxPosDistance = m_CurrentDistance.value + m_MaxPosDistance - distance;
  m_MaxPosDistance = distance - (m_CurrentDistance.value - m_MaxPosDistance);
  /*
  wxLogMessage(std::string("StageFrame: m_MaxPosDistance: " + std::to_string(m_MaxPosDistance) +
                           ", " + std::to_string(m_MaxPosDistance * 0.00009921875/*mm per micro step*//*)).c_str());
  */
  wxLogMessage(std::string("Distance at maximum positions: " + std::to_string(m_MaxPosDistance * m_Stepsize) + " mm").c_str());

  m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                             std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */; // notify
  {
    std::lock_guard<std::mutex> lck{m_AccessListMutex};
    for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
      (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
    }
  }
  return(m_MaxPosDistance);
}

/**
 * @brief Sets the distance at maximum positions.
 * @param maxposdistance The distance at maximum positions.
 */
void StageFrame::setMaxPosDistance(long maxposdistance){
  m_MaxPosDistance = maxposdistance;

  /*
  wxLogMessage(std::string("StageFrame: m_MaxPosDistance: " + std::to_string(m_MaxPosDistance) +
                           ", " + std::to_string(m_MaxPosDistance * 0.00009921875/*mm per micro step*//*)).c_str());
  */
  wxLogMessage(std::string("Distance at maximum position: " + std::to_string(m_MaxPosDistance * m_Stepsize) + " mm").c_str());

  m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                             std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */; // notify
  {
    std::lock_guard<std::mutex> lck{m_AccessListMutex};
    for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
      (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
    }
  }
}

/**
 * @brief Zero distance.
 */
void StageFrame::setZeroDistanceOffset(void){
  m_ZeroDistanceOffset = m_CurrentDistance.value;

  m_CurrentDistance.value = (std::abs(771029 /*max. position*/ - m_CurrentPositions[0].value) +
                             std::abs(771029 - m_CurrentPositions[1].value) + m_MaxPosDistance - m_ZeroDistanceOffset);// + mZeroDistance ; //134173 /*microsteps=6.39mm offset */; // notify
  {
    std::lock_guard<std::mutex> lck{m_AccessListMutex};
    for(auto i = m_UpdateMethodList.begin(); i != m_UpdateMethodList.end(); ++i){
      (*i)(m_CurrentDistance, UpdatedValuesReceiver::ValueType::Distance);
    }
  }
}
