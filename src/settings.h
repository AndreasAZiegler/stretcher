
#ifndef SETTINGS_H
#define SETTINGS_H

#include <libconfig.h++>
#include <string>

/**
  * class Settings
  * 
  */

class Settings
{
  public:

    /**
     * @brief Load settings if file exists.
     */
    Settings();

    ~Settings();

    /**
     * @brief Trys to read the settings from the configuration file.
     * @return true if sucessfull false otherwise.
     */
    bool readSettings();

    /**
     * @brief Saves the settings in the configuration file.
     * @return true if sucessfull false otherwise.
     */
    bool writeSettings();

    /**
     * @brief Returns the com port of the first linear stage motor.
     * @return com port as a string.
     */
    std::string getLinMot1ComPort(){
      return(m_LinMot1ComPort);
    }

    /**
     * @brief Returns the com port of the second linear stage motor.
     * @return com port as a string.
     */
    std::string getLinMot2ComPort(){
      return(m_LinMot2ComPort);
    }

    /**
     * @brief Returns the com port of the force sensor.
     * @return com port as a string.
     */
    std::string getForceSensorComPort(){
      return(m_ForceSensorComPort);
    }

    /**
     * @brief Returns the storage place for images and data.
     * @return Storage place as a string.
     */
    std::string getStoragePath(){
      return(m_StoragePath);
    }

    /**
     * @brief Set the com port for the first linear stage motor.
     * @param comPort com port as a string.
     * @return true if sucessfull, false otherwise.
     */
    void setLinMot1ComPort(std::string comPort){
      m_LinMot1ComPort = comPort;
    }

    /**
     * @brief Set the com port for the second linear stage motor.
     * @param comPort com port as a string.
     * @return true if sucessfull, false otherwise.
     */
    void setLinMot2ComPort(std::string comPort){
      m_LinMot2ComPort = comPort;
    }

    /**
     * @brief Set the com port for the force sensor.
     * @param comPort com port as a string.
     * @return true if sucessfull, false otherwise.
     */
    void setForceSensorComPort(std::string comPort){
      m_ForceSensorComPort = comPort;
    }

    /**
     * @brief Set the storage place for images and data
     * @param Storage place as a string
     * @return true if sucessfull, false otherwise.
     */
    void setStoragePath(std::string storagePath){
      m_StoragePath = storagePath;
    }

  private:

    libconfig::Config m_CurrentConfig;						/**< Current configuration */
    const char *s_ConfigurationFileName;		/**< The file name for the configuration file */
    libconfig::Setting* m_RootSettings;
    libconfig::Setting* m_LinMot1Settings;
    libconfig::Setting* m_LinMot2Settings;
    libconfig::Setting* m_ForceSensorSettings;
    std::string m_LinMot1ComPort;									/**< The com port for the first linear stage motor */
    std::string m_LinMot2ComPort;									/**< The com port for the second linear stage motor */
    std::string m_ForceSensorComPort;							/**< The com port for the force sensor */
    std::string m_StoragePath;										/**< Folder where the images and the data will be saved */

};

#endif // SETTINGS_H
