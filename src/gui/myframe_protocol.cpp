/**
 * @file myframe_protocol.cpp
 * @brief The main frame protocol part.
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

/**
 * @brief Method wich will be executed, when the user clicks on the preview protocol button.
 * @param event Occuring event
 */
void MyFrame::OnPreviewProtocol(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->makePreview();
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the run protocol button.
 * @param event Occuring event
 */
void MyFrame::OnRunProtocol(wxCommandEvent& event){
  /*
  wxMessageDialog *popup = new wxMessageDialog(this, "Limit will exeed, check your experiment settings.");
  popup->Show();
  popup->ShowModal();
  delete popup;
  */
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->runProtocol();
  }
}

/**
 * @brief Method wich will be executed, when the user activates Loop in protocol.
 * @param event Occuring event
 */
void MyFrame::OnLoopProtocol(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->setLoopFlag(m_ProtocolsLoopCheckBox->GetValue());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the stop protocol button. Stops the stage frame and the protocol.
 * @param event Occuring event
 */
void MyFrame::OnStopProtocol(wxCommandEvent& event){
  //OnMotorStop(event);
  m_StageFrame->stop();
  m_CurrentProtocol->stopProtocol();
}

/**
 * @brief Method wich will be executed, when the user clicks on the save protocol button.
 * @param event Occuring event
 */
void MyFrame::OnSaveProtocol(wxCommandEvent& event){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));

  std::string pathAndFilename = m_StoragePath + "/" + "Protocol_" + std::string(mbstr) + ".xml";

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save protocol file"), "", "", "Protocol files (*.xml)|*.xml", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  saveFileDialog.SetPath(pathAndFilename);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }

  m_CurrentProtocol->saveProtocol(saveFileDialog.GetPath().ToStdString());
  wxLogMessage(std::string("Protocol saved in: " + saveFileDialog.GetPath()).c_str());
}

/**
 * @brief Method wich will be executed, when the user clicks on the load protocol button.
 * @param event Occuring event
 */
void MyFrame::OnLoadProtocol(wxCommandEvent& event){
  checkProtocol();

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save protocol file"), "", "", "Protocol files (*.xml)|*.xml", wxFD_OPEN);
  saveFileDialog.SetPath(m_StoragePath);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }
  m_CurrentProtocol->loadProtocol(saveFileDialog.GetPath().ToStdString(), m_GageLength, m_MountingLength, m_MaxPosDistance, m_CurrentDistance);
}

/**
 * @brief Checks if a protocol object is already created, otherwise creates it.
 */
void MyFrame::checkProtocol(void){
  if(nullptr == m_CurrentProtocol){
  m_CurrentProtocol = std::shared_ptr<Protocols>(new Protocols(m_ProtocolsListBox,
                                                               this,
                                                               m_StageFrame,
                                                               m_ForceSensorMessageHandler,
                                                               &m_VectorLayerMutex,
                                                               m_GageLength,
                                                               m_MountingLength,
                                                               m_MaxPosDistance,
                                                               m_CurrentDistance,
                                                               &m_StagesStoppedFlag,
                                                               &m_StagesStoppedMutex,
                                                               &m_WaitMutex,
                                                               &m_Wait,
                                                               &m_PreloadDoneFlag,
                                                               &m_PreloadDoneMutex,

                                                               m_ProtocolsLoopCheckBox->GetValue(),
                                                               m_Area,
                                                               m_MaxDistanceLimit,
                                                               m_MinDistanceLimit,
                                                               m_MaxForceLimit,
                                                               m_MinForceLimit,
                                                               m_ForceStressSensitivity,
                                                               m_DistanceSensitivity,

                                                               &m_ForceStressDistanceGraph,
                                                               &m_ForceStressDisplacementGraph,
                                                               &m_StressForcePreviewGraph,
                                                               &m_DistancePreviewGraph,
                                                               &m_MaxStressForceLimitGraph,
                                                               &m_MinStressForceLimitGraph,
                                                               &m_MaxDistanceLimitGraph,
                                                               &m_MinDistanceLimitGraph,
                                                               m_StoragePath));
  }
}

/**
 * @brief Method wich will be executed, when the user makes a right click in the list box. Creates an context menu with the entry "Edit".
 * @param event Occuring event
 */
