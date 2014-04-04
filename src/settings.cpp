
// Includes
#include <memory>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "settings.h"

using namespace std;
using namespace libconfig;


/**
 * @brief Load settings if file exists.
 */
Settings::Settings()
  : s_ConfigurationFileName("config.cfg"),
    m_RootSettings(NULL),
    m_LinMot1Settings(NULL),
    m_LinMot2Settings(NULL),
    m_ForceSensorSettings(NULL),
    m_LinMot1ComPortSetting(NULL),
    m_LinMot2ComPortSetting(NULL),
    m_LinMot1BaudRateSetting(NULL),
    m_LinMot2BaudRateSetting(NULL),
    m_ForceSensorComPortSetting(NULL),
    m_StoragePathSetting(NULL),
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
    m_StoragePath("/home")
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
  if(FILE *file = fopen(s_ConfigurationFileName, "r")){
    fclose(file);

    // Open the configuration file.
    try{
      m_CurrentConfig.readFile(s_ConfigurationFileName);
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

    // Read the storage path
    try{
      m_RootSettings->lookupValue("StoragePath", m_StoragePath);
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

  // Write the storage path
  if(m_RootSettings->exists("StoragePath")){
    m_StoragePathSetting = &m_RootSettings->operator []("StoragePath");
    *m_StoragePathSetting = m_StoragePath;
  }else{
    m_RootSettings->add("StoragePath", Setting::TypeString) = m_StoragePath;
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

  // Finally try to write the configuration to the file.
  try{
    m_CurrentConfig.writeFile(s_ConfigurationFileName);
    std::cerr << "Configuration successfully written to: " << s_ConfigurationFileName << std::endl;
  }catch(const FileIOException &fioex){
    return(false);
  }

  return(true);
}
