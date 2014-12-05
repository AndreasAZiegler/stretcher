/**
 * @file myframe_graph.cpp
 * @brief The main frame graph part.
 * @author Andreas Ziegler
 */

// Includes
#include "myframe.h"
#include "protocols.h"
#include "myexportdialog.h"

/**
 * @brief Method wich will be executed, when the user changes the graph type.
 * @param event Occuring event
 */
void MyFrame::OnGraphChangeType(wxCommandEvent& event){
  // If the graph is active
  if(true == m_ShowGraphFlag){
    switch(m_GraphTypeComboBox->GetSelection()){
      case 0: /*Distance*/
        m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
        m_Graph->DelLayer(&m_ForceStressDistanceGraph);
        m_Graph->AddLayer(&m_ForceStressDistanceGraph);
        m_XAxis->SetName(wxT("Distance [mm]"));
        break;
      case 1: /*Displacement*/
        m_Graph->DelLayer(&m_ForceStressDistanceGraph);
        m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
        m_Graph->AddLayer(&m_ForceStressDisplacementGraph);
        m_XAxis->SetName(wxT("Displacement"));
        break;
      case 2: /*Distance and Displacmente*/
        m_Graph->DelLayer(&m_ForceStressDistanceGraph);
        m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
        m_Graph->AddLayer(&m_ForceStressDistanceGraph);
        m_Graph->AddLayer(&m_ForceStressDisplacementGraph);
        m_XAxis->SetName(wxT("Distance/Displacement [mm]"));
        break;
    }
    m_Graph->Fit();
  }
}

/**
 * @brief Method wich will be executed, when the user wants to see the limits in the graph.
 * @param event Occuring event
 */
void MyFrame::OnGraphShowLimits(wxCommandEvent& event){
  // If the graph is active
  if(true == m_ShowGraphFlag){
    if(true == m_GraphLimitsCheckBox->GetValue()){
      m_Graph->AddLayer(&m_MaxStressForceLimitGraph);
      m_Graph->AddLayer(&m_MinStressForceLimitGraph);
      m_Graph->AddLayer(&m_MaxDistanceLimitGraph);
      m_Graph->AddLayer(&m_MinDistanceLimitGraph);
    }else{
      m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
      m_Graph->DelLayer(&m_MinStressForceLimitGraph);
      m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
      m_Graph->DelLayer(&m_MinDistanceLimitGraph);
    }
    m_Graph->Fit();
  }
}

/**
 * @brief Prepares the graph to show the experiment values.
 */