void MyFrame::OnProtocolsListRightClick(wxMouseEvent& event){
  // Creat context menu with an edit entry
  std::unique_ptr<wxMenu> popup = std::unique_ptr<wxMenu>(new wxMenu);
  popup->Append(wxID_OPEN, wxT("Edit"));

  // Bind the context menu events, pop up the dialog and unbind.
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnEditExperiment, this);
  PopupMenu(popup.get());
  Unbind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnEditExperiment, this);
}

/**
 * @brief Method wich will be executed, when the user clicks on edit in the context menu. Switch to the according experiment tab and loads the parameters.
 * @param event Occuring event
 */
void MyFrame::OnEditExperiment(wxCommandEvent& event){

  switch(m_CurrentProtocol->getEditExperimentType()){
    case ExperimentType::Preload:
      {
        // Change to preload tab.
        m_Experiments->SetSelection(3);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get preload parameters.
        std::shared_ptr<Preload> preload = std::dynamic_pointer_cast<Preload>(m_CurrentProtocol->getEditExperiment());
        PreloadParameters parameters = preload->getParametersForGUI();

        m_PreloadLimitSpinCtrl->SetValue(parameters.stressForceLimit);
        m_PreloadSpeedMmSpinCtrl->SetValue(parameters.velocity);
        m_PreloadSpeedPreloadSpinCtrl->SetValue(parameters.velocity / (m_GageLength * 0.00009921875/*mm per micro step*/) * 100/*%*/);

        m_PreloadSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::OneStepEvent:
      {
        // Change to the one step event tab.
        m_Experiments->SetSelection(4);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get one step event parameters.
        std::shared_ptr<OneStepEvent> onestepevent = std::dynamic_pointer_cast<OneStepEvent>(m_CurrentProtocol->getEditExperiment());
        OneStepEventParameters parameters = onestepevent->getParametersForGUI();

        m_OneStepDistanceVelocitySpinCtrl->SetValue(parameters.velocity);
        m_OneStepStressForceVelocitySpinCtrl->SetValue(parameters.velocity);

        if(DistanceOrPercentage::Distance == parameters.velocityDistanceOrPercentage){
          m_OneStepStressForceVelocityMmRadioBtn->SetValue(true);
          m_OneStepStressForceVelocityPercentRadioBtn->SetValue(false);

          m_OneStepDistanceVelocityMmRadioBtn->SetValue(true);
          m_OneStepDistanceVelocityPercentRadioBtn->SetValue(false);
        }else if(DistanceOrPercentage::Percentage == parameters.velocityDistanceOrPercentage){
          m_OneStepStressForceVelocityMmRadioBtn->SetValue(false);
          m_OneStepStressForceVelocityPercentRadioBtn->SetValue(true);

          m_OneStepDistanceVelocityMmRadioBtn->SetValue(false);
          m_OneStepDistanceVelocityPercentRadioBtn->SetValue(true);
        }

        m_OneStepDistanceDelaySpinCtrl->SetValue(parameters.delay);
        m_OneStepStressForceDelaySpinCtrl->SetValue(parameters.delay);

        m_OneStepDistanceDwellSpinCtrl->SetValue(parameters.dwell);
        m_OneStepStressForceDwellSpinCtrl->SetValue(parameters.dwell);

        if(DistanceOrForceOrStress::Distance == parameters.distanceOrStressOrForce){
          m_OneStepStressForcePanel->Show(false);
          m_OneStepDistancePanel->Show(true);
          m_OneStepPanel21->Layout();

          m_OneStepStressForceRadioBtn->SetValue(false);
          m_OneStepDistanceRadioBtn->SetValue(true);

          m_OneStepDistanceLimitSpinCtrl->SetValue(parameters.limit);
          if(DistanceOrPercentage::DistanceRelative == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRadioBtn->SetValue(false);
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Distance == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitMmRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(false);
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Percentage == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRadioBtn->SetValue(false);
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(false);
          }

        }else{
          m_OneStepDistancePanel->Show(false);
          m_OneStepStressForcePanel->Show(true);
          m_OneStepPanel21->Layout();

          m_OneStepDistanceRadioBtn->SetValue(false);
          m_OneStepStressForceRadioBtn->SetValue(true);

          m_OneStepStressForceLimitSpinCtrl->SetValue(parameters.limit);
        }

        if(1 < parameters.cycles){
          m_OneStepEndOfEventRepeatCheckBox->SetValue(true);
          m_OneStepEndOfEventRepeatSpinCtrl->SetValue(parameters.cycles);
        }

        switch(parameters.behaviorAfterStop){
          case BehaviorAfterStop::Stop:
            m_OneStepEndOfEventStopRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::HoldADistance:
            m_OneStepEndOfEventHoldRadioBtn->SetValue(true);
            switch(parameters.holdDistanceOrPercentage){
              case DistanceOrPercentage::DistanceRelative:
                m_OneStepEndOfEventHoldMmRelRadioBtn->SetValue(true);
                m_OneStepEndOfEventHoldMmRadioBtn->SetValue(false);
                m_OneStepEndOfEventHoldPercentRadioBtn->SetValue(false);
                break;
              case DistanceOrPercentage::Distance:
                m_OneStepEndOfEventHoldMmRelRadioBtn->SetValue(false);
                m_OneStepEndOfEventHoldMmRadioBtn->SetValue(true);
                m_OneStepEndOfEventHoldPercentRadioBtn->SetValue(false);
                break;
              case DistanceOrPercentage::Percentage:
                m_OneStepEndOfEventHoldMmRelRadioBtn->SetValue(false);
                m_OneStepEndOfEventHoldMmRadioBtn->SetValue(false);
                m_OneStepEndOfEventHoldPercentRadioBtn->SetValue(true);
                break;
            }

            m_OneStepEndOfEventHoldSpinCtrl->SetValue(parameters.holdDistance);
            break;
          case BehaviorAfterStop::GoToL0:
            m_OneStepEndOfEventL0RadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToML:
            m_OneStepEndOfEventMLRadioBtn->SetValue(true);
            break;
        }

        m_OneStepSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::ContinuousEvent:
      {
        // Change to the continuous event tab.
        m_Experiments->SetSelection(5);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get continuous event parameters.
        std::shared_ptr<ContinuousEvent> continuousevent = std::dynamic_pointer_cast<ContinuousEvent>(m_CurrentProtocol->getEditExperiment());
        ContinuousEventParameters parameters = continuousevent->getParametersForGUI();

        m_ContinuousDistanceVelocitySpinCtrl->SetValue(parameters.velocity);
        m_ContinuousStressForceVelocitySpinCtrl->SetValue(parameters.velocity);

        m_ContinuousDistanceHoldTimeSpinCtrl->SetValue(parameters.holdtime);
        m_ContinuousStressForceHoldTimeSpinCtrl->SetValue(parameters.holdtime);

        if(DistanceOrPercentage::Distance == parameters.velocityDistanceOrPercentage){
          m_ContinuousDistanceVelocityMmRadioBtn->SetValue(true);
          m_ContinuousDistanceVelocityPercentRadioBtn->SetValue(false);

          m_ContinuousStressForceVelocityMmRadioBtn->SetValue(true);
          m_ContinuousStressForceVelocityPercentRadioBtn->SetValue(false);
        }else if(DistanceOrPercentage::Percentage == parameters.velocityDistanceOrPercentage){
          m_ContinuousDistanceVelocityMmRadioBtn->SetValue(false);
          m_ContinuousDistanceVelocityPercentRadioBtn->SetValue(true);

          m_ContinuousStressForceVelocityMmRadioBtn->SetValue(false);
          m_ContinuousStressForceVelocityPercentRadioBtn->SetValue(true);
        }

        if(DistanceOrForceOrStress::Distance == parameters.distanceOrStressOrForce){
          m_ContinuousStressForcePanel->Show(false);
          m_ContinuousDistancePanel->Show(true);
          m_ContinuousPanel21->Layout();

          m_ContinuousStressForceRadioBtn->SetValue(false);
          m_ContinuousDistanceRadioBtn->SetValue(true);

          m_ContinuousDistanceIncrementSpinCtrl->SetValue(parameters.increment);
          if(DistanceOrPercentage::Distance == parameters.incrementDistanceOrPercentage){
            m_ContinuousDistanceIncrementMmRadioBtn->SetValue(true);
            m_ContinuousDistanceIncrementPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Percentage == parameters.incrementDistanceOrPercentage){
            m_ContinuousDistanceIncrementPercentRadioBtn->SetValue(true);
            m_ContinuousDistanceIncrementMmRadioBtn->SetValue(false);
          }

          if(StepsOrMaxValue::MaxValue == parameters.stepsOrMaxValue){
            m_ContinuousDistanceStepsRadioBtn->SetValue(false);
            m_ContinuousDistanceMaxValueRadioBtn->SetValue(true);

            m_ContinuousDistanceMaxValueSpinCtrl->SetValue(parameters.maxvalue);
            if(DistanceOrPercentage::Distance == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(false);
            }else if(DistanceOrPercentage::DistanceRelative == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(false);
            }else if(DistanceOrPercentage::Percentage == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(false);
            }
          }else{
            m_ContinuousDistanceMaxValueRadioBtn->SetValue(false);
            m_ContinuousDistanceStepsRadioBtn->SetValue(true);

            m_ContinuousDistanceStepsSpinCtrl->SetValue(parameters.steps);
          }

        }else{
          m_ContinuousDistancePanel->Show(false);
          m_ContinuousStressForcePanel->Show(true);
          m_ContinuousPanel21->Layout();

          m_ContinuousDistanceRadioBtn->SetValue(false);
          m_ContinuousStressForceRadioBtn->SetValue(true);

          m_ContinuousStressForceIncrementSpinCtrl->SetValue(parameters.increment);

          if(StepsOrMaxValue::MaxValue == parameters.stepsOrMaxValue){
            m_ContinuousStressForceStepsRadioBtn->SetValue(false);
            m_ContinuousStressForceMaxValueRadioBtn->SetValue(true);

            m_ContinuousStressForceMaxValueSpinCtrl->SetValue(parameters.maxvalue);
            if(true == parameters.ramp2failure){
              m_ContinuousStressForceMaxValueValueRadioBtn->SetValue(false);
              m_ContinuousStressForceMaxValuePercentRadioBtn->SetValue(true);
            }else{
              m_ContinuousStressForceMaxValuePercentRadioBtn->SetValue(false);
              m_ContinuousStressForceMaxValueValueRadioBtn->SetValue(true);
            }
          }else{
            m_ContinuousStressForceMaxValueRadioBtn->SetValue(false);
            m_ContinuousStressForceStepsRadioBtn->SetValue(true);

            m_ContinuousStressForceStepsSpinCtrl->SetValue(parameters.steps);
          }

        }

        if(1 < parameters.cycles){
          m_ContinuousEndOfEventRepeatCheckBox->SetValue(true);
          m_ContinuousEndOfEventRepeatSpinCtrl->SetValue(parameters.cycles);
        }

        switch(parameters.behaviorAfterStop){
          case BehaviorAfterStop::Stop:
            m_ContinuousEndOfEventStopRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToL0:
            m_ContinuousEndOfEventL0RadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToML:
            m_ContinuousEndOfEventMLRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::HoldAForce:
            m_ContinuousEndOfEventHoldRadioBtn->SetValue(true);
            m_ContinuousEndOfEventHoldSpinCtrl->SetValue(parameters.stopAtForceStress);
            break;
        }
        m_ContinuousSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::Pause:
      // Get pause parameters
      std::shared_ptr<Pause> pause = std::dynamic_pointer_cast<Pause>(m_CurrentProtocol->getEditExperiment());
      double pausetime = pause->getParametersForGUI();

      std::unique_ptr<MyPauseDialog> dialog = std::unique_ptr<MyPauseDialog>(new MyPauseDialog(std::dynamic_pointer_cast<Pause>(m_CurrentProtocol->getEditExperiment()).get(),
                                                                                               pausetime));
      dialog->ShowModal();
      m_CurrentProtocol->updateEditedExperimentParameters();

      /*
      if(true == dialog->getCreateExperimentFlag()){
        //m_CurrentProtocol->addExperiment(experiment);
      }
      */
      break;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the delete experiment button.
 * @param event Occuring event
 */
void MyFrame::OnDeleteExperiment(wxCommandEvent& event){
  if(wxNOT_FOUND != m_ProtocolsListBox->GetSelection()){
    m_CurrentProtocol->removeExperiment(m_ProtocolsListBox->GetSelection());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment up button.
 * @param event Occuring event
 */
void MyFrame::OnMoveUpExperiment(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->moveExperimentUp(m_ProtocolsListBox->GetSelection());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment down button.
 * @param event Occuring event
 */
void MyFrame::OnMoveDownExperiment(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->moveExperimentDown(m_ProtocolsListBox->GetSelection());
  }
}
