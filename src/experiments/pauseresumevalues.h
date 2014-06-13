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
                      mpFXYVector *maxlimitvector,
                      mpFXYVector *minlimitvector,
                      MyFrame *myframe,
                      ExperimentType experimentType,
                      DistanceOrStressOrForce distanceOrStressOrForce,
                      double area);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

    /**
     * @brief Returns the experiment settings in a short form, usable for the experiment name.
     * @return The experiment settings in a short form.
     */
    virtual std::string experimentSettingsForName(void);

};

#endif // PAUSERESUMEVALUES_H
