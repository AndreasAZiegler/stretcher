#include <iostream>
#include "preload.h"

Preload::Preload(Experiment::ExperimentType type,
                 Experiment::ForceOrStress forceOrStress,
                 StageFrame *stageframe,
                 ForceSensorMessageHandler *forcesensormessagehandler,
                 double stressForceLimit,
                 double speedInMM,
                 double area)
  : Experiment(type, forceOrStress),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_ForceStressLimit(stressForceLimit),
    m_SpeedInMM(speedInMM),
    m_Area(area),
    m_CurrentState(State::stopState),
    m_CurrentDirection(Direction::Stop),
    m_ForceStressThreshold(0.3)
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdateValues::updateValue, this);
}

Preload::~Preload(){
  std::cout << "Preload destructor." << std::endl;
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
}

/**
 * @brief Sets the area.
 * @param x Length in x direction.
 * @param y Length in y direction.
 */
void Preload::setArea(double x, double y){
  m_Area = x * y;
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Preload::updateValue(int value, UpdateValues::ValueType type){
  if(UpdateValues::ValueType::Force == type){
    m_CurrentForce = value / 10000.0;

    process(Event::evUpdate);
  }
}

/**
 * @brief FSM of the preload process
 * @param e Occuring event
 */
void Preload::process(Event e){
  switch(m_CurrentState){
    case stopState:
      if(evStart == e){
        //std::cout << "Preload FSM switched to state: runState." << std::endl;
        m_CurrentState = runState;

        if((m_CurrentForce - m_ForceStressLimit) > m_ForceStressThreshold){
          //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
          m_CurrentDirection = Direction::Forwards;
          m_StageFrame->moveForward();
        }else if((m_ForceStressLimit - m_CurrentForce) > m_ForceStressThreshold){
          //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
          m_CurrentDirection = Direction::Backwards;
          m_StageFrame->moveBackward();
        }
      }
      break;
    case runState:
      if(evStop == e){
        std::cout << "Preload FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_StageFrame->stop();
      }
      if(evUpdate == e){
        if((m_CurrentForce - m_ForceStressLimit) > m_ForceStressThreshold){
          //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

          if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward();
          }
        }else if((m_ForceStressLimit - m_CurrentForce) > m_ForceStressThreshold){ // Only reverse motor, if state changed
          //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

          if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward();
          }
        }else{

          if((Direction::Forwards == m_CurrentDirection) || (Direction::Backwards == m_CurrentDirection)){
            m_CurrentState = stopState;
            m_CurrentDirection = Direction::Stop;
            m_StageFrame->stop();
          }
        }
        break;
      }
  }
}
