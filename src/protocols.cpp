/**
 * @file main.cpp
 * @brief The protocol.
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <fstream>
#include <chrono>
#include <wx/log.h>
#include "pugixml/pugixml.hpp"
#include <wx/msgdlg.h>
#include "../gui/myframe.h"
#include "../gui/myexportdialog.h"
#include "protocols.h"
#include "experiments/preload.h"
#include "experiments/onestepevent.h"
#include "experiments/continuousevent.h"
#include "experiments/pause.h"
#include "experiments/pauseresume.h"

// An deleter which doesn't do anything, required for passing shared_ptr.
void do_nothing_deleter(std::vector<double> *){return;}

/**
 * @brief Initializes all the needed variables.
 * @param listbox Pointer to the wxListBox object.
 * @param myframe Pointer to the main frame object.
 * @param stageframe Pointer to the stage frame object.
 * @param forcesensormessagehandler Pointer to the force sensor message hander object.
 * @param vectoraccessmutex Pointer to the mutex to protect the vectors.
 * @param gagelength The gage length.
 * @param mountinglength The mounting length.
 * @param maxposdistance The distance at maximal positions.
 * @param currentdistance The current distance.
 * @param stagesstoppedflag Pointer to the flag that indicates that the stages stopped
 * @param stagesstoppedmutex Pointer to the mutex to protect stagesstoppedflag.
 * @param waitmutex Pointer to the mutex for the condition variable wait.
 * @param wait Pointer to the condition variable to wait for finished experiments.
 * @param preloaddoneflag Pointer to the flag that indicates that preloading is done.
 * @param preloaddonemutex Pointer to the mutex to protect preloaddoneflag.
 * @param loopflag Flag to indicate that the protocol should be looped.
 * @param area Cross section area.
 * @param maxdistance Maximal distance.
 * @param mindistance Minimal distance.
 * @param maxforce Maximal force.
 * @param minforce Minimal force.
 * @param forceStressDistanceGraph Pointer to the force/stress distance graph.
 * @param forceStressDisplacementGraph Pointer to the force/stress displacement graph.
 * @param stressForceGraph Pointer to the stress/force graph.
 * @param distanceGraph Pointer to the distance graph.
 * @param maxStressForceLimitGraph Pointer to the maximal stress/force limit graph.
 * @param minStressForceLimitGraph Pointer to the minimal stress/force limit graph.
 * @param maxDistanceLimitGraph Pointer to the maximal distance limit graph.
 * @param minDistanceLimitGraph Pointer to the minimal distance limit graph.
 * @param path Path.
 */
Protocols::Protocols(wxListBox *listbox,
                     MyFrame *myframe,
                     std::shared_ptr<StageFrame> stageframe,
                     std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                     std::mutex *vectoraccessmutex,
                     long gagelength,
                     long mountinglength,
                     long maxposdistance,
                     long currentdistance,
                     bool *stagesstoppedflag,
                     std::mutex *stagesstoppedmutex,
                     std::mutex *waitmutex,
                     std::condition_variable *wait,
                     bool *preloaddoneflag,
                     std::mutex *preloaddonemutex,
                     bool loopflag,
                     double area,
                     long maxdistance,
                     long mindistance,
                     long maxforce,
                     long minforce,
                     long forcestresssensitivity,
                     long distancesensitivity,
                     mpFXYVector *forceStressDistanceGraph,
                     mpFXYVector *forceStressDisplacementGraph,
                     mpFXYVector *stressForceGraph,
                     mpFXYVector *distanceGraph,
                     mpFXYVector *maxStressForceLimitGraph,
                     mpFXYVector *minStressForceLimitGraph,
                     mpFXYVector *maxDistanceLimitGraph,
                     mpFXYVector *minDistanceLimitGraph,
                     std::string path)
  : m_ListBox(listbox),
    m_MyFrame(myframe),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_VectorLayerMutex(vectoraccessmutex),
    m_GageLength(gagelength),
    m_MountingLength(mountinglength),
    m_MaxPosDistance(maxposdistance),
    m_CurrentDistance(currentdistance),
    m_StagesStoppedFlag(stagesstoppedflag),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_WaitMutex(waitmutex),
    m_Wait(wait),
    m_LoopProtocolFlag(loopflag),
    m_StopProtocolFlag(false),
    m_PreloadDoneFlag(preloaddoneflag),
    m_PreloadDoneMutex(preloaddonemutex),
    m_Area(area),
    m_MaxDistanceLimit(maxdistance),
    m_MinDistanceLimit(mindistance),
    m_MaxForceLimit(maxforce),
    m_MinForceLimit(minforce),
    m_ForceStressSensitivity(forcestresssensitivity),
    m_DistanceSensitivity(distancesensitivity),
    m_ForceStressDistanceGraph(forceStressDistanceGraph),
    m_ForceStressDisplacementGraph(forceStressDisplacementGraph),
    m_StressForcePreviewGraph(stressForceGraph),
    m_DistancePreviewGraph(distanceGraph),
    m_MaxStressForceLimitGraph(maxStressForceLimitGraph),
    m_MinStressForceLimitGraph(minStressForceLimitGraph),
    m_MaxDistanceLimitGraph(maxDistanceLimitGraph),
    m_MinDistanceLimitGraph(minDistanceLimitGraph),
    m_StoragePath(path),
    m_PreloadDistance(0),
    m_ExperimentRunningFlag(false),
    m_MeasurementValuesRecordingFlag(false),
    m_CurrentExperimentNr(0),
    m_EditedExperiment(0)
{
}

