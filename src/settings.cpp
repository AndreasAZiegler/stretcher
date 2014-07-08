/**
 * @file settings.cpp
 * @brief The protocol.
 * @author Andreas Ziegler
 */

// Includes
#include <memory>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "settings.h"

// Namespaces
using namespace std;
using namespace libconfig;

/**
 * @brief Load settings if file exists.
 */
Settings::Settings()
  : //s_ConfigurationFileName("config.cfg"),
    m_ConfigurationStoragePath("config.cfg"),
    m_RootSettings(NULL),
    m_LinMot1Settings(NULL),
    m_LinMot2Settings(NULL),
    m_ForceSensorSettings(NULL),
    m_LinMot1ComPortSetting(NULL),
    m_LinMot2ComPortSetting(NULL),
    m_LinMot1BaudRateSetting(NULL),
    m_LinMot2BaudRateSetting(NULL),
    m_ForceSensorComPortSetting(NULL),
    m_StoragePathsSetting(NULL),
    m_ConfigurationStoragePathSetting(NULL),
    m_MeasurementValuesStoragePathSetting(NULL),
    m_LinMot1ComPort("/dev/ttyUSB0"),
    m_LinMot2ComPort("/dev/ttyUSB1"),
    m_LinMot1BaudRate(115200),
    m_LinMot2BaudRate(115200),
    m_ForceSensorComPort("/dev/ttyUSB2"),
    m_ForceSensorBaudRate(115200),
    m_ForceSensorScalingFactor(161380.83),
    m_ForceSensorNominalForce(20.0),
    m_ForceSensorNominalValue(0.4965),
    m_ForceSensorInputSensitivity(1.0),
    m_ForceSensorMeasureEndValue(1597.83),
    m_ForceSensorZeroValue(8388608.0),
    m_MeasurementValuesStoragePath(std::string("/home")),
    m_Set1Name(std::string("")),
    m_Set1MaxDistance(0),
    m_Set1MinDistance(0),
    m_Set1MaxForce(0),
    m_Set1MinForce(0),
    m_Set2Name(std::string("")),
    m_Set2MaxDistance(0),
    m_Set2MinDistance(0),
    m_Set2MaxForce(0),
    m_Set2MinForce(0),
    m_Set3Name(std::string("")),
    m_Set3MaxDistance(0),
    m_Set3MinDistance(0),
    m_Set3MaxForce(0),
    m_Set3MinForce(0),
    m_Set4Name(std::string("")),
    m_Set4MaxDistance(0),
    m_Set4MinDistance(0),
    m_Set4MaxForce(0),
    m_Set4MinForce(0),
    m_MaxPosDistance(0),
    m_MountingLength(0),
    m_GageLength(0),
    m_MinDistanceLimit(0),
    m_MaxDistanceLimit(0),
    m_MinForceLimit(0),
    m_MaxForceLimit(0)
{
  readSettings();
}

Settings::~Settings(){
  writeSettings();
}

/**
 * @brief Trys to read the settings from the configuration file.
 * @return true if sucessfull false otherwise.
 */
