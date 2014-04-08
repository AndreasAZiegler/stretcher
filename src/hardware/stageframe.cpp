#include "stageframe.h"


StageFrame::StageFrame()
  :	m_Stepsize(0.00009921875),                    //Stepsize of Zaber T-LSM025A motor in millimeters
    m_ZeroDistance(0),
    MM_PER_MS(0.00009921875)
{
}

void StageFrame::registerLinearStages(std::vector<LinearStage*> *linearstages){
  m_LinearStages = linearstages;

  m_LinearStagesMessageHandlers.push_back((m_LinearStages->at(0))->getMessageHandler());
  m_LinearStagesMessageHandlers.push_back((m_LinearStages->at(1))->getMessageHandler());
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
  int currentDistance = getCurrentDistance();

  long amSteps = (currentDistance - (mmDistance/MM_PER_MS)) / 2;
  (m_LinearStages->at(0))->moveSteps(amSteps);
  (m_LinearStages->at(1))->moveSteps(amSteps);
}

double StageFrame::getCurrentDistance(void){
  return(std::abs(755906 /*max. position*/ - (m_LinearStagesMessageHandlers.at(0))->getCurrentPosition()) +
         std::abs(755906 /*max. position*/ - (m_LinearStagesMessageHandlers.at(1))->getCurrentPosition()) +
         m_ZeroDistance);
}
