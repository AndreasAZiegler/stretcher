/**
 * @file increasedecreasevelocitytimer.h
 * @brief The increase velocity timer.
 * @author Andreas Ziegler
 */

#ifndef INCREASEDECREASEVELOCITYTIMER_H
#define INCREASEDECREASEVELOCITYTIMER_H

// Includes
#include "../hardware/stageframe.h"

/**
 * @class IncreaseDecreaseVelocityTimer increasedecreasevelocitytimer.h "gui/increasedecreasevelocitytimer.h"
 * @brief Class which implements the methods to increase the speed of the stage frame periodically.
 */
class IncreaseDecreaseVelocityTimer
{
  public:
    /**
     * @brief Initialize all the required variables.
     * @param stageframe Reference to the StageFrame shared pointer.
     * @param velocity The start velocity of the stage frame.
     */
    IncreaseDecreaseVelocityTimer(std::shared_ptr<StageFrame> &stageframe, double velocity);

    /**
     * @brief Sets the exit flag.
     */
    void setExitFlag(void){
      m_ExitFlag = true;
    }

    /**
     * @brief Increases the speed of the linear stage during decreasing the distance, every timer cycle.
     */
    void decreaseDistanceTimer(void);

    /**
     * @brief Increases the speed of the linear stage during increasing the distance, every timer cycle.
     */
    void increaseDistanceTimer(void);

  private:
    std::shared_ptr<StageFrame> m_StageFrame;						/**< Pointer to the stage frame. */
    double m_Velocity;																	/**< The velocity at the start of the timer. */
    bool m_ExitFlag;																		/**< The exit flag. */
};

#endif // INCREASEDECREASEVELOCITYTIMER_H
