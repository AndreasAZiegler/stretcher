#include <iostream>
#include <thread>
#include "fatiguetesting.h"

FatigueTesting::FatigueTesting(Experiment::ExperimentType type,
                               StressOrForce stressOrForce,
                               StageFrame *stageframe,
                               std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                               ForceSensorMessageHandler *forcesensormessagehandler,
                               std::condition_variable *wait,
                               std::mutex *mutex,
                               int cycles, double totaltime, long amplitude, double resttime, double frequency, long preloaddistance, long currentdistance)
  : Experiment(type, stressOrForce, Experiment::Direction::Stop, 300/*stress force threshold*/, 0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/, currentdistance),
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
    m_SpeedInMm(frequency * totaltime*60 * (amplitude*0.00009921875/*mm per micro step*/) / (totaltime*60 - resttime))
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
          std::cout << "m_CurrentDistance - m_AmplitudeInDistance: " << m_CurrentDistance - m_AmplitudeInDistance << std::endl;
          m_CurrentDirection = Direction::Forwards;
          m_StageFrame->moveForward(m_SpeedInMm);
          std::cout << "Fatigue testing moveForward" << std::endl;
        }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
          std::cout << "m_AmplitudeInDistance - m_CurrentDistance : " << m_AmplitudeInDistance - m_CurrentDistance << std::endl;
          m_CurrentDirection = Direction::Backwards;
          m_StageFrame->moveBackward(m_SpeedInMm);
          std::cout << "Fatigue testing moveBackward" << std::endl;
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
        std::cout << "m_CurrentDistance - m_AmplitudeInDistance: " << m_CurrentDistance << " - " << m_AmplitudeInDistance << std::endl;
        if((m_CurrentDistance - m_AmplitudeInDistance) > m_DistanceThreshold){
          if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMm);
            std::cout << "Fatigue testing moveForward" << std::endl;
          }
        }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
          if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMm);
            std::cout << "Fatigue testing moveBackward" << std::endl;
          }
        }else{
          m_CurrentState = State::goBackState;
          m_StageFrame->gotoStepsDistance(m_PreloadDistance);
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
              m_StageFrame->moveForward(m_SpeedInMm);
            }else if((m_AmplitudeInDistance - m_CurrentDistance) > m_DistanceThreshold){
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