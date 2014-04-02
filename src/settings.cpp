
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
  : s_ConfigurationFileName("config.cfg")
{
  readSettings();
}

Settings::~Settings(){
  writeSettings();

  delete m_LinMot1ComPortSettings;
  delete m_LinMot2ComPortSettings;
  delete m_ForceSensorComPortSettings;
  delete m_LinMot1Settings;
  delete m_LinMot2Settings;
  delete m_ForceSensorSettings;
  delete m_RootSettings;
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

    try{
      // Read the storage path
      m_RootSettings->lookupValue("StoragePath", m_StoragePath);

      // Read the settings for linear motor 1
      m_LinMot1Settings = &m_RootSettings->operator []("LinearMotor1");
      m_LinMot1Settings->lookupValue("ComPort", m_LinMot1ComPort);
      m_LinMot1Settings->lookupValue("Baudrate", m_LinMot1BaudRate);

      // Read the settings for linear motor 2
      m_LinMot2Settings = &m_RootSettings->operator []("LinearMotor2");
      m_LinMot2Settings->lookupValue("ComPort", m_LinMot2ComPort);
      m_LinMot2Settings->lookupValue("Baudrate", m_LinMot2BaudRate);

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
  m_RootSettings = &m_CurrentConfig.getRoot();

  // Write the storage path
  if(m_RootSettings->exists("StoragePath")){
    m_StoragePathSettings = &m_RootSettings->operator []("StoragePath");
    *m_StoragePathSettings = m_StoragePath;
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
    m_LinMot1ComPortSettings = &m_LinMot1Settings->operator []("ComPort");
    *m_LinMot1ComPortSettings = m_LinMot1ComPort;
  }else{
    m_LinMot1Settings->add("ComPort", Setting::TypeString) = m_LinMot1ComPort;
  }

  // Writing baud rate setting for linear stage 1
  if(m_LinMot1Settings->exists("Baudrate")){
    m_LinMot1BaudRateSettings = &m_LinMot1Settings->operator []("Baudrate");
    *m_LinMot1BaudRateSettings = m_LinMot1BaudRate;
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
    m_LinMot2ComPortSettings = &m_LinMot2Settings->operator []("ComPort");
    *m_LinMot2ComPortSettings = m_LinMot2ComPort;
  }else{
    m_LinMot2Settings->add("ComPort", Setting::TypeString) = m_LinMot2ComPort;
  }

  // Writing baud rate setting for linear stage 2
  if(m_LinMot2Settings->exists("Baudrate")){
    m_LinMot2BaudRateSettings = &m_LinMot2Settings->operator []("Baudrate");
    *m_LinMot2BaudRateSettings = m_LinMot2BaudRate;
  }else{
    m_LinMot2Settings->add("Baudrate", Setting::TypeInt) = m_LinMot2BaudRate;
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