/**
 * @brief Destructor
 */
Protocols::~Protocols(){
  /*
  m_Experiments.clear();
  m_ExperimentValues.clear();
  */
}

/**
 * @brief Load a protocol from a file.
 * @param path Path of the protocol file.
 * @param gagelength Gage length
 * @param mountinglength Mounting length
 * @param maxposdistance Maximal position distance
 * @param currentdistance Current distance
 */
void Protocols::loadProtocol(std::string path, long gagelength, long mountinglength, long maxposdistance, long currentdistance){
  // Update lengths
  m_GageLength = gagelength;
  m_MountingLength = mountinglength;
  m_MaxPosDistance = maxposdistance;
  m_CurrentDistance = currentdistance;

  pugi::xml_document doc;

  // Open file.
  doc.load_file(path.c_str());

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;

  // Iterate through the experiments to load the settings.
  for(pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()){


    if(0 == strcmp("Preload", node.name())){ // Preload experiment.
      PreloadParameters parameters;

      // Load preload parmeters.
      parameters.distanceOrForceOrStress = static_cast<DistanceOrForceOrStress>(node.attribute("ForceOrStress").as_int());
      parameters.velocity = node.attribute("Velocity").as_double();
      parameters.stressForceLimit = node.attribute("ForceStressLimit").as_double();

      // Create preload experiment.
      ExperimentParameters experimentparameters;
      experimentparameters.stageframe = m_StageFrame;
      experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
      experimentparameters.myframe = m_MyFrame;
      experimentparameters.maxforcelimit = m_MaxForceLimit;
      experimentparameters.minforcelimit = m_MinForceLimit;
      experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
      experimentparameters.mindistancelimit = m_MinDistanceLimit;
      experimentparameters.type = ExperimentType::Preload;
      experimentparameters.distanceOrForceOrStress = parameters.distanceOrForceOrStress;
      experimentparameters.gagelength = m_GageLength;
      experimentparameters.mountinglength = m_MountingLength;
      experimentparameters.maxposdistance = m_MaxPosDistance;
      experimentparameters.currentdistance = m_CurrentDistance;
      experimentparameters.area = m_Area;
      std::unique_ptr<Experiment> experiment(new Preload(experimentparameters,

                                                         m_StoragePath,
                                                         m_ForceStressDistanceGraph,
                                                         m_ForceStressDisplacementGraph,
                                                         m_VectorLayerMutex,
                                                         m_MaxStressForceLimitGraph,
                                                         m_MinStressForceLimitGraph,
                                                         m_MaxDistanceLimitGraph,
                                                         m_MinDistanceLimitGraph,
                                                         m_ForceStressSensitivity,
                                                         m_DistanceSensitivity,

                                                         m_Wait,
                                                         m_WaitMutex,
                                                         m_StagesStoppedFlag,
                                                         m_StagesStoppedMutex,


                                                         parameters));

      // Add experiment.
      addExperiment(experiment);

    }else if(0 == strcmp("OneStepEvent", node.name())){ // One step event experiment.
      OneStepEventParameters parameters;

      // Load one step event parameters.
      parameters.distanceOrStressOrForce = static_cast<DistanceOrForceOrStress>(node.attribute("DistanceOrStressOrForce").as_int());
      parameters.velocityDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("VelocityDistanceOrPercentage").as_int());
      parameters.velocity = node.attribute("Velocity").as_double();
      parameters.delay = node.attribute("Delay").as_double();
      parameters.limitDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("LimitDistanceOrPercentage").as_int());
      parameters.limit = node.attribute("Limit").as_double();
      parameters.dwell = node.attribute("Dwell").as_double();
      parameters.cycles = node.attribute("Cycles").as_int();
      parameters.holdDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("HoldDistanceOrPercentage").as_int());
      parameters.holdDistance = node.attribute("HoldDistance").as_double();
      parameters.behaviorAfterStop = static_cast<BehaviorAfterStop>(node.append_attribute("BehaviorAfterStop").as_int());

      // Create one step event experiment.
      ExperimentParameters experimentparameters;
      experimentparameters.stageframe = m_StageFrame;
      experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
      experimentparameters.myframe = m_MyFrame;
      experimentparameters.maxforcelimit = m_MaxForceLimit;
      experimentparameters.minforcelimit = m_MinForceLimit;
      experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
      experimentparameters.mindistancelimit = m_MinDistanceLimit;
      experimentparameters.type = ExperimentType::OneStepEvent;
      experimentparameters.distanceOrForceOrStress = parameters.distanceOrStressOrForce;
      experimentparameters.gagelength = m_GageLength;
      experimentparameters.mountinglength = m_MountingLength;
      experimentparameters.maxposdistance = m_MaxPosDistance;
      experimentparameters.currentdistance = m_CurrentDistance;
      experimentparameters.area = m_Area;
      std::unique_ptr<Experiment> experiment(new OneStepEvent(experimentparameters,

                                                              m_StoragePath,
                                                              m_ForceStressDistanceGraph,
                                                              m_ForceStressDisplacementGraph,
                                                              m_VectorLayerMutex,
                                                              m_MaxStressForceLimitGraph,
                                                              m_MinStressForceLimitGraph,
                                                              m_MaxDistanceLimitGraph,
                                                              m_MinDistanceLimitGraph,
                                                              m_ForceStressSensitivity,
                                                              m_DistanceSensitivity,

                                                              m_Wait,
                                                              m_WaitMutex,

                                                              parameters));

      // Add experiment.
      addExperiment(experiment);

    }else if(0 == strcmp("ContinuousEvent", node.name())){ // Continuous event experiment.
      ContinuousEventParameters parameters;

      parameters.distanceOrStressOrForce = static_cast<DistanceOrForceOrStress>(node.attribute("DistanceOrStressOrForce").as_int());
      parameters.ramp2failure = node.attribute("Ramp2Failure").as_bool();
      parameters.velocityDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("VelocityDistanceOrPercentage").as_int());
      parameters.velocity = node.attribute("Velocity").as_double();
      parameters.holdtime = node.attribute("HoldTime").as_double();
      parameters.incrementDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("IncrementDistanceOrPercentage").as_int());
      parameters.increment = node.attribute("Increment").as_double();
      parameters.stepsOrMaxValue = static_cast<StepsOrMaxValue>(node.attribute("StepsOrMaxValue").as_int());
      parameters.maxValueDistanceOrPercentage = static_cast<DistanceOrPercentage>(node.attribute("MaxValueDistanceOrPercentage").as_int());
      parameters.maxvalue = node.attribute("MaxValue").as_double();
      parameters.steps = node.attribute("Steps").as_int();
      parameters.cycles = node.attribute("Cycles").as_int();
      parameters.behaviorAfterStop = static_cast<BehaviorAfterStop>(node.attribute("BehaviorAfterStop").as_int());
      parameters.holdForceStress = node.attribute("HoldForce").as_double();

      // Create continuous event experiment.
      ExperimentParameters experimentparameters;
      experimentparameters.stageframe = m_StageFrame;
      experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
      experimentparameters.myframe = m_MyFrame;
      experimentparameters.maxforcelimit = m_MaxForceLimit;
      experimentparameters.minforcelimit = m_MinForceLimit;
      experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
      experimentparameters.mindistancelimit = m_MinDistanceLimit;
      experimentparameters.type = ExperimentType::ContinuousEvent;
      experimentparameters.distanceOrForceOrStress = parameters.distanceOrStressOrForce;
      experimentparameters.gagelength = m_GageLength;
      experimentparameters.mountinglength = m_MountingLength;
      experimentparameters.maxposdistance = m_MaxPosDistance;
      experimentparameters.currentdistance = m_CurrentDistance;
      experimentparameters.area = m_Area;
      std::unique_ptr<Experiment> experiment(new ContinuousEvent(experimentparameters,

                                                                 m_StoragePath,
                                                                 m_ForceStressDistanceGraph,
                                                                 m_ForceStressDisplacementGraph,
                                                                 m_VectorLayerMutex,
                                                                 m_MaxStressForceLimitGraph,
                                                                 m_MinStressForceLimitGraph,
                                                                 m_MaxDistanceLimitGraph,
                                                                 m_MinDistanceLimitGraph,
                                                                 m_ForceStressSensitivity,
                                                                 m_DistanceSensitivity,

                                                                 m_Wait,
                                                                 m_WaitMutex,

                                                                 parameters));

      // Add experiment.
      addExperiment(experiment);

    }else if(0 == strcmp("Pause", node.name())){ // Pause experiment.
      double pausetime = node.attribute("PauseTime").as_double();
      DistanceOrForceOrStress distanceorstressorforce = static_cast<DistanceOrForceOrStress>(node.attribute("DistanceOrStressOrForce").as_int());

      if(((nullptr == maxlimitvector) || (nullptr == minlimitvector))){
        maxlimitvector = m_MaxStressForceLimitGraph;
        minlimitvector = m_MinStressForceLimitGraph;
      }

      // Create pause experiment.
      ExperimentParameters experimentparameters;
      experimentparameters.stageframe = m_StageFrame;
      experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
      experimentparameters.myframe = m_MyFrame;
      experimentparameters.maxforcelimit = m_MaxForceLimit;
      experimentparameters.minforcelimit = m_MinForceLimit;
      experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
      experimentparameters.mindistancelimit = m_MinDistanceLimit;
      experimentparameters.type = ExperimentType::Pause;
      experimentparameters.distanceOrForceOrStress = distanceorstressorforce;
      experimentparameters.gagelength = m_GageLength;
      experimentparameters.mountinglength = m_MountingLength;
      experimentparameters.maxposdistance = m_MaxPosDistance;
      experimentparameters.currentdistance = m_CurrentDistance;
      experimentparameters.area = m_Area;
      std::unique_ptr<Experiment> experiment(new Pause(experimentparameters,

                                                       m_ForceStressDistanceGraph,
                                                       m_ForceStressDisplacementGraph,
                                                       m_VectorLayerMutex,
                                                       m_MaxStressForceLimitGraph,
                                                       m_MinStressForceLimitGraph,
                                                       m_MaxDistanceLimitGraph,
                                                       m_MinDistanceLimitGraph,

                                                       m_Wait,
                                                       m_WaitMutex));

      // Add experiment.
      addExperiment(experiment);

    }else if(0 == strcmp("PauseResume", node.name())){ // Pause/Resume experiment.
      DistanceOrForceOrStress distanceorstressorforce = static_cast<DistanceOrForceOrStress>(node.attribute("DistanceOrStressOrForce").as_int());

      // Create pause resume experiment.
      ExperimentParameters experimentparameters;
      experimentparameters.stageframe = m_StageFrame;
      experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
      experimentparameters.myframe = m_MyFrame;
      experimentparameters.maxforcelimit = m_MaxForceLimit;
      experimentparameters.minforcelimit = m_MinForceLimit;
      experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
      experimentparameters.mindistancelimit = m_MinDistanceLimit;
      experimentparameters.type = ExperimentType::PauseResume;
      experimentparameters.distanceOrForceOrStress = distanceorstressorforce;
      experimentparameters.gagelength = m_GageLength;
      experimentparameters.mountinglength = m_MountingLength;
      experimentparameters.maxposdistance = m_MaxPosDistance;
      experimentparameters.currentdistance = m_CurrentDistance;
      experimentparameters.area = m_Area;
      std::unique_ptr<Experiment> experiment(new PauseResume(experimentparameters,

                                                             m_ForceStressDistanceGraph,
                                                             m_ForceStressDisplacementGraph,
                                                             m_VectorLayerMutex,
                                                             m_MaxStressForceLimitGraph,
                                                             m_MinStressForceLimitGraph,
                                                             m_MaxDistanceLimitGraph,
                                                             m_MinDistanceLimitGraph,

                                                             m_Wait,
                                                             m_WaitMutex));

      // Add experiment.
      addExperiment(experiment);
    }

  }
}

