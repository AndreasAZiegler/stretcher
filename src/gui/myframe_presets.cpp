/**
 * @file myframe_presets.cpp
 * @brief The main frame preset part.
 * @author Andreas Ziegler
 */

// Includes
#include "myframe.h"
#include <libconfig.h++>

// Namespaces
using namespace std;
using namespace libconfig;

/**
 * @brief Method which will be executed, when the user click on the file picker to load a preset.
 * @param event Occuring event
 */
void MyFrame::OnLoadPreset(wxFileDirPickerEvent& event){
  std::string filename = m_InitializePresetFilePicker->GetPath().ToStdString();

  // Check if file exists
  if(FILE *file = fopen(filename.c_str(), "r")){
    fclose(file);

    Config config;
    try{
      config.readFile(filename.c_str());
    }catch(const FileIOException &fioex){
      std::cerr << "I/O error while reading preset file." << std::endl;
      //return(false);
    }catch(const ParseException &pex){
      std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
    }

    // Get the root settings.
    Setting &rootsettings = config.getRoot();
    Setting &settings = rootsettings["Settings"];

    // Read parameters
    try{
      settings.lookupValue("MaxPosDistance", m_TempMaxPosDistance);
      settings.lookupValue("MountingLength", m_TempMountingLength);
      settings.lookupValue("GageLength", m_TempGageLength);
      settings.lookupValue("MinDistanceLimit", m_TempMinDistanceLimit);
      settings.lookupValue("MaxDistanceLimit", m_TempMaxDistanceLimit);
      settings.lookupValue("MinForceLimit", m_TempMinForceLimit);
      settings.lookupValue("MaxForceLimit", m_TempMaxForceLimit);
      settings.lookupValue("ForceStressSensitivity", m_TempForceStressSensitivity);
      settings.lookupValue("DistanceSensitivity", m_TempDistanceSensitivity);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Displays the values of the preset in the GUI.
    m_InitializeMaxPosShowStaticText->SetLabelText(to_string_wp(m_TempMaxPosDistance, 2));
    m_InitializeMountingLengthShowStaticText->SetLabelText(to_string_wp(m_TempMountingLength, 2));
    m_InitializeMinDistanceShowStaticText->SetLabelText(to_string_wp(m_TempMinDistanceLimit, 2));
    m_InitializeMaxDistanceShowStaticText->SetLabelText(to_string_wp(m_TempMaxDistanceLimit, 2));
    m_InitializeMinForceShowStaticText->SetLabelText(to_string_wp(m_TempMinForceLimit, 2));
    m_InitializeMaxForceShowStaticText->SetLabelText(to_string_wp(m_TempMaxForceLimit, 2));
    m_InitializeForceStressSensitivityShowStaticText->SetLabelText(to_string_wp(m_TempForceStressSensitivity, 2));
    m_InitializeDistanceSensitivityShowStaticText->SetLabelText(to_string_wp(m_TempDistanceSensitivity, 2));
  }
}

/**
 * @brief Method which will be executed, when the user applies a preset.
 * @param event Occuring event
 */
void MyFrame::OnApplyPreset(wxCommandEvent& event){

  if(1 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = m_TempMaxForceLimit * 10000.0;
    m_MinForceLimit = m_TempMinForceLimit * 10000.0;
  } else if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = (m_TempMaxForceLimit * m_InitializeCrossSectionSpinCtrl->GetValue() / 1000) * 10000.0;
    m_MinForceLimit = (m_TempMinForceLimit * m_InitializeCrossSectionSpinCtrl->GetValue() / 1000) * 10000.0;
  }

  // Applies parameters of the preset.
  m_StageFrame->setMaxDistanceLimitMM(m_TempMaxDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_StageFrame->setMinDistanceLimitMM(m_TempMinDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_MaxDistanceLimit = m_TempMaxDistanceLimit / 0.00009921875/*mm per micro step*/;
  m_MinDistanceLimit = m_TempMinDistanceLimit / 0.00009921875/*mm per micro step*/;
  m_MaxPosDistance = m_TempMaxPosDistance / 0.00009921875/*mm per micro step*/;
  m_MountingLength = m_TempMountingLength / 0.00009921875/*mm per micro step*/;
  m_GageLength = m_TempGageLength / 0.00009921875/*mm per micro step*/;
  m_ForceStressSensitivity = m_TempForceStressSensitivity * 10000.0;
  m_DistanceSensitivity = m_TempDistanceSensitivity / 0.00009921875/*mm per micro step*/;

  m_StageFrame->setMaxPosDistance(m_MaxPosDistance);
}

/**
 * @brief Method which will be executed, when the user saves a preset. Opens a file dialog and then saves the preset.
 * @param event Occuring event
 */
void MyFrame::OnSavePreset(wxCommandEvent& event){

  wxFileDialog saveFileDialog(this, _("Save current parameters as preset"), "", "", "Preset files (*.cfg)|*.cfg", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

  if(saveFileDialog.ShowModal() == wxID_CANCEL){
    return;
  }

  // Save the parameters in the file.
  Config config;
  Setting &rootsettings = config.getRoot();

  Setting &settings = rootsettings.add("Settings", Setting::TypeGroup);

  settings.add("MaxPosDistance", Setting::TypeFloat) = m_MaxPosDistance * 0.00009921875/*mm per micro step*/;
  settings.add("MountingLength", Setting::TypeFloat) = m_MountingLength * 0.00009921875/*mm per micro step*/;
  settings.add("GageLength", Setting::TypeFloat) = m_GageLength * 0.00009921875/*mm per micro step*/;
  settings.add("MinDistanceLimit", Setting::TypeFloat) = m_MinDistanceLimit * 0.00009921875/*mm per micro step*/;
  settings.add("MaxDistanceLimit", Setting::TypeFloat) = m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/;
  settings.add("MinForceLimit", Setting::TypeFloat) = m_MinForceLimit / 10000.0;
  settings.add("MaxForceLimit", Setting::TypeFloat) = m_MaxForceLimit / 10000.0;
  settings.add("ForceStressSensitivity", Setting::TypeFloat) = m_ForceStressSensitivity / 10000.0;
  settings.add("DistanceSensitivity", Setting::TypeFloat) = m_DistanceSensitivity  * 0.00009921875/*mm per micro step*/;

  try{
    config.writeFile(saveFileDialog.GetPath().c_str());
    wxLogMessage(std::string("MyFrame:: Preset successfully written to: " + saveFileDialog.GetPath()).c_str());
  }catch(const FileIOException &fioex){
      std::cerr << "I/O error while saving preset file." << std::endl;
  }

}
