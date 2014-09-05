#ifndef INCREASEDECREASEVELOCITYTIMER_H
#define INCREASEDECREASEVELOCITYTIMER_H

// Includes
#include "../hardware/stageframe.h"

class IncreaseDecreaseVelocityTimer
{
  public:
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
