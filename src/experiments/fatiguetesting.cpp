#include <iostream>
#include <thread>
#include <mathplot.h>
#include "fatiguetesting.h"

/**
 * @brief Initializes all the needed variables.
 * @param type Type of the experiment.
 * @param stressOrForce Indicates if the experiment is stress or force based.
 * @param stageframe Pointer to the stage frame object.
 * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param cycles The number of cycles.
 * @param totaltime The total time for the experiment.
 * @param amplitude The amplitude in mm or %preload distance.
 * @param resttime The rest time after every cycle.
 * @param frequency The frequency in [cycles/sec].
 * @param preloaddistance The preload distance.
 * @param currentdistance The current distance.
 */
FatigueTesting::FatigueTesting(Experiment::ExperimentType type,
                               StressOrForce stressOrForce,
                               StageFrame *stageframe,
                               std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                               ForceSensorMessageHandler *forcesensormessagehandler,
                               mpFXYVector *vector,
                               std::mutex *vectoraccessmutex,
                               MyFrame *myframe,
                               std::condition_variable *wait,
                               std::mutex *mutex,
                               int cycles, double totaltime, long amplitude, double resttime, double frequency, double area, long preloaddistance, long currentdistance)
  : Experiment(type,
               stressOrForce,
               stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               Experiment::Direction::Stop,
               300/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/,
               area,
               currentdistance),
    m_StageFrame(stageframe),
    m_LinearStageMessageHanders(linearstagemessagehandlers),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_Cycles(cycles),
    m_TotalTime(totaltime),
    m_AmplitudeInDistance(amplitude + preloaddistance),
    m_RestTime(resttime),
    m_Frequency(frequency),
    m_PreloadDistance(preloaddistance),
    m_CurrentCycle(0),
    m_SpeedInMm(frequency * totaltime*60 * (amplitude*0.00009921875/*mm per micro step*/) / (totaltime*60 - resttime)),
    m_DecreaseSpeedFlag(false)
{
  m_CurrentDirection = Direction::Stop;
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
FatigueTesting::~FatigueTesting(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief FSM of the fatigue experiment
 * @param event Occuring event
 */
void FatigueTesting::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == event){
        m_StageFrame->setSpeed(m_SpeedInMm);
        m_CurrentState = runState;

        // If force/stress based
        if((m_CurrentDistance - m_AmplitudeInDistance) > m_DistanceThreshold){
          //std::cout << "m_CurrentDistance - m_AmplitudeInDistance: " << m_CurrentDistance - m_AmplitudeInDistance << std::endl;
          m_CurrentDirection = Direction::Forwards;
          m_StageFrame->moveForward(m_SpeedInMm);
          //std::cout << "Fatigue testing moveForward" << std::endl;
        }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
          //std::cout << "m_AmplitudeInDistance - m_CurrentDistance : " << m_AmplitudeInDistance - m_CurrentDistance << std::endl;
          m_CurrentDirection = Direction::Backwards;
          m_StageFrame->moveBackward(m_SpeedInMm);
          //std::cout << "Fatigue testing moveBackward" << std::endl;
        }
      }

      break;

    case runState:

      if(evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_one();
      }
      if(evUpdate == event){
        //std::cout << "m_CurrentDistance - m_AmplitudeInDistance: " << m_CurrentDistance << " - " << m_AmplitudeInDistance << std::endl;
        // Reduce speed to a tenth if stages are close to the turn point.
        if((m_CurrentDistance - m_AmplitudeInDistance) < (200 * m_DistanceThreshold)){
          if(false == m_DecreaseSpeedFlag){
            m_DecreaseSpeedFlag = true;
            m_StageFrame->setSpeed(m_SpeedInMm/10);
          }
        }
        // Reduce speed to a tenth if stages are close to the turn point.
        else if((m_AmplitudeInDistance - m_CurrentDistance) < (200 * m_DistanceThreshold)){
          if(false == m_DecreaseSpeedFlag){
            m_DecreaseSpeedFlag = true;
            m_StageFrame->setSpeed(m_SpeedInMm/10);
          }
        }
        if((m_CurrentDistance - m_AmplitudeInDistance) > m_DistanceThreshold){
          if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward();
            std::cout << "Fatigue testing moveForward" << std::endl;
          }
        }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
          if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward();
            //std::cout << "Fatigue testing moveBackward" << std::endl;
          }
        }else{
          m_CurrentState = State::goBackState;
          m_StageFrame->setSpeed(m_SpeedInMm);
          m_StageFrame->gotoStepsDistance(m_PreloadDistance);
          //std::cout << "Fatigue testing go back to preload." << std::endl;
        }
      }
      break;

    case goBackState:
      if(evUpdate == event){
        //std::cout << "m_CurrentDistance - m_PreloadDistance > m_DistanceThreshold: " << m_CurrentDistance - m_PreloadDistance << "   " << m_DistanceThreshold << std::endl;
        //std::cout << "m_CurrentDistance: " << m_CurrentDistance << " m_PreloadDistance: " << m_PreloadDistance << std::endl;
        if(std::abs(m_PreloadDistance - m_CurrentDistance) < m_DistanceThreshold){
          //std::cout << "diff < m_DistanceThreshold m_Cycles: " << m_Cycles - 1 << " m_CurrentCycle: " << m_CurrentCycle << std::endl;
          if((m_Cycles - 1) <= m_CurrentCycle){
            std::cout << "Stop fatigue testing." << std::endl;
            m_CurrentState = stopState;
            m_CurrentCycle = 0;
            m_CurrentDirection = Direction::Stop;
            //m_StageFrame->stop();
            std::lock_guard<std::mutex> lck(*m_WaitMutex);
            m_Wait->notify_all();
          }else{
            //std::cout << "Another cycle." << std::endl;
            m_CurrentCycle++;

            // Check if a rest time is set.
            if(0 < m_RestTime){
              m_CurrentState = State::pauseState;
              m_StageFrame->stop();

              std::cout << "Fatigue testing sleep for " << m_RestTime * 1000 << " ms" << std::endl;
              std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_RestTime * 1000)));
              std::cout << "Fatigue testing sleeping over." << std::endl;
            }

            m_CurrentState = runState;

            if((m_CurrentDistance - m_AmplitudeInDistance) > m_DistanceThreshold){
              m_CurrentDirection = Direction::Forwards;
              m_DecreaseSpeedFlag = false;
              m_StageFrame->moveForward(m_SpeedInMm);
            }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
              m_DecreaseSpeedFlag = false;
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMm);
            }
          }
        }
      }
      break;
  }

}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void FatigueTesting::updateValues(long value, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = value;
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = value;
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  process(Event::evUpdate);
}