/**
 * @brief Saves the current protocol to the desired place as an .xml file.
 */
void Protocols::saveProtocol(std::string path){
  pugi::xml_document doc;

  // Collect the xml attributes from the single experiments.
  for(int i = 0; i < m_Experiments.size(); ++i){
    m_Experiments[i]->getXML(doc);
  }

  doc.save_file(path.c_str());
  wxLogMessage(std::string("Protocols: Protocol saved in: " + path).c_str());
}

/**
 * @brief Remembers, which experiment will be changed and returns the experiment type.
 * @return The experiment type.
 */
ExperimentType Protocols::getEditExperimentType(void){
  m_EditedExperiment = m_ListBox->GetSelection();
  return(m_Experiments[m_EditedExperiment]->getExperimentType());
}

/**
 * @brief Updates the parameters of the edited experiment in the wxListBox.
 */
void Protocols::updateEditedExperimentParameters(void){
  const wxString tmp((m_ExperimentValues[m_EditedExperiment])->experimentTypeToString() + ":" + m_ExperimentValues[m_EditedExperiment]->experimentSettingsForName());
  m_ListBox->SetString(m_EditedExperiment, tmp);
}

/**
 * @brief Set new limits and forwards them to the experiments.
 * @param mindistancelimit Value for the minimal distance limit.
 * @param maxdistancelimit Value for the maximal distance limit.
 * @param minforcelimit Value for the minimal force limit.
 * @param maxforcelimit Value for the maximal force limit.
 */