bool Settings::readSettings ()
{
  // Check if file exists
  //if(FILE *file = fopen(s_ConfigurationFileName, "r")){
  if(FILE *file = fopen(m_ConfigurationStoragePath.c_str(), "r")){
    fclose(file);

    // Open the configuration file.
    try{
      m_CurrentConfig.readFile(m_ConfigurationStoragePath.c_str());
    }catch(const FileIOException &fioex){
      std::cerr << "I/O error while reading configuration file." << std::endl;
      return(false);
    }catch(const ParseException &pex){
      std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
    }

    // Get the root settings
    m_RootSettings = &m_CurrentConfig.getRoot();
    //m_RootSettings.reset(&m_CurrentConfig.getRoot());

    // Read the settings for storage paths.
    try{
      m_StoragePathsSetting = &m_RootSettings->operator []("StoragePaths");
      m_StoragePathsSetting->lookupValue("ConfigurationStoragePath", m_ConfigurationStoragePath);
      m_StoragePathsSetting->lookupValue("MeasurementStoragePath", m_MeasurementValuesStoragePath);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for linear stage 1
    try{
      m_LinMot1Settings = &m_RootSettings->operator []("LinearMotor1");
      m_LinMot1Settings->lookupValue("ComPort", m_LinMot1ComPort);
      m_LinMot1Settings->lookupValue("Baudrate", m_LinMot1BaudRate);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for linear stage 2
    try{
      m_LinMot2Settings = &m_RootSettings->operator []("LinearMotor2");
      m_LinMot2Settings->lookupValue("ComPort", m_LinMot2ComPort);
      m_LinMot2Settings->lookupValue("Baudrate", m_LinMot2BaudRate);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for the force sensor
    try{
      m_ForceSensorSettings = &m_RootSettings->operator []("ForceSensor");
      m_ForceSensorSettings->lookupValue("ComPort", m_ForceSensorComPort);
      m_ForceSensorSettings->lookupValue("Baudrate", m_ForceSensorBaudRate);

      m_ForceSensorSettings->lookupValue("ScalingFactor", m_ForceSensorScalingFactor);
      m_ForceSensorSettings->lookupValue("NominalForce", m_ForceSensorNominalForce);
      m_ForceSensorSettings->lookupValue("NominalValue", m_ForceSensorNominalValue);
      m_ForceSensorSettings->lookupValue("InputSensitivity", m_ForceSensorInputSensitivity);
      m_ForceSensorSettings->lookupValue("MeasureEndValue", m_ForceSensorMeasureEndValue);
      m_ForceSensorSettings->lookupValue("ZeroValue", m_ForceSensorZeroValue);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for the limit set 1
    try{
      m_Set1Settings = &m_RootSettings->operator []("Set1");

      m_Set1Settings->lookupValue("Name", m_Set1Name);
      m_Set1Settings->lookupValue("MaxDistance", m_Set1MaxDistance);
      m_Set1Settings->lookupValue("MinDistance", m_Set1MinDistance);
      m_Set1Settings->lookupValue("MaxForce", m_Set1MaxForce);
      m_Set1Settings->lookupValue("MinForce", m_Set1MinForce);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for the limit set 2
    try{
      m_Set2Settings = &m_RootSettings->operator []("Set2");

      m_Set2Settings->lookupValue("Name", m_Set2Name);
      m_Set2Settings->lookupValue("MaxDistance", m_Set2MaxDistance);
      m_Set2Settings->lookupValue("MinDistance", m_Set2MinDistance);
      m_Set2Settings->lookupValue("MaxForce", m_Set2MaxForce);
      m_Set2Settings->lookupValue("MinForce", m_Set2MinForce);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for the limit set 3
    try{
      m_Set3Settings = &m_RootSettings->operator []("Set3");

      m_Set3Settings->lookupValue("Name", m_Set3Name);
      m_Set3Settings->lookupValue("MaxDistance", m_Set3MaxDistance);
      m_Set3Settings->lookupValue("MinDistance", m_Set3MinDistance);
      m_Set3Settings->lookupValue("MaxForce", m_Set3MaxForce);
      m_Set3Settings->lookupValue("MinForce", m_Set3MinForce);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the settings for the limit set 4
    try{
      m_Set4Settings = &m_RootSettings->operator []("Set4");

      m_Set4Settings->lookupValue("Name", m_Set4Name);
      m_Set4Settings->lookupValue("MaxDistance", m_Set4MaxDistance);
      m_Set4Settings->lookupValue("MinDistance", m_Set4MinDistance);
      m_Set4Settings->lookupValue("MaxForce", m_Set4MaxForce);
      m_Set4Settings->lookupValue("MinForce", m_Set4MinForce);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    // Read the start up settings.
    try{
      m_StartUpSettings = &m_RootSettings->operator []("StartUp");

      m_StartUpSettings->lookupValue("MaxPosDistance", m_MaxPosDistance);
      m_StartUpSettings->lookupValue("MountingLength", m_MountingLength);
      m_StartUpSettings->lookupValue("GageLength", m_GageLength);
      m_StartUpSettings->lookupValue("MinDistanceLimit", m_MinDistanceLimit);
      m_StartUpSettings->lookupValue("MaxDistanceLimit", m_MaxDistanceLimit);
      m_StartUpSettings->lookupValue("MinForceLimit", m_MinForceLimit);
      m_StartUpSettings->lookupValue("MaxForceLimit", m_MaxForceLimit);
    }catch(const SettingNotFoundException &nfex){
      std::cerr << "Setting " << nfex.getPath() << " not found." << std::endl;
    }

    return(true);
  }

  return(false);
}

/**
 * @brief Saves the settings in the configuration file.
 * @return true if sucessfull false otherwise.
 */
bool Settings::writeSettings(){
  if(NULL == m_RootSettings){
    //m_RootSettings.reset(&m_CurrentConfig.getRoot());
    m_RootSettings = &m_CurrentConfig.getRoot();
  }

  // Write the settings for storage paths
  if(m_RootSettings->exists("StoragePaths")){
    m_StoragePathsSetting = &m_RootSettings->operator []("StoragePaths");
  }else{
    m_StoragePathsSetting = &m_RootSettings->add("StoragePaths", Setting::TypeGroup);
  }

  // Writing configuration file storage path setting for storage paths
  if(m_StoragePathsSetting->exists("ConfigurationStoragePath")){
    m_ConfigurationStoragePathSetting = &m_StoragePathsSetting->operator []("ConfigurationStoragePath");
    *m_ConfigurationStoragePathSetting = m_ConfigurationStoragePath;
  }else{
    m_StoragePathsSetting->add("ConfigurationStoragePath", Setting::TypeString) = m_ConfigurationStoragePath;
  }

  // Writing measurement values storage path setting for storage paths
  if(m_StoragePathsSetting->exists("MeasurementStoragePath")){
    m_MeasurementValuesStoragePathSetting = &m_StoragePathsSetting->operator []("MeasurementStoragePath");
    *m_MeasurementValuesStoragePathSetting = m_MeasurementValuesStoragePath;
  }else{
    m_StoragePathsSetting->add("MeasurementStoragePath", Setting::TypeString) = m_MeasurementValuesStoragePath;
  }

  // Write the settings for linear motor 1
  if(m_RootSettings->exists("LinearMotor1")){
    m_LinMot1Settings = &m_RootSettings->operator []("LinearMotor1");
  }else{
    m_LinMot1Settings = &m_RootSettings->add("LinearMotor1", Setting::TypeGroup);
  }

  // Writing com port setting for linear stage 1
  if(m_LinMot1Settings->exists("ComPort")){
    m_LinMot1ComPortSetting = &m_LinMot1Settings->operator []("ComPort");
    *m_LinMot1ComPortSetting = m_LinMot1ComPort;
  }else{
    m_LinMot1Settings->add("ComPort", Setting::TypeString) = m_LinMot1ComPort;
  }

  // Writing baud rate setting for linear stage 1
  if(m_LinMot1Settings->exists("Baudrate")){
    m_LinMot1BaudRateSetting = &m_LinMot1Settings->operator []("Baudrate");
    *m_LinMot1BaudRateSetting = m_LinMot1BaudRate;
  }else{
    m_LinMot1Settings->add("Baudrate", Setting::TypeInt) = m_LinMot1BaudRate;
  }

  // Write the settings for linear motor 2
  if(m_RootSettings->exists("LinearMotor2")){
    m_LinMot2Settings = &(m_RootSettings->operator []("LinearMotor2"));
  }else{
    m_LinMot2Settings = &m_RootSettings->add("LinearMotor2", Setting::TypeGroup);
  }
  // Writing com port setting for linear stage 2
  if(m_LinMot2Settings->exists("ComPort")){
    m_LinMot2ComPortSetting = &m_LinMot2Settings->operator []("ComPort");
    *m_LinMot2ComPortSetting = m_LinMot2ComPort;
  }else{
    m_LinMot2Settings->add("ComPort", Setting::TypeString) = m_LinMot2ComPort;
  }

  // Writing baud rate setting for linear stage 2
  if(m_LinMot2Settings->exists("Baudrate")){
    m_LinMot2BaudRateSetting = &m_LinMot2Settings->operator []("Baudrate");
    *m_LinMot2BaudRateSetting = m_LinMot2BaudRate;
  }else{
    m_LinMot2Settings->add("Baudrate", Setting::TypeInt) = m_LinMot2BaudRate;
  }

  // Writing the settings for the force sensor
  if(m_RootSettings->exists("ForceSensor")){
    m_ForceSensorSettings = &m_RootSettings->operator []("ForceSensor");
  }else{
    m_ForceSensorSettings = &m_RootSettings->add("ForceSensor", Setting::TypeGroup);
  }

  // Writing com port setting for force sensor
  if(m_ForceSensorSettings->exists("ComPort")){
    m_ForceSensorComPortSetting = &m_ForceSensorSettings->operator []("ComPort");
    *m_ForceSensorComPortSetting = m_ForceSensorComPort;
  }else{
    m_ForceSensorSettings->add("ComPort", Setting::TypeString) = m_ForceSensorComPort;
  }

  // Writing baud rate setting for force sensor
  if(m_ForceSensorSettings->exists("Baudrate")){
    m_ForceSensorBaudRateSetting = &m_ForceSensorSettings->operator []("Baudrate");
    *m_ForceSensorBaudRateSetting = m_ForceSensorBaudRate;
  }else{
    m_ForceSensorSettings->add("Baudrate", Setting::TypeInt) = m_ForceSensorBaudRate;
  }

  // Writing scaling factor for force sensor
  if(m_ForceSensorSettings->exists("ScalingFactor")){
    m_ForceSensorScalingFactorSetting = &m_ForceSensorSettings->operator []("ScalingFactor");
    *m_ForceSensorScalingFactorSetting = m_ForceSensorScalingFactor;
  }else{
    m_ForceSensorSettings->add("ScalingFactor", Setting::TypeFloat) = m_ForceSensorScalingFactor;
  }

  // Writing nominal force for force sensor
  if(m_ForceSensorSettings->exists("NominalForce")){
    m_ForceSensorNominalForceSetting = &m_ForceSensorSettings->operator []("NominalForce");
    *m_ForceSensorNominalForceSetting = m_ForceSensorNominalForce;
  }else{
    m_ForceSensorSettings->add("NominalForce", Setting::TypeFloat) = m_ForceSensorNominalForce;
  }

  // Writing nominal value for force sensor
  if(m_ForceSensorSettings->exists("NominalValue")){
    m_ForceSensorNominalValueSetting = &m_ForceSensorSettings->operator []("NominalValue");
    *m_ForceSensorNominalValueSetting = m_ForceSensorNominalValue;
  }else{
    m_ForceSensorSettings->add("NominalValue", Setting::TypeFloat) = m_ForceSensorNominalValue;
  }

  // Writing input sensitivity for force sensor
  if(m_ForceSensorSettings->exists("InputSensitivity")){
    m_ForceSensorInputSensitivitySetting = &m_ForceSensorSettings->operator []("InputSensitivity");
    *m_ForceSensorInputSensitivitySetting = m_ForceSensorInputSensitivity;
  }else{
    m_ForceSensorSettings->add("InputSensitivity", Setting::TypeFloat) = m_ForceSensorInputSensitivity;
  }

  // Writing measure end value for force sensor
  if(m_ForceSensorSettings->exists("MeasureEndValue")){
    m_ForceSensorMeasureEndValueSetting = &m_ForceSensorSettings->operator []("MeasureEndValue");
    *m_ForceSensorMeasureEndValueSetting = m_ForceSensorMeasureEndValue;
  }else{
    m_ForceSensorSettings->add("MeasureEndValue", Setting::TypeFloat) = m_ForceSensorMeasureEndValue;
  }

  // Writing zero value for force sensor
  if(m_ForceSensorSettings->exists("ZeroValue")){
    m_ForceSensorZeroValueSetting = &m_ForceSensorSettings->operator []("ZeroValue");
    *m_ForceSensorZeroValueSetting = m_ForceSensorZeroValue;
  }else{
    m_ForceSensorSettings->add("ZeroValue", Setting::TypeFloat) = m_ForceSensorZeroValue;
  }

  // Writing the settings for the limit 1 settings
  if(m_RootSettings->exists("Set1")){
    m_Set1Settings = &m_RootSettings->operator []("Set1");
  }else{
    m_Set1Settings = &m_RootSettings->add("Set1", Setting::TypeGroup);
  }

  // Writing name setting for limit set 1
  if(m_Set1Settings->exists("Name")){
    m_Set1NameSetting = &m_Set1Settings->operator []("Name");
    *m_Set1NameSetting = m_Set1Name;
  }else{
    m_Set1Settings->add("Name", Setting::TypeString) = m_Set1Name;
  }

  // Writing max distance setting for limit set 1
  if(m_Set1Settings->exists("MaxDistance")){
    m_Set1MaxDistanceSetting = &m_Set1Settings->operator []("MaxDistance");
    *m_Set1MaxDistanceSetting = m_Set1MaxDistance;
  }else{
    m_Set1Settings->add("MaxDistance", Setting::TypeInt) = m_Set1MaxDistance;
  }

  // Writing min distance setting for limit set 1
  if(m_Set1Settings->exists("MinDistance")){
    m_Set1MinDistanceSetting = &m_Set1Settings->operator []("MinDistance");
    *m_Set1MinDistanceSetting = m_Set1MinDistance;
  }else{
    m_Set1Settings->add("MinDistance", Setting::TypeInt) = m_Set1MinDistance;
  }

  // Writing max force setting for limit set 1
  if(m_Set1Settings->exists("MaxForce")){
    m_Set1MaxForceSetting = &m_Set1Settings->operator []("MaxForce");
    *m_Set1MaxForceSetting = m_Set1MaxForce;
  }else{
    m_Set1Settings->add("MaxForce", Setting::TypeInt) = m_Set1MaxForce;
  }

  // Writing min force setting for limit set 1
  if(m_Set1Settings->exists("MinForce")){
    m_Set1MinForceSetting = &m_Set1Settings->operator []("MinForce");
    *m_Set1MinForceSetting = m_Set1MinForce;
  }else{
    m_Set1Settings->add("MinForce", Setting::TypeInt) = m_Set1MinForce;
  }

  // Writing the settings for the limit 2 settings
  if(m_RootSettings->exists("Set2")){
    m_Set2Settings = &m_RootSettings->operator []("Set2");
  }else{
    m_Set2Settings = &m_RootSettings->add("Set2", Setting::TypeGroup);
  }

  // Writing name setting for limit set 2
  if(m_Set2Settings->exists("Name")){
    m_Set2NameSetting = &m_Set2Settings->operator []("Name");
    *m_Set2NameSetting = m_Set2Name;
  }else{
    m_Set2Settings->add("Name", Setting::TypeString) = m_Set2Name;
  }

  // Writing max distance setting for limit set 2
  if(m_Set2Settings->exists("MaxDistance")){
    m_Set2MaxDistanceSetting = &m_Set2Settings->operator []("MaxDistance");
    *m_Set2MaxDistanceSetting = m_Set2MaxDistance;
  }else{
    m_Set2Settings->add("MaxDistance", Setting::TypeInt) = m_Set2MaxDistance;
  }

  // Writing min distance setting for limit set 2
  if(m_Set2Settings->exists("MinDistance")){
    m_Set2MinDistanceSetting = &m_Set2Settings->operator []("MinDistance");
    *m_Set2MinDistanceSetting = m_Set2MinDistance;
  }else{
    m_Set2Settings->add("MinDistance", Setting::TypeInt) = m_Set2MinDistance;
  }

  // Writing max force setting for limit set 2
  if(m_Set2Settings->exists("MaxForce")){
    m_Set2MaxForceSetting = &m_Set2Settings->operator []("MaxForce");
    *m_Set2MaxForceSetting = m_Set2MaxForce;
  }else{
    m_Set2Settings->add("MaxForce", Setting::TypeInt) = m_Set2MaxForce;
  }

  // Writing min force setting for limit set 2
  if(m_Set2Settings->exists("MinForce")){
    m_Set2MinForceSetting = &m_Set2Settings->operator []("MinForce");
    *m_Set2MinForceSetting = m_Set2MinForce;
  }else{
    m_Set2Settings->add("MinForce", Setting::TypeInt) = m_Set2MinForce;
  }

  // Writing the settings for the limit 3 settings
  if(m_RootSettings->exists("Set3")){
    m_Set3Settings = &m_RootSettings->operator []("Set3");
  }else{
    m_Set3Settings = &m_RootSettings->add("Set3", Setting::TypeGroup);
  }

  // Writing name setting for limit set 3
  if(m_Set3Settings->exists("Name")){
    m_Set3NameSetting = &m_Set3Settings->operator []("Name");
    *m_Set3NameSetting = m_Set3Name;
  }else{
    m_Set3Settings->add("Name", Setting::TypeString) = m_Set3Name;
  }

  // Writing max distance setting for limit set 3
  if(m_Set3Settings->exists("MaxDistance")){
    m_Set3MaxDistanceSetting = &m_Set3Settings->operator []("MaxDistance");
    *m_Set3MaxDistanceSetting = m_Set3MaxDistance;
  }else{
    m_Set3Settings->add("MaxDistance", Setting::TypeInt) = m_Set3MaxDistance;
  }

  // Writing min distance setting for limit set 3
  if(m_Set3Settings->exists("MinDistance")){
    m_Set3MinDistanceSetting = &m_Set3Settings->operator []("MinDistance");
    *m_Set3MinDistanceSetting = m_Set3MinDistance;
  }else{
    m_Set3Settings->add("MinDistance", Setting::TypeInt) = m_Set3MinDistance;
  }

  // Writing max force setting for limit set 3
  if(m_Set3Settings->exists("MaxForce")){
    m_Set3MaxForceSetting = &m_Set3Settings->operator []("MaxForce");
    *m_Set3MaxForceSetting = m_Set3MaxForce;
  }else{
    m_Set3Settings->add("MaxForce", Setting::TypeInt) = m_Set3MaxForce;
  }

  // Writing min force setting for limit set 3
  if(m_Set3Settings->exists("MinForce")){
    m_Set3MinForceSetting = &m_Set3Settings->operator []("MinForce");
    *m_Set3MinForceSetting = m_Set3MinForce;
  }else{
    m_Set3Settings->add("MinForce", Setting::TypeInt) = m_Set3MinForce;
  }

  // Writing the settings for the limit 4 settings
  if(m_RootSettings->exists("Set4")){
    m_Set4Settings = &m_RootSettings->operator []("Set4");
  }else{
    m_Set4Settings = &m_RootSettings->add("Set4", Setting::TypeGroup);
  }

  // Writing name setting for limit set 4
  if(m_Set4Settings->exists("Name")){
    m_Set4NameSetting = &m_Set4Settings->operator []("Name");
    *m_Set4NameSetting = m_Set4Name;
  }else{
    m_Set4Settings->add("Name", Setting::TypeString) = m_Set4Name;
  }

  // Writing max distance setting for limit set 4
  if(m_Set4Settings->exists("MaxDistance")){
    m_Set4MaxDistanceSetting = &m_Set4Settings->operator []("MaxDistance");
    *m_Set4MaxDistanceSetting = m_Set4MaxDistance;
  }else{
    m_Set4Settings->add("MaxDistance", Setting::TypeInt) = m_Set4MaxDistance;
  }

  // Writing min distance setting for limit set 4
  if(m_Set4Settings->exists("MinDistance")){
    m_Set4MinDistanceSetting = &m_Set4Settings->operator []("MinDistance");
    *m_Set4MinDistanceSetting = m_Set4MinDistance;
  }else{
    m_Set4Settings->add("MinDistance", Setting::TypeInt) = m_Set4MinDistance;
  }

  // Writing max force setting for limit set 4
  if(m_Set4Settings->exists("MaxForce")){
    m_Set4MaxForceSetting = &m_Set4Settings->operator []("MaxForce");
    *m_Set4MaxForceSetting = m_Set4MaxForce;
  }else{
    m_Set4Settings->add("MaxForce", Setting::TypeInt) = m_Set4MaxForce;
  }

  // Writing min force setting for limit set 4
  if(m_Set4Settings->exists("MinForce")){
    m_Set4MinForceSetting = &m_Set4Settings->operator []("MinForce");
    *m_Set4MinForceSetting = m_Set4MinForce;
  }else{
    m_Set4Settings->add("MinForce", Setting::TypeInt) = m_Set4MinForce;
  }

  // Writing the start up settings.
  if(m_RootSettings->exists("StartUp")){
    m_StartUpSettings = &m_RootSettings->operator []("StartUp");
  }else{
    m_StartUpSettings = &m_RootSettings->add("StartUp", Setting::TypeGroup);
  }

  // Writing the maximum position distance setting.
  if(m_StartUpSettings->exists("MaxPosDistance")){
    m_MaxPosDistanceSetting = &m_StartUpSettings->operator []("MaxPosDistance");
    *m_MaxPosDistanceSetting = m_MaxPosDistance;
  }else{
    m_StartUpSettings->add("MaxPosDistance", Setting::TypeInt64) = m_MaxPosDistance;
  }

  // Writing the mounting length setting.
  if(m_StartUpSettings->exists("MountingLength")){
    m_MountingLengthSetting = &m_StartUpSettings->operator []("MountingLength");
    *m_MountingLengthSetting = m_MountingLength;
  }else{
    m_StartUpSettings->add("MountingLength", Setting::TypeInt64) = m_MountingLength;
  }

  // Writing the gage length setting.
  if(m_StartUpSettings->exists("GageLength")){
    m_GageLengthSetting = &m_StartUpSettings->operator []("GageLength");
    *m_GageLengthSetting = m_GageLength;
  }else{
    m_StartUpSettings->add("GageLength", Setting::TypeInt64) = m_GageLength;
  }

  // Writing the minimum distance limit setting.
  if(m_StartUpSettings->exists("MinDistanceLimit")){
    m_MinDistanceLimitSetting = &m_StartUpSettings->operator []("MinDistanceLimit");
    *m_MinDistanceLimitSetting = m_MinDistanceLimit;
  }else{
    m_StartUpSettings->add("MinDistanceLimit", Setting::TypeInt64) = m_MinDistanceLimit;
  }

  // Writing the maximum distance limit setting.
  if(m_StartUpSettings->exists("MaxDistanceLimit")){
    m_MaxDistanceLimitSetting = &m_StartUpSettings->operator []("MaxDistanceLimit");
    *m_MaxDistanceLimitSetting = m_MaxDistanceLimit;
  }else{
    m_StartUpSettings->add("MaxDistanceLimit", Setting::TypeInt64) = m_MaxDistanceLimit;
  }

  // Writing the minimum force limit setting.
  if(m_StartUpSettings->exists("MinForceLimit")){
    m_MinForceLimitSetting = &m_StartUpSettings->operator []("MinForceLimit");
    *m_MinForceLimitSetting = m_MinForceLimit;
  }else{
    m_StartUpSettings->add("MinForceLimit", Setting::TypeInt64) = m_MinForceLimit;
  }

  // Writing the maximum force limit setting.
  if(m_StartUpSettings->exists("MaxForceLimit")){
    m_MaxForceLimitSetting = &m_StartUpSettings->operator []("MaxForceLimit");
    *m_MaxForceLimitSetting = m_MaxForceLimit;
  }else{
    m_StartUpSettings->add("MaxForceLimit", Setting::TypeInt64) = m_MaxForceLimit;
  }

  // Finally try to write the configuration to the file.
  try{
    //m_CurrentConfig.writeFile(s_ConfigurationFileName);
    m_CurrentConfig.writeFile(m_ConfigurationStoragePath.c_str());
    std::cerr << "Configuration successfully written to: " << m_ConfigurationStoragePath << std::endl;
  }catch(const FileIOException &fioex){
    return(false);
  }

  return(true);
}
