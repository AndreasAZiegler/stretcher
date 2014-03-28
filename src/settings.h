
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
    const char* getLinMot1ComPort(){
      return(m_LinMot1ComPort.c_str());
    }

    /**
     * @brief Returns the baud rate of the first linear stage motor.
     * @return com port as a string.
     */
    int getLinMot1BaudRate(){
      return(m_LinMot1BaudRate);
    }

    /**
     * @brief Returns the com port of the second linear stage motor.
     * @return com port as a string.
     */
    const char* getLinMot2ComPort(){
      return(m_LinMot2ComPort.c_str());
    }

    /**
     * @brief Returns the baud rate of the second linear stage motor.
     * @return com port as a string.
     */
    int getLinMot2BaudRate(){
      return(m_LinMot2BaudRate);
    }

    /**
     * @brief Returns the com port of the force sensor.
     * @return com port as a string.
     */
    const char* getForceSensorComPort(){
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
     */
    void setLinMot1ComPort(const char* comPort){
      m_LinMot1ComPort = comPort;
    }

    /**
     * @brief Set the baud rate for the first linear stage motor.
     * @param comPort com port as a string.
     */
    void setLinMot1BaudRate(int baudrate){
      m_LinMot1BaudRate = baudrate;
    }

    /**
     * @brief Set the com port for the second linear stage motor.
     * @param comPort com port as a string.
     * @return true if sucessfull, false otherwise.
     */
    void setLinMot2ComPort(const char* comPort){
      m_LinMot2ComPort = comPort;
    }

    /**
     * @brief Set the baud rate for the second linear stage motor.
     * @param comPort com port as a string.
     */
    void setLinMot2BaudRate(int baudrate){
      m_LinMot2BaudRate = baudrate;
    }

    /**
     * @brief Set the com port for the force sensor.
     * @param comPort com port as a string.
     * @return true if sucessfull, false otherwise.
     */
    void setForceSensorComPort(const char* comPort){
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
    const char *s_ConfigurationFileName;					/**< The file name for the configuration file */

    libconfig::Setting* m_RootSettings;						/**< The root settings */
    libconfig::Setting* m_LinMot1Settings;				/**< The settings for the first linear stage motor */
    libconfig::Setting* m_LinMot2Settings;				/**< The settings for the second linear stage motor */
    libconfig::Setting* m_ForceSensorSettings;		/**< The settings for the force sensor */
    libconfig::Setting* m_LinMot1ComPortSettings;
    libconfig::Setting* m_LinMot2ComPortSettings;
    libconfig::Setting* m_LinMot1BaudRateSettings;
    libconfig::Setting* m_LinMot2BaudRateSettings;
    libconfig::Setting* m_ForceSensorComPortSettings;
    libconfig::Setting* m_StoragePathSettings;

    std::string m_LinMot1ComPort;									/**< The com port for the first linear stage motor */
    std::string m_LinMot2ComPort;									/**< The com port for the second linear stage motor */
    int m_LinMot1BaudRate;												/**< The baud rate for the first linear stage motor */
    int m_LinMot2BaudRate;												/**< The baud rate for the second linear stage motor */
    const char* m_ForceSensorComPort;							/**< The com port for the force sensor */
    std::string m_StoragePath;										/**< Folder where the images and the data will be saved */

};

#endif // SETTINGS_H