void Protocols::setLimits(long mindistancelimit, long maxdistancelimit, long minforcelimit, long maxforcelimit){
  m_MinDistanceLimit = mindistancelimit;
  m_MaxDistanceLimit = maxdistancelimit;
  m_MinForceLimit = minforcelimit;
  m_MaxForceLimit = maxdistancelimit;

  for(auto i : m_Experiments){
    i->setLimits(mindistancelimit, maxdistancelimit, minforcelimit, maxforcelimit);
  }
}

/**
 * @brief Sets the cross section area.
 * @param crosssectionarea The cross section area.
 */
void Protocols::setCrossSectionArea(double crosssectionarea){
  m_Area = crosssectionarea;

  for(auto i : m_ExperimentValues){
    i->setCrossSectionArea(m_Area);
  }
}

/**
 * @brief Sets the new sensitivities.
 * @param forcestresssensitivity The force/stress sensitivity.
 * @param distancesensitivity The distance sensitivity.
 */
void Protocols::setSensitivities(long forcestresssensitivity, long distancesensitivity){
  m_ForceStressSensitivity = forcestresssensitivity;
  m_DistanceSensitivity = distancesensitivity;

  for(auto i : m_Experiments){
    i->setSensitivities(m_ForceStressSensitivity, m_DistanceSensitivity);
  }
}

/**
 * @brief Create the preview vector and display it in the graph.
 */
void Protocols::makePreview(void){
  if(0 == m_Experiments.size()){
    return;
  }

  m_PreviewValues.clear();
  m_PreviewStressForceLimitTimePoints.clear();
  m_MaxStressForceLimits.clear();
  m_MinStressForceLimits.clear();
  m_MaxDistanceLimits.clear();
  m_MinDistanceLimits.clear();

  getPreviewValues();

  // Create limit vectors
  m_PreviewStressForceLimitTimePoints.push_back(m_PreviewValues.front().timepoint);
  m_PreviewStressForceLimitTimePoints.push_back(m_PreviewValues.back().timepoint);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit / 10000.0);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit / 10000.0);
  m_MinStressForceLimits.push_back(m_MinForceLimit / 10000.0);
  m_MinStressForceLimits.push_back(m_MinForceLimit / 10000.0);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit * 0.00009921875/*mm per micro step*/);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit * 0.00009921875/*mm per micro step*/);

  // Set the the vector data.
  m_DistancePreviewGraph->SetData(m_DistanceTimePreviewValues, m_DistancePreviewValues);
  m_StressForcePreviewGraph->SetData(m_StressForceTimePreviewValues, m_StressForcePreviewValues);
  m_MaxStressForceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MaxStressForceLimits);
  m_MinStressForceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MinStressForceLimits);
  m_MaxDistanceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MaxDistanceLimits);
  m_MinDistanceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MinDistanceLimits);

  // Show preview in the graph.
  m_MyFrame->showPreviewGraph();
}

