#include <iostream>
#include <mathplot.h>
#include "ramp2failure.h"

/**
 * @brief Initializes all the needed variables.
 * @param type Type of the experiment.
 * @param stageframe Pointer to the stage frame object.
 * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param speedInMM Speed in mm/s.
 * @param area Value of the area.
 * @param preloaddistance Preload distance of the stage frame.
 */
Ramp2Failure::Ramp2Failure(std::shared_ptr<StageFrame> stageframe,
                           std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                           std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                           mpFXYVector *vector,
                           std::mutex *vectoraccessmutex,
                           MyFrame *myframe,
                           std::string path,
                           long maxforcelimit,
                           long minforcelimit,
                           long maxdistancelimit,
                           long mindistancelimit,

                           std::condition_variable *wait,
                           std::mutex *mutex,

                           ExperimentType type,
                           DistanceOrStressOrForce distanceOrStressOrForce,
                           long gagelength,
                           long zerodistance,
                           long currentdistance,
                           double area,

                           BehaviorAfterFailure behavior,
                           double speedInMM,
                           double dropbeforestop,
                           long preloaddistance,
                           long distanceafterfailure)
  : Experiment(stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,
               maxforcelimit,
               minforcelimit,
               maxdistancelimit,
               mindistancelimit,

               type,
               distanceOrStressOrForce,
               Direction::Stop,
               gagelength,
               zerodistance,
               currentdistance,
               area,
               300/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_StageFrame(stageframe),
    m_LinearStageMessageHanders(linearstagemessagehandlers),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_BehaviorAfterFailure(behavior),
    m_CurrentState(State::stopState),
    m_SpeedInMm(speedInMM),
    m_DropBeforeStop(dropbeforestop),
    m_Area(area),
    m_GageLength(preloaddistance),
    m_DistanceAfterFailure(distanceafterfailure),
    m_MaxForce(0)
{
  m_CurrentForce = 0;
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

Ramp2Failure::~Ramp2Failure(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void Ramp2Failure::getPreview(std::vector<PreviewValue> &previewvalue){

}

/**
 * @brief FSM of the Ramp2Failure experiment
 * @param event Occuring event
 */
void Ramp2Failure::process(Event e){
  switch(m_CurrentState){
    case State::stopState:
      if(Event::evStart == e){
        m_StageFrame->setSpeed(m_SpeedInMm);
        m_CurrentState = State::runState;
        //m_ExperimentValues->setStartPoint();
        m_StageFrame->moveBackward(m_SpeedInMm);
      }
      break;

    case runState:
      if(Event::evStop == e){
            m_CurrentState = State::stopState;
            m_CurrentDirection = Direction::Stop;
            m_StageFrame->stop();
            std::lock_guard<std::mutex> lck(*m_WaitMutex);
            m_Wait->notify_all();
      }if(Event::evForceUpdate == e){
        std::lock_guard<std::mutex> lck{m_ForceMutex};
        if(std::abs(m_CurrentForce) < ((m_DropBeforeStop / 100.0) * std::abs(m_MaxForce))){
          //std::cout << "std::abs(m_CurrentForce) < ((m_DropBeforeStop / 100.0) * std::abs(m_MaxForce) " << std::abs(m_CurrentForce) << " " << ((m_DropBeforeStop / 100.0) * std::abs(m_MaxForce)) << std::endl;
          //std::cout << "R2F Percent reached, m_BehaviorAfterFailure: " << static_cast<int>(m_BehaviorAfterFailure) << std::endl;
          switch(m_BehaviorAfterFailure){
            case BehaviorAfterFailure::PreloadPos:
              m_CurrentState = State::goBackState;
              m_StageFrame->gotoStepsDistance(m_GageLength);
              //std::cout << "R2F go back." << std::endl;
              break;

            case BehaviorAfterFailure::Stop:
              {
                m_CurrentState = State::stopState;
                m_StageFrame->stop();
                std::cout << "Stop Ramp2Failure in runState." << std::endl;
                m_CurrentDirection = Direction::Stop;
                std::lock_guard<std::mutex> lck(*m_WaitMutex);
                m_Wait->notify_all();
              }
              break;

            case BehaviorAfterFailure::GoTo:
              m_CurrentState = State::goBackState;
              m_StageFrame->gotoStepsDistance(m_DistanceAfterFailure);
              std::cout << "R2F go to distance after failure." << std::endl;
              break;
          }
        }
      }

      break;

    case goBackState:
      if(Event::evDistanceUpdate == e){
        switch(m_BehaviorAfterFailure){
          case BehaviorAfterFailure::PreloadPos:
            /*
            if((m_CurrentDistance - m_PreloadDistance) > m_DistanceThreshold){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_SpeedInMm);
            }else if((m_PreloadDistance - m_CurrentDistance) > m_DistanceThreshold){
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMm);
            }*/
            if(std::abs(m_GageLength - m_CurrentDistance) < m_DistanceThreshold){
              std::cout << "Stop Ramp2Failure in goBackState." << std::endl;
              m_CurrentState = stopState;
              m_CurrentDirection = Direction::Stop;
              std::lock_guard<std::mutex> lck(*m_WaitMutex);
              m_Wait->notify_all();
            }
            break;

          case BehaviorAfterFailure::GoTo:
            if(std::abs(m_DistanceAfterFailure - m_CurrentDistance) < m_DistanceThreshold){
              std::cout << "Stop Ramp2Failure in goBackState." << std::endl;
              m_CurrentState = stopState;
              m_CurrentDirection = Direction::Stop;
              std::lock_guard<std::mutex> lck(*m_WaitMutex);
              m_Wait->notify_all();
            }
            break;
        }
      }
      break;
  }
}

/**
 * @brief Sets the distance after failure according to the percentage of the preload.
 * @param percent Percent of preload.
 */
void Ramp2Failure::setPercentPreload(double percent){
  m_DistanceAfterFailure = (percent / 100) * m_GageLength;
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Ramp2Failure::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      {
        std::lock_guard<std::mutex> lck{m_ForceMutex};
        m_CurrentForce = measurementValue.value;
        if(std::abs(measurementValue.value) > std::abs(m_MaxForce)){
          m_MaxForce = measurementValue.value;
        }
      }
      process(Event::evForceUpdate);
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      process(Event::evDistanceUpdate);
      break;
  }

}
