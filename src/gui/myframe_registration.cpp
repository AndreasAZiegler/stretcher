/**
 * @file myframe_registration.cpp
 * @brief The main frame registration part.
 * @author Andreas Ziegler
 */

// Includes
#include "myframe.h"

/**
 * @brief Register the liner stages and the stage frame, registers the update method at the stage frame and registers the stop wait conditional variable at the stage frame.
 * @param linearstage Pointer to the vector containing the linear stages.
 * @param stageframe Pointer to the stage frame ojbect.
 */
void MyFrame::registerLinearStage(std::vector<std::shared_ptr<LinearStage>> &linearstage, std::shared_ptr<StageFrame> &stageframe){
  m_LinearStages.push_back(linearstage.at(0));
  m_LinearStages.push_back(linearstage.at(1));
  m_StageFrame = stageframe;

  // Registers update methods at stage frame.
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  // Registers stop wait conditional variable and mutex.
  m_StageFrame->registerStagesStopped(&m_StagesStoppedFlag, &m_StagesStoppedMutex);

  // Reset limit
  m_StageFrame->setMaxDistanceLimitMM(153);
  m_StageFrame->setMinDistanceLimitMM(0);
}

/**
 * @brief Registers the message handlers of the linear stages, registers the stage frame at the linear stage message handlers.
 * @param linearstagesmessagehandlers Pointer to the vector containing the message handlers of the linear stages.
 */
void MyFrame::registerLinearStageMessageHandlers(std::vector<std::shared_ptr<LinearStageMessageHandler>> &linearstagesmessagehandlers){
  m_LinearStagesMessageHandlers.push_back(linearstagesmessagehandlers.at(0));
  m_LinearStagesMessageHandlers.push_back(linearstagesmessagehandlers.at(1));

  // Registers stage frame
  (m_LinearStagesMessageHandlers.at(0))->registerStageFrame(m_StageFrame);
  (m_LinearStagesMessageHandlers.at(1))->registerStageFrame(m_StageFrame);
}

/**
 * @brief Register the force sensor and register update method at the force sensor message handler.
 * @param forcesensor Pointer to the force sensor.
 */
void MyFrame::registerForceSensor(std::shared_ptr<ForceSensor> forcesensor){
  m_ForceSensor = forcesensor;

  // Registers update method at forcesensormessagehandler.
  m_ForceSensorMessageHandler = m_ForceSensor->getMessageHandler();
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}