/**
 * @brief Get the preview values.
 */
void Protocols::getPreviewValues(void){
  getValues();

  // Split preview point into stressforce and distance points.
  for(auto i : m_PreviewValues){
    if(DistanceOrForceOrStress::Distance ==  i.distanceOrForce){
      m_DistancePreviewValues.push_back(i.value * 0.00009921875/*mm per micro step*/);
      m_DistanceTimePreviewValues.push_back(i.timepoint);
    } else if(DistanceOrForceOrStress::Force ==  i.distanceOrForce){
      m_StressForcePreviewValues.push_back(i.value / 10000.0);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    } else if(DistanceOrForceOrStress::Stress ==  i.distanceOrForce){
      m_StressForcePreviewValues.push_back((i.value / 10.0) / m_Area);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    }
  }
}

/**
* @brief Get the values for the limit check.
*/
void Protocols::getLimitValues(void){
  getValues();

  // Split preview point into stressforce and distance points.
  for(auto i : m_PreviewValues){
    if(DistanceOrForceOrStress::Distance ==  i.distanceOrForce){
      m_DistancePreviewValues.push_back(i.value);
      m_DistanceTimePreviewValues.push_back(i.timepoint);
    } else if(DistanceOrForceOrStress::Force ==  i.distanceOrForce){
      m_StressForcePreviewValues.push_back(i.value);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    } else if(DistanceOrForceOrStress::Stress ==  i.distanceOrForce){
      m_StressForcePreviewValues.push_back(i.value * m_Area / 1000);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    }
  }
}

/**
* @brief Get the values from the experiments.
*/
void Protocols::getValues(void){
  m_DistancePreviewValues.clear();
  m_DistanceTimePreviewValues.clear();
  m_StressForcePreviewValues.clear();
  m_StressForceTimePreviewValues.clear();

  // Collect the preview points from the single experiments.
  for(int i = 0; i < m_Experiments.size(); ++i){
    m_Experiments[i]->getPreview(m_PreviewValues);
  }
}

/**
 * @brief Runs the protocol.
 */
