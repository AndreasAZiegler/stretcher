#ifndef PRELOADVALUES_H
#define PRELOADVALUES_H

// Includes
#include "experimentvalues.h"

class PreloadValues : public ExperimentValues
{
  public:
    PreloadValues(std::shared_ptr<StageFrame> stageframe,
                  std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                  mpFXYVector *forceStressDistanceGraph,
                  mpFXYVector *forceStressDisplacementGraph,
                  std::mutex *vectoraccessmutex,
                  mpFXYVector *maxlimitvector,
                  mpFXYVector *minlimitvector,
                  MyFrame *myframe,

                  ExperimentType experimentType,
                  DistanceOrStressOrForce distanceOrStressOrForce,
                  double area,
                  long gagelength,

                  double stressForceLimit,
                  double speedInMm);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(PreloadParameters parameters);

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