void MyFrame::createValuesGraph(void){
  // Indicate, that graph is active.
  m_ShowGraphFlag = true;

  // Remove layers
  m_Graph->DelLayer(m_XAxis.get());
  m_Graph->DelLayer(m_Y1Axis.get());
  m_Graph->DelLayer(m_Y2Axis.get());
  m_Graph->DelLayer(&m_ForceStressDistanceGraph);
  m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
  m_Graph->DelLayer(&m_StressForcePreviewGraph);
  m_Graph->DelLayer(&m_DistancePreviewGraph);
  m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
  m_Graph->DelLayer(&m_MinStressForceLimitGraph);
  m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
  m_Graph->DelLayer(&m_MinDistanceLimitGraph);

  // Clear vectors
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_ForceStressDistanceGraph.Clear();
    m_ForceStressDisplacementGraph.Clear();
  }

  // Defines pen styles and font.
  wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
  wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);
  wxPen vectorpenLimit(*wxRED, 2, wxSOLID);
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

  // Set up and add axis.
  switch(m_GraphTypeComboBox->GetSelection()){
    case 0:
      m_XAxis.reset(new mpScaleX(wxT("Distance [mm]"), mpALIGN_BOTTOM, true, mpX_NORMAL));
      break;

    case 1:
      m_XAxis.reset(new mpScaleX(wxT("Displacement"), mpALIGN_BOTTOM, true, mpX_NORMAL));
      break;

    case 2:
      m_XAxis.reset(new mpScaleX(wxT("Distance/Displacement [mm]"), mpALIGN_BOTTOM, true, mpX_NORMAL));
      break;
  }

  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    m_Y1Axis.reset(new mpScaleY(wxT("Force [N]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
    m_ForceStressDistanceGraph.SetName("Force / Distance");
    m_ForceStressDisplacementGraph.SetName("Force / Displacement");
    m_MaxStressForceLimitGraph.SetName("Maximal force limit");
    m_MinStressForceLimitGraph.SetName("Minimal force limit");
  } else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    m_Y1Axis.reset(new mpScaleY(wxT("Stress [MPa]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
    m_ForceStressDistanceGraph.SetName("Stress / Distance");
    m_ForceStressDisplacementGraph.SetName("Stress / Displacement");
    m_MaxStressForceLimitGraph.SetName("Maximal stress limit");
    m_MinStressForceLimitGraph.SetName("Minimal stress limit");
  }
  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis.get());
  m_Graph->AddLayer(m_Y1Axis.get());

  // Add vectors according to the active graph type.
  switch(m_GraphTypeComboBox->GetSelection()){
    case 0: /*Distance*/
      m_Graph->AddLayer(&m_ForceStressDistanceGraph);
      break;
    case 1: /*Displacement*/
      m_Graph->AddLayer(&m_ForceStressDisplacementGraph);
      break;
    case 2: /*Distance and Displacmente*/
      m_Graph->AddLayer(&m_ForceStressDistanceGraph);
      m_Graph->AddLayer(&m_ForceStressDisplacementGraph);
      break;
  }

  if((DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress)){
    m_MaxStressForceLimitGraph.SetPen(vectorpenLimit);
    m_MinStressForceLimitGraph.SetPen(vectorpenLimit);
    m_ForceStressDistanceGraph.SetPen(vectorpenStressForce);
    m_ForceStressDisplacementGraph.SetPen(vectorpenStressForce);
  } else if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
    m_MaxDistanceLimitGraph.SetPen(vectorpenLimit);
    m_MinDistanceLimitGraph.SetPen(vectorpenLimit);
    m_ForceStressDistanceGraph.SetPen(vectorpenDistance);
    m_ForceStressDisplacementGraph.SetPen(vectorpenDistance);
  }
  if(true == m_GraphLimitsCheckBox->GetValue()){
    m_Graph->AddLayer(&m_MaxStressForceLimitGraph);
    m_Graph->AddLayer(&m_MinStressForceLimitGraph);
    m_Graph->AddLayer(&m_MaxDistanceLimitGraph);
    m_Graph->AddLayer(&m_MinDistanceLimitGraph);
  }

  // Update graph.
  m_Graph->Fit();
}

/**
 * @brief Method which will be called from the class ExperimentValues to update the graph. Executes updateGraph() from the main thread.
 */
void MyFrame::updateGraphFromExperimentValues(){
  std::lock_guard<std::mutex> lck{m_UpdateGraphMutex};
  CallAfter(&MyFrame::updateGraph);
}

/**
 * @brief Method which will be called from the class Protocols to create the preview graph. Executes createPreviewGraph() from the main thread.
 */
void MyFrame::showPreviewGraph(){
  CallAfter(&MyFrame::createPreviewGraph);
}

/**
 * @brief Updates the graph in the GUI.
 */
void MyFrame::updateGraph(void){
  m_Graph->Fit();
  //std::cout << "ExperimentVaues graph fitted." << std::endl;
}

/**
 * @brief Creates the preview graph.
 */
void MyFrame::createPreviewGraph(void){
  // Indicate, that graph is active.
  m_ShowGraphFlag = true;

  // Remove layers
  m_Graph->DelLayer(m_XAxis.get());
  m_Graph->DelLayer(m_Y1Axis.get());
  m_Graph->DelLayer(m_Y2Axis.get());
  m_Graph->DelLayer(&m_ForceStressDistanceGraph);
  m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
  m_Graph->DelLayer(&m_StressForcePreviewGraph);
  m_Graph->DelLayer(&m_DistancePreviewGraph);
  m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
  m_Graph->DelLayer(&m_MinStressForceLimitGraph);
  m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
  m_Graph->DelLayer(&m_MinDistanceLimitGraph);

  // Defines pen styles and font.
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
  wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);

  // Set up and add axis.
  m_XAxis.reset(new mpScaleX(wxT("Time"), mpALIGN_BOTTOM, true, mpX_NORMAL));
  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    m_Y1Axis.reset(new mpScaleY(wxT("Force [N]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
    m_Y1Axis->SetBrush(wxBrush(*wxBLUE_BRUSH));
    m_MaxStressForceLimitGraph.SetName("Maximal force limit");
    m_MinStressForceLimitGraph.SetName("Minimal force limit");
  } else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    m_Y1Axis.reset(new mpScaleY(wxT("Stress [MPa]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
    m_MaxStressForceLimitGraph.SetName("Maximal stress limit");
    m_MinStressForceLimitGraph.SetName("Minimal stress limit");
  }
  m_Y2Axis.reset(new mpScaleY(wxT("Distance [mm]"), mpALIGN_BORDER_RIGHT, true));
  m_Y2Axis->SetPen(vectorpenDistance);

  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_Y2Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);
  m_Y2Axis->SetDrawOutsideMargins(false);

  // Set up and add vectors.
  m_MaxStressForceLimitGraph.SetPen(vectorpenStressForce);
  m_MinStressForceLimitGraph.SetPen(vectorpenStressForce);
  m_MaxDistanceLimitGraph.SetPen(vectorpenDistance);
  m_MinDistanceLimitGraph.SetPen(vectorpenDistance);
  m_StressForcePreviewGraph.SetPen(vectorpenStressForce);
  m_DistancePreviewGraph.SetPen(vectorpenDistance);

  // Set margins and add layers.
  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis.get());
  m_Graph->AddLayer(m_Y1Axis.get());
  m_Graph->AddLayer(m_Y2Axis.get());
  m_Graph->AddLayer(&m_StressForcePreviewGraph);
  m_Graph->AddLayer(&m_DistancePreviewGraph);

  if(true == m_GraphLimitsCheckBox->GetValue()){
    m_Graph->AddLayer(&m_MaxStressForceLimitGraph);
    m_Graph->AddLayer(&m_MinStressForceLimitGraph);
    m_Graph->AddLayer(&m_MaxDistanceLimitGraph);
    m_Graph->AddLayer(&m_MinDistanceLimitGraph);
  }

  m_Graph->Fit();
}

/**
 * @brief Method wich will be executed, when the user clicks on the export csv button. Checks if there is any data to export and opens the export dialog if there is some data.
 * @param event Occuring event
 */
void MyFrame::OnExportCSV(wxCommandEvent& event){
  checkProtocol();
  // Checks if there is any data to export.
  if(true == m_CurrentProtocol->hasData()){
    std::unique_ptr<MyExportDialog> dialog = std::unique_ptr<MyExportDialog>(new MyExportDialog(m_CurrentProtocol, m_CurrentProtocol->getExperimentNames(), m_StoragePath));
    dialog->ShowModal();
  }
}

/**
 * @brief Shows the export dialog.
 */
void MyFrame::showExportCSVDialogFromProtocols(void){
  CallAfter(&MyFrame::showExportCSVDialog);
}

/**
 * @brief A message dialog asks the user if he/she wants to save the recorded data. If yes, the export dialog will show up.
 */
void MyFrame::showExportCSVDialog(void){
  // Ask if recorded data should be saved.
  std::unique_ptr<wxMessageDialog> dialog = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this, "The protocol is finished, do you want to save the recorded values?", wxMessageBoxCaptionStr, wxOK|wxCANCEL));
  // Show the export dialog if yes.
  if(wxID_OK == dialog->ShowModal()){
    std::unique_ptr<MyExportDialog> dialog = std::unique_ptr<MyExportDialog>(new MyExportDialog(m_CurrentProtocol, m_CurrentProtocol->getExperimentNames(), m_StoragePath));
    dialog->ShowModal();
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the export png button. Opens a file dialog and then saves the screen shot.
 * @param event Occuring event
 */
void MyFrame::OnExportPNG(wxCommandEvent& event){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));

  std::string pathAndFilename = m_StoragePath + "/" + "Graph_" + std::string(mbstr) + ".png";

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save png file"), "", "", "PNG files (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  saveFileDialog.SetPath(pathAndFilename);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }

  m_Graph->SaveScreenshot(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
  wxLogMessage(std::string("Graph saved in: " + saveFileDialog.GetPath()).c_str());
}

/**
 * @brief Method wich will be executed, when the user clicks on the clear graph button.
 * @param event Occuring event
 */
void MyFrame::OnClearGraph(wxCommandEvent& event){
  m_CurrentProtocol->clearGraphStop();
  /*
  {
    std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
    if(true == m_MeasurementValuesRecordingFlag){
      m_MeasurementValuesRecordingFlag = false;
      m_CurrentExperimentValues->stopMeasurement();
    }
  }
      */
  //m_Graph->DelLayer(&m_VectorLayer);
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_ForceStressDistanceGraph.Clear();
    m_ForceStressDisplacementGraph.Clear();
  }
  wxLogMessage("Cleared graph.");
  m_Graph->Fit();
  //delete m_CurrentExperimentValues;
}
