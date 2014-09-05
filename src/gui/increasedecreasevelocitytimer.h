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
     * @brief Sets the exit flag true.
     */
    void setExitFlag(void){
      m_ExitFlag = true;
    }

    /**
     * @brief Timer method which increases the stage frame speed every cycle.
     */
    void increaseTimer(void);

    /**
     * @brief Timer method which decreases the stage frame speed every cycle.
     */
    void decreaseTimer(void);

  private:
    std::shared_ptr<StageFrame> m_StageFrame;
    double m_Velocity;
    bool m_ExitFlag;
};

#endif // INCREASEDECREASEVELOCITYTIMER_H
