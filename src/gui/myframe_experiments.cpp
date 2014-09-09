/**
 * @file myframe_experiments.cpp
 * @brief The main frame experiments part.
 * @author Andreas Ziegler
 */

// Includes
#include "myframe.h"
#include "mypausedialog.h"
#include "protocols.h"
#include "../experiments/preload.h"
#include "../experiments/onestepevent.h"
#include "../experiments/continuousevent.h"
#include "../experiments/pause.h"
#include "../experiments/pauseresume.h"
#include "../experiments/phototrigger.h"

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedPercentChanged(wxSpinDoubleEvent& event){
 //double speedmm = m_MountingLength * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 double speedmm = m_GageLength * 0.00009921875/*mm per micro step*/  * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 m_PreloadSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_PreloadSpeedMmSpinCtrl->GetValue() / (m_GageLength * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  m_PreloadSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in preload. Aborts the edit process.
 * @param event Occuring event
 */
void MyFrame::OnPreloadCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_PreloadSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload. Creates a new preload experiment or changes its parameter
 *        if the user is editing an existing preload experiment.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  // Get parameters.
  PreloadParameters parameters;
  parameters.distanceOrForceOrStress = m_DistanceOrForceOrStress;
  parameters.stressForceLimit = m_PreloadLimitSpinCtrl->GetValue();
  parameters.velocity = m_PreloadSpeedMmSpinCtrl->GetValue();

  // Update parameters if an existing experiment should be changed.
  if(true == m_BlockNotebookTabFlag){
    m_PreloadSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<Preload> preload = std::dynamic_pointer_cast<Preload>(m_CurrentProtocol->getEditExperiment());
    preload->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;

  }else{ // Otherwise create a new experiment.
    //Experiment* experiment = new Preload(ExperimentType::Preload,
    ExperimentParameters experimentparameters;
    experimentparameters.stageframe = m_StageFrame;
    experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
    experimentparameters.myframe = this;
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
                                                       &m_ForceStressDistanceGraph,
                                                       &m_ForceStressDisplacementGraph,
                                                       &m_VectorLayerMutex,
                                                       &m_MaxStressForceLimitGraph,
                                                       &m_MinStressForceLimitGraph,
                                                       &m_MaxDistanceLimitGraph,
                                                       &m_MinDistanceLimitGraph,
                                                       m_ForceStressSensitivity,
                                                       m_DistanceSensitivity,

                                                       &m_Wait,
                                                       &m_WaitMutex,
                                                       &m_StagesStoppedFlag,
                                                       &m_StagesStoppedMutex,

                                                       parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
}

/**
 * @brief Method wich will be executed, when the user chooses stress/force in the one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepStressForce(wxCommandEvent& event){
  m_OneStepDistancePanel->Show(false);
  m_OneStepStressForcePanel->Show(true);
  m_OneStepPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses distance in the one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepDistance(wxCommandEvent& event){
  m_OneStepStressForcePanel->Show(false);
  m_OneStepDistancePanel->Show(true);
  m_OneStepPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in the one step event. Aborts the edit process.
 * @param event Occuring event
 */
void MyFrame::OnOneStepCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_OneStepSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in one step event. Creates a new one step event experiment
 *				or changes its parameter if the user is editing an existing one step event experiment.
 * @param event Occuring event
 */
void MyFrame::OnOneStepSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  // Get parameters.
  OneStepEventParameters parameters;
  if(true == m_OneStepStressForceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = m_DistanceOrForceOrStress;
    parameters.delay = m_OneStepStressForceDelaySpinCtrl->GetValue();
    parameters.dwell = m_OneStepStressForceDwellSpinCtrl->GetValue();
    parameters.holdLimit = m_OneStepStressForceHoldLimitCheckBox->GetValue();
    parameters.limit = m_OneStepStressForceLimitSpinCtrl->GetValue();

    if(true == m_OneStepStressForceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_OneStepStressForceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.velocity = m_OneStepStressForceVelocitySpinCtrl->GetValue();
  }else if(true == m_OneStepDistanceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = DistanceOrForceOrStress::Distance;
    parameters.delay = m_OneStepDistanceDelaySpinCtrl->GetValue();

    if(true == m_OneStepDistanceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_OneStepDistanceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.velocity = m_OneStepDistanceVelocitySpinCtrl->GetValue();

    if(true == m_OneStepDistanceLimitMmRelRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
    }else if(true == m_OneStepDistanceLimitMmRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::Distance;
    }else if(true == m_OneStepDistanceLimitPercentRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.limit = m_OneStepDistanceLimitSpinCtrl->GetValue();
    parameters.dwell = m_OneStepDistanceDwellSpinCtrl->GetValue();
  }
  //wxLogMessage(std::string("MyFrame: limit: " + std::to_string(parameters.limit)).c_str());

  if(true == m_OneStepEndOfEventHoldMmRelRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
  }else if(true == m_OneStepEndOfEventHoldMmRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::Distance;
  }else if(true == m_OneStepEndOfEventHoldPercentRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::Percentage;
  }
  parameters.holdDistance = m_OneStepEndOfEventHoldSpinCtrl->GetValue();

  if(true == m_OneStepEndOfEventRepeatCheckBox->GetValue()){
    parameters.cycles = m_OneStepEndOfEventRepeatSpinCtrl->GetValue();
  }else{
    parameters.cycles = 1;
  }

  if(true == m_OneStepEndOfEventHoldRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::HoldADistance;
  }else if(true == m_OneStepEndOfEventL0RadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToL0;
  }else if(true == m_OneStepEndOfEventStopRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::Stop;
  }else if(true == m_OneStepEndOfEventMLRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToML;
  }

  // Update parameters if an existing experiment should be changed.
  if(true == m_BlockNotebookTabFlag){
    m_OneStepSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<OneStepEvent> onestepevent = std::dynamic_pointer_cast<OneStepEvent>(m_CurrentProtocol->getEditExperiment());
    onestepevent->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;

  }else{ // Otherwise create a new experiment.
    ExperimentParameters experimentparameters;
    experimentparameters.stageframe = m_StageFrame;
    experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
    experimentparameters.myframe = this;
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
                                                            &m_ForceStressDistanceGraph,
                                                            &m_ForceStressDisplacementGraph,
                                                            &m_VectorLayerMutex,
                                                            &m_MaxStressForceLimitGraph,
                                                            &m_MinStressForceLimitGraph,
                                                            &m_MaxDistanceLimitGraph,
                                                            &m_MinDistanceLimitGraph,
                                                            m_ForceStressSensitivity,
                                                            m_DistanceSensitivity,

                                                            &m_Wait,
                                                            &m_WaitMutex,

                                                            parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
}

/**
 * @brief Method wich will be executed, when the user chooses stress/force in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousStressForce(wxCommandEvent& event){
  m_ContinuousDistancePanel->Show(false);
  m_ContinuousStressForcePanel->Show(true);
  m_ContinuousPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses distance in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousDistance(wxCommandEvent& event){
  m_ContinuousStressForcePanel->Show(false);
  m_ContinuousDistancePanel->Show(true);
  m_ContinuousPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses max value in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousMaxValue(wxCommandEvent& event){
  m_ContinuousStressForceStepsStaticText->Show(false);
  m_ContinuousStressForceStepsSpinCtrl->Show(false);
  m_ContinuousDistanceStepsStaticText->Show(false);
  m_ContinuousDistanceStepsSpinCtrl->Show(false);

  m_ContinuousStressForceMaxValueStaticText->Show(true);
  m_ContinuousStressForceMaxValueSpinCtrl->Show(true);
  m_ContinuousStressForceMaxValueValueRadioBtn->Show(true);
  m_ContinuousStressForceMaxValuePercentRadioBtn->Show(true);
  m_ContinuousDistanceMaxValueStaticText->Show(true);
  m_ContinuousDistanceMaxValueSpinCtrl->Show(true);
  m_ContinuousDistancePanel23->Show(true);

  m_ContinuousStressForcePanel->Layout();
  m_ContinuousDistancePanel->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses steps in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousSteps(wxCommandEvent& event){
  m_ContinuousStressForceMaxValueStaticText->Show(false);
  m_ContinuousStressForceMaxValueSpinCtrl->Show(false);
  m_ContinuousStressForceMaxValueValueRadioBtn->Show(false);
  m_ContinuousStressForceMaxValuePercentRadioBtn->Show(false);
  m_ContinuousDistanceMaxValueStaticText->Show(false);
  m_ContinuousDistanceMaxValueSpinCtrl->Show(false);
  m_ContinuousDistancePanel23->Show(false);

  m_ContinuousStressForceStepsStaticText->Show(true);
  m_ContinuousStressForceStepsSpinCtrl->Show(true);
  m_ContinuousDistanceStepsStaticText->Show(true);
  m_ContinuousDistanceStepsSpinCtrl->Show(true);

  m_ContinuousStressForcePanel->Layout();
  m_ContinuousDistancePanel->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in the continuous event. Aborts the edit process.
 * @param event Occuring event
 */
void MyFrame::OnContinuousCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_ContinuousSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in the continuous event. Creates a new continuous event experiment
 *				or changes its parameter if the user is editing an existing continuous event experiment.
 * @param event Occuring event
 */
void MyFrame::OnContinuousSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  // Get parameters.
  ContinuousEventParameters parameters;
  parameters.ramp2failure = false;
  if(true == m_ContinuousStressForceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = m_DistanceOrForceOrStress;
    parameters.holdtime = m_ContinuousStressForceHoldTimeSpinCtrl->GetValue();
    parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Distance;
    parameters.increment = m_ContinuousStressForceIncrementSpinCtrl->GetValue();
    parameters.velocity = m_ContinuousStressForceVelocitySpinCtrl->GetValue();
    parameters.maxvalue = m_ContinuousStressForceMaxValueSpinCtrl->GetValue();

    if(true == m_ContinuousStressForceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousStressForceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousStressForceMaxValueRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousStressForceMaxValuePercentRadioBtn->GetValue()){
        parameters.ramp2failure = true;
      }else if(true == m_ContinuousStressForceMaxValueValueRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Distance;
      }
    } else if(true == m_ContinuousStressForceStepsRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::Steps;
      parameters.steps = m_ContinuousStressForceStepsSpinCtrl->GetValue();
    }

  }else if(true == m_ContinuousDistanceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = DistanceOrForceOrStress::Distance;
    parameters.velocity = m_ContinuousDistanceVelocitySpinCtrl->GetValue();
    parameters.holdtime = m_ContinuousDistanceHoldTimeSpinCtrl->GetValue();
    parameters.increment = m_ContinuousDistanceIncrementSpinCtrl->GetValue();
    parameters.maxvalue = m_ContinuousDistanceMaxValueSpinCtrl->GetValue();

    if(true == m_ContinuousDistanceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousDistanceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousDistanceIncrementMmRadioBtn->GetValue()){
      parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousDistanceIncrementPercentRadioBtn->GetValue()){
      parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousDistanceMaxValueRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousDistanceMaxValueMmRelRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
      }else if(true == m_ContinuousDistanceMaxValueMmRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Distance;
      }else if(true == m_ContinuousDistanceMaxValuePercentRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Percentage;
      }
    } else if(true == m_ContinuousDistanceStepsRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::Steps;
      parameters.steps = m_ContinuousDistanceStepsSpinCtrl->GetValue();
    }
  }

  if(true == m_ContinuousEndOfEventRepeatCheckBox->GetValue()){
    parameters.cycles = m_ContinuousEndOfEventRepeatSpinCtrl->GetValue();
  }else{
    parameters.cycles = 1;
  }

  if(true == m_ContinuousEndOfEventStopRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::Stop;
  }else if(true == m_ContinuousEndOfEventL0RadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToL0;
  }else if(true == m_ContinuousEndOfEventMLRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToML;
  }else if(true == m_ContinuousEndOfEventHoldRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::HoldAForce;

      //parameters.stopAtForceStress = m_ContinuousEndOfEventHoldSpinCtrl->GetValue() * m_Area * 10.0;
    parameters.stopAtForceStress = m_ContinuousEndOfEventHoldSpinCtrl->GetValue();
  }


  // Update parameters if an existing experiment should be changed.
  if(true == m_BlockNotebookTabFlag){
    m_ContinuousSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<ContinuousEvent> continuousevent = std::dynamic_pointer_cast<ContinuousEvent>(m_CurrentProtocol->getEditExperiment());
    continuousevent->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }else{ // Otherwise creat a new experiment.
    ExperimentParameters experimentparameters;
    experimentparameters.stageframe = m_StageFrame;
    experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
    experimentparameters.myframe = this;
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
                                                               &m_ForceStressDistanceGraph,
                                                               &m_ForceStressDisplacementGraph,
                                                               &m_VectorLayerMutex,
                                                               &m_MaxStressForceLimitGraph,
                                                               &m_MinStressForceLimitGraph,
                                                               &m_MaxDistanceLimitGraph,
                                                               &m_MinDistanceLimitGraph,
                                                               m_ForceStressSensitivity,
                                                               m_DistanceSensitivity,

                                                               &m_Wait,
                                                               &m_WaitMutex,

                                                               parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the pause experiment down button. Creates a pause experiment.
 * @param event Occuring event
 */
void MyFrame::OnPauseExperiment(wxCommandEvent& event){
  checkProtocol();

  // Get parmeters.
  ExperimentParameters experimentparameters;
  experimentparameters.stageframe = m_StageFrame;
  experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
  experimentparameters.myframe = this;
  experimentparameters.maxforcelimit = m_MaxForceLimit;
  experimentparameters.minforcelimit = m_MinForceLimit;
  experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
  experimentparameters.mindistancelimit = m_MinDistanceLimit;
  experimentparameters.type = ExperimentType::Pause;
  experimentparameters.distanceOrForceOrStress = m_DistanceOrForceOrStress;
  experimentparameters.gagelength = m_GageLength;
  experimentparameters.mountinglength = m_MountingLength;
  experimentparameters.maxposdistance = m_MaxPosDistance;
  experimentparameters.currentdistance = m_CurrentDistance;
  experimentparameters.area = m_Area;
  std::unique_ptr<Experiment> experiment(new Pause(experimentparameters,

                                                   &m_ForceStressDistanceGraph,
                                                   &m_ForceStressDisplacementGraph,
                                                   &m_VectorLayerMutex,
                                                   &m_MaxStressForceLimitGraph,
                                                   &m_MinStressForceLimitGraph,
                                                   &m_MaxDistanceLimitGraph,
                                                   &m_MinDistanceLimitGraph,

                                                   &m_Wait,
                                                   &m_WaitMutex));

  Pause *ptr = dynamic_cast<Pause*>(experiment.get());

  // Create dialog to choose the pause time.
  std::unique_ptr<MyPauseDialog> dialog = std::unique_ptr<MyPauseDialog>(new MyPauseDialog(ptr));
  dialog->ShowModal();

  if(true == dialog->getCreateExperimentFlag()){
    m_CurrentProtocol->addExperiment(experiment);
  }
  //dialog->Close();
}

/**
 * @brief Method wich will be executed, when the user clicks on the pause resume experiment button. Creates an pause/resume experiment.
 * @param event Occuring event
 */
void MyFrame::OnPauseResumeExperiment(wxCommandEvent& event){

  checkProtocol();

  // Get parameters.
  ExperimentParameters experimentparameters;
  experimentparameters.stageframe = m_StageFrame;
  experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
  experimentparameters.myframe = this;
  experimentparameters.maxforcelimit = m_MaxForceLimit;
  experimentparameters.minforcelimit = m_MinForceLimit;
  experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
  experimentparameters.mindistancelimit = m_MinDistanceLimit;
  experimentparameters.type = ExperimentType::PauseResume;
  experimentparameters.distanceOrForceOrStress = m_DistanceOrForceOrStress;
  experimentparameters.gagelength = m_GageLength;
  experimentparameters.mountinglength = m_MountingLength;
  experimentparameters.maxposdistance = m_MaxPosDistance;
  experimentparameters.currentdistance = m_CurrentDistance;
  experimentparameters.area = m_Area;
  std::unique_ptr<Experiment> experiment(new PauseResume(experimentparameters,

                                                         &m_ForceStressDistanceGraph,
                                                         &m_ForceStressDisplacementGraph,
                                                         &m_VectorLayerMutex,
                                                         &m_MaxStressForceLimitGraph,
                                                         &m_MinStressForceLimitGraph,
                                                         &m_MaxDistanceLimitGraph,
                                                         &m_MinDistanceLimitGraph,

                                                         &m_Wait,
                                                         &m_WaitMutex));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Method wich will be executed, when the user clicks on the photo button. Creates a photo trigger point.
 * @param event Occuring event
 */
void MyFrame::OnMakePhoto(wxCommandEvent& event){

  checkProtocol();

  // Get parameters.
  ExperimentParameters experimentparameters;
  experimentparameters.stageframe = m_StageFrame;
  experimentparameters.forcesensormessagehandler = m_ForceSensorMessageHandler;
  experimentparameters.myframe = this;
  experimentparameters.maxforcelimit = m_MaxForceLimit;
  experimentparameters.minforcelimit = m_MinForceLimit;
  experimentparameters.maxdistancelimit = m_MaxDistanceLimit;
  experimentparameters.mindistancelimit = m_MinDistanceLimit;
  experimentparameters.type = ExperimentType::Photo;
  experimentparameters.distanceOrForceOrStress = m_DistanceOrForceOrStress;
  experimentparameters.gagelength = m_GageLength;
  experimentparameters.mountinglength = m_MountingLength;
  experimentparameters.maxposdistance = m_MaxPosDistance;
  experimentparameters.currentdistance = m_CurrentDistance;
  experimentparameters.area = m_Area;

  std::unique_ptr<Experiment> experiment(new PhotoTrigger(experimentparameters,

                                                          &m_ForceStressDistanceGraph,
                                                          &m_ForceStressDisplacementGraph,
                                                          &m_VectorLayerMutex,
                                                          &m_MaxStressForceLimitGraph,
                                                          &m_MinStressForceLimitGraph,
                                                          &m_MaxDistanceLimitGraph,
                                                          &m_MinDistanceLimitGraph,

                                                          &m_Wait,
                                                          &m_WaitMutex,

                                                          m_SerialTrigger));

  m_CurrentProtocol->addExperiment(experiment);

}

/**
 * @brief Shows pause/resume dialog.
 */
void MyFrame::showPauseResumeDialogFromPauseResume(std::condition_variable *wait, std::mutex *mutex){
  CallAfter(&MyFrame::showPauseResumeDialog, wait, mutex);
}

/**
 * @brief Shows pause/resume dialog.
 */
void MyFrame::showPauseResumeDialog(std::condition_variable *wait, std::mutex *mutex){
  std::unique_ptr<wxMessageDialog> popup = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this, "Push the button OK to resume the protocol."));
  popup->ShowModal();
  std::lock_guard<std::mutex> lck(*mutex);
  wait->notify_all();
}
