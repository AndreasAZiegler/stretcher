#ifndef PAUSERESUMEVALUES_H
#define PAUSERESUMEVALUES_H

// Includes
#include "experimentvalues.h"

class PauseResumeValues : public ExperimentValues
{
  public:
    PauseResumeValues(std::shared_ptr<StageFrame> stageframe,
                      std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                      mpFXYVector *vector,
                      std::mutex *vectoraccessmutex,
                      mpFXYVector *maxforcelimitvector,
                      mpFXYVector *minforcelimitvector,
                      mpFXYVector *maxdistancelimitvector,
                      mpFXYVector *mindistancelimitvector,
                      MyFrame *myframe,
                      ExperimentType experimentType,
                      DistanceOrStressOrForce distanceOrStressOrForce,
                      double area);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

};

#endif // PAUSERESUMEVALUES_H
