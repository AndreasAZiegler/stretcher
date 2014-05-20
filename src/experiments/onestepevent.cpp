#include "onestepevent.h"

OneStepEvent::OneStepEvent(StageFrame *stageframe,
                           ForceSensorMessageHandler *forcesensormessagehandler,
                           mpFXYVector *vector,
                           std::mutex *vectoraccessmutex,
                           MyFrame *myframe,
                           std::string path,

                           std::condition_variable *wait,
                           std::mutex *mutex,
                           bool *stagesstopped,
                           std::mutex *stagesstoppedmutex,

                           ExperimentType type,
                           DistanceOrStressOrForce distanceOrStressForce,
                           long gagelength,
                           long currentdistance,
                           double area,

                           DistanceOrPercentage velocityDistanceOrPercentage,
                           double velocity,
                           double holdtime1,
                           long upperlimit,
                           double holdtime2,
                           long lowerlimit,
                           DistanceOrPercentage holdDistanceOrPercentage,
                           long holddistance,
                           int cycles,
                           BehaviorAfterStop behaviorAfterStop)
  : Experiment(stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,

               type,
               distanceOrStressForce,
               Direction::Stop,
               gagelength,
               currentdistance,
               area,
               0.3/*stress force threshold*/,
               0.01/*distance threshold*/),
    m_VelocityDistanceOrPercentage(velocityDistanceOrPercentage),
    m_Velocity(velocity),
    m_HoldTime1(holdtime1),
    m_UpperLimit(upperlimit),
    m_HoldTime2(holdtime2),
    m_LowerLimit(lowerlimit),
    m_Cycles(cycles),
    m_HoldDistanceOrPercentage(holdDistanceOrPercentage),
    m_HoldDistance(holddistance),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_ExperimentValues(new OneStepEventValues(stageframe,
                                              forcesensormessagehandler,
                                              vector,
                                              vectoraccessmutex,
                                              myframe,
                                              path,

                                              type,
                                              distanceOrStressForce,
                                              area,

                                              velocity,
                                              holdtime1,
                                              upperlimit,
                                              holdtime2,
                                              lowerlimit,
                                              holddistance,
                                              cycles,
                                              behaviorAfterStop))
{
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void OneStepEvent::setPreloadDistance(long preloaddistance){
  m_GageLength = preloaddistance;

  if(DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_Velocity / 100.0) * m_GageLength;
    m_ExperimentValues->setVelocity(m_Velocity);
  }
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void OneStepEvent::getPreview(std::vector<Experiment::PreviewValue>& previewvalue){
  // Calculate time point.
  int timepoint;
  if(0 == previewvalue.size()){
   timepoint = 1;
  } else{
   timepoint =  previewvalue.back().getTimepoint() + 1;
  }

  for(int i = 0; i < m_Cycles; ++i){
    // Make point if there is a hold time 1.
    if(0 < m_HoldTime1){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, 0));
      timepoint++;
    }
    // Make upper limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
    timepoint++;
    // Make point if there is a hold time 2.
    if(0 < m_HoldTime2){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
      timepoint++;
    }
    // Make lower limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_LowerLimit));
  }
  timepoint++;

  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::GoToL0:
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));

  }
}

/**
* @brief FSM of the experiment
* @param e Occuring event
*/
void OneStepEvent::process(Event event){

}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void OneStepEvent::updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type){

}

/**
* @brief Do all the required thing to stop the experiment during process.
*/
void OneStepEvent::resetExperiment(void){

}
