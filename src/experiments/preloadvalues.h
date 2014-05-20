#ifndef PRELOADVALUES_H
#define PRELOADVALUES_H

// Includes
#include "experimentvalues.h"

class PreloadValues : public ExperimentValues
{
  public:
    PreloadValues(ExperimentType experimentType,
                  StressOrForce stressOrForce,
                  StageFrame *stageframe,
                  ForceSensorMessageHandler *forcesensormessagehandler,
                  mpFXYVector *vector,
                  std::mutex *vectoraccessmutex,
                  MyFrame *myframe,
                  double area,
                  double stressForceLimit,
                  double speedInMm);

    //PreloadValues(const PreloadValues &preloadvalues);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

  private:
    double m_StressForceLimit;
    double m_SpeedInMm;
};

#endif // PRELOADVALUES_H
