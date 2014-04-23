#include <iostream>
#include <chrono>
#include <thread>
#include <mathplot.h>
#include "relaxation.h"


/**
 * @brief Initializes all the needed variables.
 * @param type Type of the experiment.
 * @param stressOrForce Indicates if the experiment is stress or force based.
 * @param currentdistance The current distance of the stage frame.
 * @param stageframe Pointer to the stage frame object.
 * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param distance Distance the stages should move per step.
 * @param pause The pause duration in sec.
 * @param steps The amount of steps.
 * @param preloaddistance Preload distance of the stage frame.
 */
Relaxation::Relaxation(Experiment::ExperimentType type,
                       Experiment::StressOrForce stressOrForce,
                       long currentdistance,
                       StageFrame *stageframe,
                       std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                       ForceSensorMessageHandler *forcesensormessagehandler,
                       mpWindow *graph,
                       std::condition_variable *wait,
                       std::mutex *mutex,
                       long distance, double pause, int steps, double area, long preloaddistance)
  : Experiment(type, stressOrForce, stageframe, forcesensormessagehandler, graph, Direction::Stop, 300/*stress force threshold*/, 0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/, area, currentdistance),
    m_StageFrame(stageframe),
    m_LinearStageMessageHanders(linearstagemessagehandlers),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_Distance(distance),
    m_DistanceLimit(distance),
    m_Pause(pause),
    m_Steps(steps),
    m_CurrentStep(0),
    m_PreloadDistance(preloaddistance)
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
Relaxation::~Relaxation(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief FSM of the conditioning experiment
 * @param event Occuring event
 */
void Relaxation::process(Experiment::Event e){
  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == e){
        //m_StageFrame->setSpeed();
        m_CurrentState = State::runState;

        m_StageFrame->gotoStepsDistance(m_PreloadDistance + m_DistanceLimit);
        std::cout << "Relaxation started, mPrlealoadDistance: " << m_PreloadDistance << " m_DistanceLimit: " << m_DistanceLimit << std::endl;

      }
      break;

    case runState:

      if(Experiment::Event::evStop == e){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentStep = 0;
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_one();
      }
      if(Experiment::Event::evUpdate == e){
        //std::cout << "std::abs(m_DistanceLimit - m_CurrentDistance) < m_DistanceThreshold " << std::abs(m_DistanceLimit - m_CurrentDistance) << " " << m_DistanceThreshold << std::endl;
        if(std::abs((m_PreloadDistance + m_DistanceLimit) - m_CurrentDistance) < m_DistanceThreshold){
          if((m_Steps - 1) <= m_CurrentStep){
            std::cout << "Stop relaxation." << std::endl;
            m_CurrentState = stopState;
            m_CurrentDirection = Direction::Stop;
            m_CurrentStep = 0;
            //m_StageFrame->stop();
            std::lock_guard<std::mutex> lck(*m_WaitMutex);
            m_Wait->notify_all();
          }else{
            m_CurrentStep++;

            m_CurrentState = State::pauseState;
            std::cout << "Relaxation sleep for " << m_Pause * 1000 << " ms" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_Pause * 1000)));
            std::cout << "Relaxation sleeping over." << std::endl;

            m_CurrentState = State::runState;
            m_DistanceLimit += m_Distance;
            m_StageFrame->gotoStepsDistance(m_PreloadDistance + m_DistanceLimit);
            std::cout << "Relaxation: Go to next step." << std::endl;
          }
        }
      }
      break;

    case pauseState:
      if(Experiment::Event::evStop == e){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = State::stopState;
        m_CurrentDirection = Direction::Stop;
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_one();
      }

      /*
        if((m_CurrentDistance - m_DistanceLimit) > m_DistanceThreshold){
          std::cout << "m_CurrentDistance - m_DistanceLimit: " << m_CurrentDistance - m_DistanceLimit << std::endl;
          m_CurrentDirection = Direction::Forwards;
          m_StageFrame->moveForward(m_SpeedInMm);
          std::cout << "Conditioning moveForward" << std::endl;
        }else if((m_DistanceLimit - m_CurrentDistance) > m_DistanceThreshold){
          std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_DistanceLimit - m_CurrentDistance << std::endl;
          m_CurrentDirection = Direction::Backwards;
          m_StageFrame->moveBackward(m_SpeedInMm);
          std::cout << "Conditioning moveBackward" << std::endl;
        }
        */

      break;

    case goBackState:

      break;
  }
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Relaxation::updateValues(long value, UpdatedValuesReceiver::ValueType type){
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
