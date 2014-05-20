#include "onestepevent.h"

OneStepEvent::OneStepEvent(ExperimentType type,
                           DistanceOrStressForce distanceOrStressForce,
                           StressOrForce stressOrForce,
                           DistanceOrPercentage distanceOrPercentage,
                           StageFrame *stageframe,
                           ForceSensorMessageHandler *forcesensormessagehandler,
                           mpFXYVector *vector,
                           std::mutex *vectoraccessmutex,
                           MyFrame *myframe,
                           std::string path,
                           std::condition_variable *wait,
                           std::mutex *mutex,
                           bool *stagesstopped,
                           std::mutex *stagesstoppedmutex,
                           double velocity,
                           double holdtime1,
                           long upperlimit,
                           double holdtime2,
                           long lowerlimit,
                           DistanceOrPercentage holddistanceOrPercentage,
                           long holddistance,
                           int cycles,
                           BehaviorAfterStop behaviorAfterStop,
                           double area,
                           long currentdistance)
  : Experiment(type,
               distanceOrStressForce,
               stressOrForce,
               distanceOrPercentage,
               stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,
               Direction::Stop,
               0.3/*stress force threshold*/,
               0.01/*distance threshold*/,
               area,
               currentdistance),
    m_Velocity(velocity),
    m_HoldTime1(holdtime1),
    m_UpperLimit(upperlimit),
    m_HoldTime2(holdtime2),
    m_LowerLimit(lowerlimit),
    m_Cycles(cycles),
    m_HoldDistanceOrPercentage(holddistanceOrPercentage),
    m_HoldDistance(holddistance),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_ExperimentValues(new OneStepEventValues(type,
                                              distanceOrStressForce,
                                              stressOrForce,
                                              stageframe,
                                              forcesensormessagehandler,
                                              vector,
                                              vectoraccessmutex,
                                              myframe,
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
    // If distance based.
    if(DistanceOrStressForce::Distance == m_DistanceOrStressForce){
      // Make point if there is a hold time 1.
      if(0 < m_HoldTime1){
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::Distance, 0));
        timepoint++;
      }
      // Make upper limit point.
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::Distance, m_UpperLimit));
      timepoint++;
      // Make point if there is a hold time 2.
      if(0 < m_HoldTime2){
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::Distance, m_UpperLimit));
        timepoint++;
      }
      // Make lower limit point.
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::Distance, m_LowerLimit));

    } else if(DistanceOrStressForce::StressForce == m_DistanceOrStressForce){ // If stress/force based.
      if(0 < m_HoldTime1){
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::StressForce, 0));
        timepoint++;
      }
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::StressForce, m_UpperLimit));
      timepoint++;
      if(0 < m_HoldTime2){
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::StressForce, m_UpperLimit));
        timepoint++;
      }
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressForce::StressForce, m_LowerLimit));
      timepoint++;
    }
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
