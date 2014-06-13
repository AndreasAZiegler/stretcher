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
                  mpFXYVector *maxforcelimitvector,
                  mpFXYVector *minforcelimitvector,
                  mpFXYVector *maxdistancelimitvector,
                  mpFXYVector *mindistancelimitvector,
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

    /**
     * @brief Returns the experiment settings in a short form, usable for the experiment name.
     * @return The experiment settings in a short form.
     */
    virtual std::string experimentSettingsForName(void);

  private:
    double m_StressForceLimit;
    double m_SpeedInMm;
};

#endif // PRELOADVALUES_H
