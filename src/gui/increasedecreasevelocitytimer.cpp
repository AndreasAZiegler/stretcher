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
 * @brief Increases the speed of the linear stage during decreasing the distance, every timer cycle.
 */
void IncreaseDecreaseVelocityTimer::decreaseDistanceTimer(void){
  // Run until exit flag is set true by the main frame, when the decrease/increase-button is released.
  while(!m_ExitFlag){
    // Wait two seconds.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if((false == m_ExitFlag) && (20/*mm/s*/ > m_Velocity)){
      // Set the new speed of the stage frame, if the exit flag is not already set and the speed is lower than 20 mm/s
      m_Velocity *= 2;
      m_StageFrame->moveForward(m_Velocity);
    }
  }
}

/**
 * @brief Increases the speed of the linear stage during increasing the distance, every timer cycle.
 */
void IncreaseDecreaseVelocityTimer::increaseDistanceTimer(void){
  // Run until exit flag is set true by the main frame, when the decrease/increase-button is released.
  while(!m_ExitFlag){
    // Wait two seconds.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if((false == m_ExitFlag) && (20/*mm/s*/ > m_Velocity)){
      // Set the new speed of the stage frame, if the exit flag is not already set and the speed is lower than 20 mm/s
      m_Velocity *= 2;
      m_StageFrame->moveBackward(m_Velocity);
    }
  }
}
