
#ifndef SETTINGS_H
#define SETTINGS_H

#include <libconfig.h++>
#include <string>
#include <memory>

/**
 * @brief Loads the settings from the file "config.cfg" it exists during program start and saves the settings during program exit.
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
      return(m_ForceSensorComPort.c_str());
    }

    /**
     * @brief Returns the baud rate of the force sensor.
     * @return com port as a string.
     */
    int getForceSensorBaudRate(){
      return(m_ForceSensorBaudRate);
    }

    /**
     * @brief Returns the scaling factor of the force sensor.
     * @return scaling factor
     */
    double getForceSensorScalingFactor(){
      return(m_ForceSensorScalingFactor);
    }

    /**
     * @brief Returns the nominal force of the force sensor.
     * @return nominal force
     */
    double getForceSensorNominalForce(){
      return(m_ForceSensorNominalForce);
    }

    /**
     * @brief Returns the nominal value of the force sensor.
     * @return nominal value
     */
    double getForceSensorNominalValue(){
      return(m_ForceSensorNominalValue);
    }

    /**
     * @brief Returns the input sensitivity of the force sensor.
     * @return input sensitivity
     */
    double getForceSensorInputSensitivity(){
      return(m_ForceSensorInputSensitivity);
    }

    /**
     * @brief Returns the measure end value of the force sensor.
     * @return measure end value
     */
    double getForceSensorMeasureEndValue(){
      return(m_ForceSensorMeasureEndValue);
    }

    /**
     * @brief Returns the zero value of the force sensor.
     * @return zero value
     */
    int getForceSensorZeroValue(){
      return(m_ForceSensorZeroValue);
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
    void setLinMot1ComPort(std::string comPort){
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
    void setLinMot2ComPort(std::string comPort){
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
     */
    void setForceSensorComPort(std::string comPort){
      m_ForceSensorComPort = comPort;
    }

    /**
     * @brief Set the baud rate for the force sensor.
     * @param baudrate baud rate
     */
    void setForceSensorBaudRate(int baudrate){
      m_ForceSensorBaudRate = baudrate;
    }

    /**
     * @brief Set the scaling factor for the force sensor.
     * @param scalingfactor scaling factor
     */
    void setForceSensorScalingFactor(double scalingfactor){
      m_ForceSensorScalingFactor = scalingfactor;
    }

    /**
     * @brief Set the nominal force for the force sensor.
     * @param scalingfactor scaling factor
     */
    void setForceSensorNominalForce(double nominalforce){
      m_ForceSensorNominalForce = nominalforce;
    }

    /**
     * @brief Set the nominal value for the force sensor.
     * @param nominalvalue nominal value
     */
    void setForceSensorNominalValue(double nominalvalue){
      m_ForceSensorNominalValue = nominalvalue;
    }

    /**
     * @brief Set the input sensitivity for the force sensor.
     * @param inputsensitivity input sensitivity
     */
    void setForceSensorInputSensitivity(double inputsensitivity){
      m_ForceSensorInputSensitivity = inputsensitivity;
    }

    /**
     * @brief Set the measure end value for the force sensor.
     * @param measureendvalue measure end value
     */
    void setForceMeasureEndValue(double measureendvalue){
      m_ForceSensorMeasureEndValue = measureendvalue;
    }

    /**
     * @brief Set the zero value for the force sensor.
     * @param zerovalue zero value
     */
    void setForceZeroValue(int zerovalue){
      m_ForceSensorZeroValue = zerovalue;
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

    libconfig::Config m_CurrentConfig;												/**< Current configuration */
    const char *s_ConfigurationFileName;											/**< The file name for the configuration file */

    libconfig::Setting* m_RootSettings;												/**< The root settings */

    libconfig::Setting* m_LinMot1Settings;										/**< The settings for the first linear stage motor */
    libconfig::Setting* m_LinMot2Settings;										/**< The settings for the second linear stage motor */
    libconfig::Setting* m_LinMot1ComPortSetting;							/**< The setting for the com port of the first linear stage */
    libconfig::Setting* m_LinMot2ComPortSetting;							/**< The setting for the com port of the second linear stage */
    libconfig::Setting* m_LinMot1BaudRateSetting;							/**< The setting for the baud rate of the first linear stage */
    libconfig::Setting* m_LinMot2BaudRateSetting;							/**< The setting for the baud rate of the second linear stage */

    libconfig::Setting* m_ForceSensorSettings;								/**< The setting for the force sensor */
    libconfig::Setting* m_ForceSensorComPortSetting; 					/**< The setting for the com port of the force sensor */
    libconfig::Setting* m_ForceSensorBaudRateSetting;					/**< The setting for the baud rate of the force sensor */
    libconfig::Setting* m_ForceSensorScalingFactorSetting;		/**< The setting for the scaling factor of the force sensor */
    libconfig::Setting* m_ForceSensorNominalForceSetting;			/**< The setting for the nominal force of the force sensor */
    libconfig::Setting* m_ForceSensorNominalValueSetting;			/**< The setting for the nominal value of the force sensor */
    libconfig::Setting* m_ForceSensorInputSensitivitySetting;	/**< The setting for the input sesitivity of the force sensor */
    libconfig::Setting* m_ForceSensorMeasureEndValueSetting;	/**< The setting for the measure end value of the force sensor */
    libconfig::Setting* m_ForceSensorZeroValueSetting;				/**< The setting for the zero value of the force sensor */

    libconfig::Setting* m_StoragePathSetting;									/**< The settings for the storage path */

    std::string m_LinMot1ComPort;															/**< The com port for the first linear stage motor */
    std::string m_LinMot2ComPort;															/**< The com port for the second linear stage motor */
    int m_LinMot1BaudRate;																		/**< The baud rate for the first linear stage motor */
    int m_LinMot2BaudRate;																		/**< The baud rate for the second linear stage motor */

    std::string m_ForceSensorComPort;													/**< The com port for the force sensor */
    int m_ForceSensorBaudRate;																/**< The baud rate for the force sensor */
    double m_ForceSensorScalingFactor;												/**< The scaling factor for the force sensor */
    double m_ForceSensorNominalForce;													/**< The nominal force for the force sensor */
    double m_ForceSensorNominalValue;													/**< The nominal value for the force sensor */
    double m_ForceSensorInputSensitivity;											/**< The input sensitivity for the force sensor */
    double m_ForceSensorMeasureEndValue;											/**< The measure end value for the force sensor */
    double m_ForceSensorZeroValue;														/**< The zero value for the force sensor */

    std::string m_StoragePath;																/**< Folder where the images and the data will be saved */

};

#endif // SETTINGS_H