void Protocols::runProtocol(void){
  // Check if the protocol will exceed some limits and returns with a message in this case.
  if(false == checkProtocol()){
    std::unique_ptr<wxMessageDialog> popup = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(m_MyFrame, "Limit will exeed, check your experiment settings."));
    popup->ShowModal();
    return;
  }

  // Reset recorded experiment values.
  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    m_ExperimentValues[i]->resetProtocol();
  }

  m_StopProtocolFlag = false;
  m_CurrentExperimentNr = 0;

  // Clear vectors
  m_PreviewStressForceLimitTimePoints.clear();
  m_DistanceLimitTimePoints.clear();
  m_MaxStressForceLimits.clear();
  m_MinStressForceLimits.clear();
  m_MaxDistanceLimits.clear();
  m_MinDistanceLimits.clear();

  // Create limit vectors
  m_PreviewStressForceLimitTimePoints.push_back(m_MyFrame->getCurrentDistance() * 0.00009921875/*mm per micro step*/);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit / 10000.0);
  m_MinStressForceLimits.push_back(m_MinForceLimit / 10000.0);
  m_DistanceLimitTimePoints.push_back(m_MyFrame->getCurrentForce() / 10000.0);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit * 0.00009921875/*mm per micro step*/);

  m_MaxStressForceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MaxStressForceLimits);
  m_MinStressForceLimitGraph->SetData(m_PreviewStressForceLimitTimePoints, m_MinStressForceLimits);
  m_MaxDistanceLimitGraph->SetData(m_DistanceLimitTimePoints, m_MaxDistanceLimits);
  m_MinDistanceLimitGraph->SetData(m_DistanceLimitTimePoints, m_MinDistanceLimits);
  wxLogMessage(std::string("Protocols: m_PreviewStressForceLimitTimePoints: " + std::to_string(m_PreviewStressForceLimitTimePoints.size()) + " m_MaxStressForceLimits: " + std::to_string(m_MaxStressForceLimits.size())).c_str());
  wxLogMessage(std::string("Protocols: m_DistanceLimitTimePoints: " + std::to_string(m_DistanceLimitTimePoints.size()) + " m_MaxDistanceLimits: " + std::to_string(m_MaxDistanceLimits.size())).c_str());

  m_MyFrame->createValuesGraph();

  // Only continue if there are expeiments in the protocol.
  if(0 < m_Experiments.size()){
    // Run first experiment.

    // Check if the experiment is a preloading experiment where a flag has to be set.
    if(ExperimentType::Preload == m_Experiments[m_CurrentExperimentNr]->getExperimentType()){
      std::lock_guard<std::mutex> lck{*m_PreloadDoneMutex};
      *m_PreloadDoneFlag = false;
    }

    {
      std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
      m_ExperimentRunningFlag = true;
    }
    // Set start point.
    m_StartTimePoint = std::chrono::high_resolution_clock::now();

    // Start recording measurement values.
    {
      std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
      m_MeasurementValuesRecordingFlag = true;
    }
    // Clear graph vectors.
    m_StressForceGraphValues.clear();
    m_DistanceGraphValues.clear();
    m_DisplacementGraphValues.clear();
    // Create shared_ptr's which are needed.
    std::shared_ptr<std::vector<double>> stressforcegraph(&m_StressForceGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> distancegraph(&m_DistanceGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> displacementgraph(&m_DisplacementGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> maxforcelimitgraph(&m_MaxStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> minforcelimitgraph(&m_MinStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> maxdistancelimitgraph(&m_MaxDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> mindistancelimitgraph(&m_MinDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> forcelimitstimepointsgraph(&m_PreviewStressForceLimitTimePoints, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> distancelimitstimepointsgraph(&m_DistanceLimitTimePoints, do_nothing_deleter);
    // Start recording values.
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement(stressforcegraph,
                                                                distancegraph,
                                                                displacementgraph,
                                                                maxforcelimitgraph,
                                                                minforcelimitgraph,
                                                                maxdistancelimitgraph,
                                                                mindistancelimitgraph,
                                                                forcelimitstimepointsgraph,
                                                                distancelimitstimepointsgraph);

    // Mark the running experiment in the list box.
    m_ListBox->SetSelection(m_CurrentExperimentNr);
    // Start the experiment.
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Experiment::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.detach();
    m_CurrentExperimentNr++;

    // Start checkFinishedExperiment method to catch the end of the experiment.
    m_ExperimentRunningThread.reset(new std::thread(&Protocols::checkFinishedExperiment, this));
    m_ExperimentRunningThread->detach();
  }
}

/**
 * @brief Process the next experiments.
 */
void Protocols::process(void){
  // Return if protocol should be stopped.
  if(true == m_StopProtocolFlag){
    return;
  }

  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }
  if(m_Experiments.size() > m_CurrentExperimentNr){

    // Check if the experiment is a preloading experiment where a flag has to be set.
    if(ExperimentType::Preload == m_Experiments[m_CurrentExperimentNr]->getExperimentType()){
      std::lock_guard<std::mutex> lck{*m_PreloadDoneMutex};
      *m_PreloadDoneFlag = false;
    }
    {
      std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
      m_ExperimentRunningFlag = true;
    }

    // Start recording measurement values.
    {
      std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
      m_MeasurementValuesRecordingFlag = true;
    }

    // Create shared_ptr's which are needed.
    std::shared_ptr<std::vector<double>> stressforcegraph(&m_StressForceGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> distancegraph(&m_DistanceGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> displacementgraph(&m_DisplacementGraphValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> maxforcelimitgraph(&m_MaxStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> minforcelimitgraph(&m_MinStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> maxdistancelimitgraph(&m_MaxDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> mindistancelimitgraph(&m_MinDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> forcelimitstimepointsgraph(&m_PreviewStressForceLimitTimePoints, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> distancelimitstimepointsgraph(&m_DistanceLimitTimePoints, do_nothing_deleter);
    // Start recording values.
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement(stressforcegraph,
                                                                distancegraph,
                                                                displacementgraph,
                                                                maxforcelimitgraph,
                                                                minforcelimitgraph,
                                                                maxdistancelimitgraph,
                                                                mindistancelimitgraph,
                                                                forcelimitstimepointsgraph,
                                                                distancelimitstimepointsgraph);

    // Mark the running experiment in the list box.
    m_ListBox->SetSelection(m_CurrentExperimentNr);
    //std::cout << "Protocols: start experiment nr.: " << m_CurrentExperimentNr << std::endl;
    // Start the experiment.
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Experiment::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.detach();
    m_CurrentExperimentNr++;

    // Start checkFinishedExperiment method to catch the end of the experiment.
    m_ExperimentRunningThread.reset(new std::thread(&Protocols::checkFinishedExperiment, this));
    m_ExperimentRunningThread->detach();

  } else{
    m_CurrentExperimentNr = 0;
    m_ListBox->SetSelection(m_CurrentExperimentNr);

    if(true == m_LoopProtocolFlag){
      //runProtocol();
      process();
    }else{
      // Ask the user if the recorded data should be saved.
      m_MyFrame->showExportCSVDialogFromProtocols();
    }
  }
}

/**
 * @brief Checks if protocol exceeds some limits.
 */
bool Protocols::checkProtocol(void){
  // Get the values for checking limits.
  getLimitValues();

  double maxforcelimit = m_MaxForceLimit / 10000.0;
  double minforcelimit = m_MinForceLimit / 10000.0;
  double maxdistancelimit = m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/;
  double mindistancelimit = m_MinDistanceLimit * 0.00009921875/*mm per micro step*/;

  for(double i : m_StressForcePreviewValues){
    if((i > (m_MaxForceLimit)) || (i < (m_MinForceLimit))){
    //if((i > (maxforcelimit)) || (i < (minforcelimit))){
      return(false);
    }
  }
  for(double i : m_DistancePreviewValues){
    if((i > (m_MaxDistanceLimit)) || (i < (m_MinDistanceLimit))){
    //if((i > (maxdistancelimit)) || (i < (mindistancelimit))){
      return(false);
    }
  }
  return(true);
}

/**
 * @brief Stops the protocol.
 */
void Protocols::stopProtocol(void){
  // Wait until the stages stopped.
  {
    bool tmp = false;
    while(false == tmp){
      std::unique_lock<std::mutex> lck3(*m_StagesStoppedMutex);
      tmp = *m_StagesStoppedFlag;
    }
  }

  m_StopProtocolFlag = true;
  {
    std::lock_guard<std::mutex> lck(*m_WaitMutex);
    m_Wait->notify_all();
  }
  //clearGraphStop();
}

/**
 * @brief Moves the experiment from the current position to the one before.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentUp(int experimentPosition){
  // Return if the experiment to move is the currently running experiment.
  /*
  std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
  if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == m_CurrentExperimentNr)){
    return;
  }
  */

  // Check if the poition is not the first position.
  if(0 < experimentPosition){
    // Swap the elements in the vector.
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition - 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition - 1]);

    // Swap the elements in the list box.
    const wxString stringPos(m_ListBox->GetString(experimentPosition));
    const wxString stringUp(m_ListBox->GetString(experimentPosition - 1));
    m_ListBox->SetString(experimentPosition, stringUp);
    m_ListBox->SetString(experimentPosition - 1, stringPos);

    // Swap the selection in the list box.
    m_ListBox->SetSelection(experimentPosition, false);
    m_ListBox->SetSelection(experimentPosition - 1, true);

    // Adapt the current experiment number if the experiment to move is the currently running experiment.
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == m_CurrentExperimentNr)){
      m_CurrentExperimentNr--;
    }else if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == (m_CurrentExperimentNr + 1))){
      m_CurrentExperimentNr++;
    }
  }
}

/**
 * @brief Moves the experiment from the current position to the one after.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentDown(int experimentPosition){
  // Return if the experiment to move is the currently running experiment.
  /*
  std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
  if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == m_CurrentExperimentNr)){
    return;
  }
  */

  // Check if the poition is not the last position.
  if(m_Experiments.size() > experimentPosition){
    // Swap the elements in the vector.
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition + 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition + 1]);

    // Swap the elements in the list box.
    const wxString stringPos(m_ListBox->GetString(experimentPosition));
    const wxString stringDown(m_ListBox->GetString(experimentPosition + 1));
    m_ListBox->SetString(experimentPosition, stringDown);
    m_ListBox->SetString(experimentPosition + 1, stringPos);

    // Swap the selection in the list box.
    m_ListBox->SetSelection(experimentPosition, false);
    m_ListBox->SetSelection(experimentPosition + 1, true);

    // Adapt the current experiment number if the experiment to move is the currently running experiment.
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == m_CurrentExperimentNr)){
      m_CurrentExperimentNr++;
    }else if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == (m_CurrentExperimentNr - 1))){
      m_CurrentExperimentNr--;
    }
  }
}

