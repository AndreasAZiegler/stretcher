#include <iostream>
#include "experimentvalues.h"

/**
 * @brief Initializes all the needed variables.
 * @param stressOrForce Indicates if the experiment is stress or force based.
 * @param stageframe Pointer to the stage frame object.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param graph Pointer to the graph object.
 * @param diameter The diameter of the sample.
 */
ExperimentValues::ExperimentValues(StressOrForce stressOrForce,
                                   StageFrame *stageframe,
                                   ForceSensorMessageHandler *forcesensormessagehandler,
                                   mpWindow *graph,
                                   double diameter)
  : m_StressOrForce(stressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Graph(graph),
    m_Diameter(diameter),
    m_VectorLayer{new mpFXYVector(_("Vector"))}
{
}

/**
 * @brief Registers the update methods to receive the measurement values.
 */
void ExperimentValues::startMeasurement(void){
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  m_Graph->AddLayer(m_VectorLayer);
}

/**
 * @brief Unregister the update method.
 */
void ExperimentValues::stopMeasurement(void){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Removes the current graph.
 */
void ExperimentValues::removeGraph(void){
  m_Graph->DelLayer(m_VectorLayer);
}

/**
 * @brief Destructor
 */
ExperimentValues::~ExperimentValues(){
  std::cout << "ExperimentValues destructor finished." << std::endl;
}

/**
 * @brief Method which will be calles by the message handlers to update the values.
 * @param value Position of linear stage 1 or 2 or the force.
 * @param type Type of value.
 */
void ExperimentValues::updateValues(long value, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      if(StressOrForce::Stress == m_StressOrForce){
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back((value / 10000.0) / m_Diameter);
        }
      }else{
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back(value / 10000.0);
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_DistanceValues.push_back(value * 0.00009921875/*mm per micro step*/);
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }
  //std::cout << "m_StressForceValue has a size of: " << m_StressForceValues.size() << " m_DistanceValues has a size of: " << m_DistanceValues.size() << std::endl;

  {
    std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
    if(m_StressForceValues.size() == m_DistanceValues.size()){
      m_VectorLayer->SetData(m_DistanceValues, m_StressForceValues);
      m_VectorLayer->SetContinuity(true);
      wxPen vectorpen(*wxBLUE, 2, wxSOLID);
      m_VectorLayer->SetPen(vectorpen);
      m_VectorLayer->SetDrawOutsideMargins(false);

      m_Graph->Fit();
      //std::cout << "ExperimentVaues graph fitted." << std::endl;
    }
  }
}