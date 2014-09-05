/**
 * @file increasedecreasevelocitytimer.cpp
 * @brief The increase velocity timer.
 * @author Andreas Ziegler
 */

// Includes
#include <chrono>
#include <thread>
#include "increasedecreasevelocitytimer.h"

/**
 * @brief Initialize all the required variables.
 * @param stageframe Reference to the StageFrame shared pointer.
 * @param velocity The start velocity of the stage frame.
 */
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
  // Run until exit flag is set true by the main frame, when the decrease/increase-button is released.
  while(!m_ExitFlag){
    // Wait two seconds.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    m_Velocity *= 2;
    if(false == m_ExitFlag){
      // Set the new speed of the stage frame, if the exit flag is not already set.
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
