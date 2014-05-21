#ifndef PRELOADVALUES_H
#define PRELOADVALUES_H

// Includes
#include "experimentvalues.h"

class PreloadValues : public ExperimentValues
{
  public:
    PreloadValues(std::shared_ptr<StageFrame> stageframe,
                  std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                  mpFXYVector *vector,
                  std::mutex *vectoraccessmutex,
                  MyFrame *myframe,

                  ExperimentType experimentType,
                  DistanceOrStressOrForce distanceOrStressOrForce,
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