/**
 * @brief Adds an experiment.
 * @param experiment Pointer to the experiment object.
 */
void Protocols::addExperiment(std::unique_ptr<Experiment> &experiment){
  m_Experiments.push_back(std::move(experiment));
  // Add experiment value object.
  m_ExperimentValues.push_back(m_Experiments.back()->getExperimentValues());

  // Add string in list box.
  const wxString tmp((m_ExperimentValues.back())->experimentTypeToString() + ":" + m_ExperimentValues.back()->experimentSettingsForName());
  m_ListBox->Append(tmp);
}

/**
 * @brief Removes the Experiment at the desired position.
 * @param experimentPosition Position of the experiment.
 */
void Protocols::removeExperiment(int experimentPosition){

  // Return if an experiment is currently running
  /*
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }
  */

  // Return if the experiment to remove is the currently running experiment.
  std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
  if((true == m_ExperimentRunningFlag) && ((experimentPosition + 1) == m_CurrentExperimentNr)){
    return;
  }else{
    m_Experiments[experimentPosition].reset();
    m_Experiments.erase(m_Experiments.begin() + experimentPosition);
    m_ExperimentValues[experimentPosition].reset();
    m_ExperimentValues.erase(m_ExperimentValues.begin() + experimentPosition);

    m_ListBox->Delete(experimentPosition);
  }
}

/**
 * @brief Sets the experiment start time point.
 */
