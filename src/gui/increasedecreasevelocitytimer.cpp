// Includes
#include <chrono>
#include <thread>
#include "increasedecreasevelocitytimer.h"

IncreaseDecreaseVelocityTimer::IncreaseDecreaseVelocityTimer(std::shared_ptr<StageFrame> &stageframe, double velocity)
  : m_StageFrame(stageframe),
    m_Velocity(velocity),
    m_ExitFlag(false)
{
}

/**
 * @brief Timer method which increases the linear stage speed every cycle.
 */
void IncreaseDecreaseVelocityTimer::increaseTimer(void){
  while(!m_ExitFlag){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    m_Velocity *= 2;
    if(false == m_ExitFlag){
      m_StageFrame->setSpeed(m_Velocity);
    }
  }
}

/**
 * @brief Timer method which decreases the linear stage speed every cycle.
 */
void IncreaseDecreaseVelocityTimer::decreaseTimer(void){
  while(!m_ExitFlag){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    m_Velocity /= 2;
    if(false == m_ExitFlag){
      m_StageFrame->setSpeed(m_Velocity);
    }
  }
}
