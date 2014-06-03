#ifndef PAUSEVALUES_H
#define PAUSEVALUES_H

// Includes
#include "experimentvalues.h"

class PauseValues : public ExperimentValues
{
  public:
    PauseValues(std::shared_ptr<StageFrame> stageframe,
                std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                mpFXYVector *vector,
                std::mutex *vectoraccessmutex,
                mpFXYVector *maxlimitvector,
                mpFXYVector *minlimitvector,
                MyFrame *myframe,
                ExperimentType experimentType,
                DistanceOrStressOrForce distanceOrStressOrForce,
                double area,
                int pausetime = 0);

    /**
     * @brief Sets pause time.
     * @param pausetime Pause time in sec.
     */
    void setPauseTime(int pausetime){
      m_PauseTime = pausetime;
    }

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

  private:
    int m_PauseTime;																				/**< The amount of seconds, the pause shoudl be. */
};

#endif // PAUSEVALUES_H