void Protocols::setStartPoint(void){
  m_StartTimePoint = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Returns the experiment names as a vector.
 * @return The experiment names as a vector.
 */
std::vector<std::string> Protocols::getExperimentNames(void){
  std::vector<std::string> stringvector;

  for(int i = 0; i < m_ListBox->GetCount(); ++i){
    stringvector.push_back(m_ListBox->GetString(i).ToStdString());
  }

  return(stringvector);
}

/**
 * @brief Checks if there is any data to export.
 * @return True if there is some data / False otherwise.
 */
bool Protocols::hasData(void){
  if(true == m_StressForceGraphValues.empty()){
    return(false);
  }else{
    return(true);
  }
}

/**
 * @brief Exports the measurement data to a .csv file.
 */
void Protocols::exportCSV(std::vector<bool> disableexport, std::string pathname){

  // Creat the file
  std::ofstream file(pathname);

  // Open the file
  if(false == file.is_open()){
    std::cerr << "Couldn't open file" << std::endl;
    throw "Couldn't open file";
  }

  // Printing header
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));
  file << "Protocol: " <<  "Date/Time: " << std::string(mbstr) << std::endl << std::endl;

  // Printing the experiment settings.
  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    if(false == disableexport[i]){
      file << m_ExperimentValues[i]->getExperimentSettings();
    }
  }

  file << std::endl << std::endl;

  file << "Stress/Force in " << m_ExperimentValues[0]->getForceOrStress() << "; Time stamp for stress/force in milliseconds; Distance in mm; Time stamp for the distance in milliseconds" << std::endl;

  int length = m_ExperimentValues[0]->getStressForceValues()->size();
  for(long j = 0; j < length; ++j){
    for(int i = 0; i < m_ExperimentValues.size(); ++i){
      if(false == disableexport[i]){

        // Get the pointer to the vectors containing the measurement values.
        std::vector<std::vector<ExperimentValues::MeasurementValue>>* stressForceValues = m_ExperimentValues[i]->getStressForceValues();
        std::vector<std::vector<ExperimentValues::MeasurementValue>>* distanceValues = m_ExperimentValues[i]->getDistanceValues();

        // Correct the vector size if needed.
        long length2 = 0;
        if(stressForceValues->operator [](j).size() > distanceValues->operator [](j).size()){
          //stressForceValues->operator [](j).resize(distanceValues->operator [](j).size());
          length2 = stressForceValues->operator [](j).size();
        }else if(stressForceValues->operator [](j).size() < distanceValues->operator [](j).size()){
          //distanceValues->operator [](j).resize(stressForceValues->operator [](j).size());
          length2 = distanceValues->operator [](j).size();
        }else{
          length2 = distanceValues->operator [](j).size();
        }

        // Print the measured values.
        for(long i = 0; i < length2; ++i){
          if(stressForceValues->operator [](j).size() > i){
            file << stressForceValues->operator [](j)[i].value << ";"
                 << std::chrono::duration_cast<std::chrono::milliseconds>(stressForceValues->operator [](j)[i].timestamp - m_StartTimePoint).count() << ";";
          }else{
            file << 0 << ";" << 0 << ";";
          }
          if(distanceValues->operator [](j).size() > i){
            file << distanceValues->operator [](j)[i].value << ";"
                 << std::chrono::duration_cast<std::chrono::milliseconds>(distanceValues->operator [](j)[i].timestamp - m_StartTimePoint).count() << ";";
          }else{
            file << 0 << ";" << 0 << ";";
          }
          file << std::endl;
        }
      }
    }
  }

  file.close();

  wxLogMessage(std::string("Saved experiment values to: " + pathname).c_str());
}


/**
 * @brief Sets the m_ExperimentRunningFlag false if experiment is finished and the stages stopped and record preload distance if a preloading happend.
 */
void Protocols::checkFinishedExperiment(void){
  {
    // Wait until experiment is finised.
    std::unique_lock<std::mutex> lck1(*m_WaitMutex);
    m_Wait->wait(lck1);
  }

  /*
  for(auto i : m_Experiments){
    i->setStartLength();
  }
  */
  // Set the start length (distance between the stages) of the next experiment if there is one.
  if(m_Experiments.size() > m_CurrentExperimentNr){
    m_Experiments[m_CurrentExperimentNr]->setStartLength();
  }
  {
    // If preloading is active.
    std::lock_guard<std::mutex> lck2{*m_PreloadDoneMutex};
    if(false == *m_PreloadDoneFlag){
      // Wait until the stages stopped.
      {
        bool tmp = false;
        while(false == tmp){
          std::unique_lock<std::mutex> lck3(*m_StagesStoppedMutex);
          tmp = *m_StagesStoppedFlag;
        }
      }

      *m_PreloadDoneFlag = true;
      // Set preload distance.
      //m_PreloadDistance = m_MyFrame->getCurrentDistance();
      m_PreloadDistance = m_StageFrame->getCurrentDistance();
      wxLogMessage(std::string("Protocols: Preload distance: " + std::to_string(m_PreloadDistance * 0.00009921875/*mm per micro step*/)).c_str());
      // Set the prelod distance in all the experiments.
      for(auto i : m_Experiments){
        i->setPreloadDistance();
      }
    }
  }
  {
    // Stop the recording of the measurement values if it is running.
    std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
    if(true == m_MeasurementValuesRecordingFlag){
      m_MeasurementValuesRecordingFlag = false;
      //m_CurrentExperimentValues->stopMeasurement();
      m_ExperimentValues[m_CurrentExperimentNr-1]->stopMeasurement();
    }
  }
  m_Experiments[m_CurrentExperimentNr - 1]->resetExperiment();
  // Reset the current experiment number if protocol should stop.
  if(true == m_StopProtocolFlag){
    m_CurrentExperimentNr = 0;
  }

  {
    // Indicate that the experiment is not longer running.
    std::lock_guard<std::mutex> lck4{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = false;
  }

  process();
  //delete m_CurrentExperiment;
  //m_CurrentExperiment = NULL;
}

/**
 * @brief Executed by the object main frame when the clear graph button is pressed. Stops the measurement.
 */
void Protocols::clearGraphStop(void){
  // Stop the recording of the measurement values if it is running.
  std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
  if(true == m_MeasurementValuesRecordingFlag){
    m_MeasurementValuesRecordingFlag = false;
    //m_CurrentExperimentValues->stopMeasurement();
    m_ExperimentValues[m_CurrentExperimentNr-1]->stopMeasurement();
  }
}
