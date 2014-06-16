
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
      return(m_MeasurementValuesStoragePath);
    }

    /**
     * @brief Returns the name of set 1.
     * @return Name as a string.
     */
    std::string getSet1Name(){
      return(m_Set1Name);
    }

    /**
     * @brief Returns the max distance of set 1.
     * @return Max distance
     */
    int getSet1MaxDistance(){
      return(m_Set1MaxDistance);
    }

    /**
     * @brief Returns the min distance of set 1.
     * @return Min distance
     */
    int getSet1MinDistance(){
      return(m_Set1MinDistance);
    }

    /**
     * @brief Returns the max force of set 1.
     * @return Max force
     */
    int getSet1MaxForce(){
      return(m_Set1MaxForce);
    }

    /**
     * @brief Returns the min force of set 1.
     * @return Min force
     */
    int getSet1MinForce(){
      return(m_Set1MinForce);
    }

    /**
     * @brief Returns the name of set 2.
     * @return Name as a string.
     */
    std::string getSet2Name(){
      return(m_Set2Name);
    }

    /**
     * @brief Returns the max distance of set 2.
     * @return Max distance
     */
    int getSet2MaxDistance(){
      return(m_Set2MaxDistance);
    }

    /**
     * @brief Returns the min distance of set 2.
     * @return Min distance
     */
    int getSet2MinDistance(){
      return(m_Set2MinDistance);
    }

    /**
     * @brief Returns the max force of set 2.
     * @return Max force
     */
    int getSet2MaxForce(){
      return(m_Set2MaxForce);
    }

    /**
     * @brief Returns the min force of set 2.
     * @return Min force
     */
    int getSet2MinForce(){
      return(m_Set2MinForce);
    }

    /**
     * @brief Returns the name of set 3.
     * @return Name as a string.
     */
    std::string getSet3Name(){
      return(m_Set3Name);
    }

    /**
     * @brief Returns the max distance of set 3.
     * @return Max distance
     */
    int getSet3MaxDistance(){
      return(m_Set3MaxDistance);
    }

    /**
     * @brief Returns the min distance of set 3.
     * @return Min distance
     */
    int getSet3MinDistance(){
      return(m_Set3MinDistance);
    }

    /**
     * @brief Returns the max force of set 3.
     * @return Max force
     */
    int getSet3MaxForce(){
      return(m_Set3MaxForce);
    }

    /**
     * @brief Returns the min force of set 3.
     * @return Min force
     */
    int getSet3MinForce(){
      return(m_Set3MinForce);
    }

    /**
     * @brief Returns the name of set 4.
     * @return Name as a string.
     */
    std::string getSet4Name(){
      return(m_Set4Name);
    }

    /**
     * @brief Returns the max distance of set 4.
     * @return Max distance
     */
    int getSet4MaxDistance(){
      return(m_Set4MaxDistance);
    }

    /**
     * @brief Returns the min distance of set 4.
     * @return Min distance
     */
    int getSet4MinDistance(){
      return(m_Set4MinDistance);
    }

    /**
     * @brief Returns the max force of set 4.
     * @return Max force
     */
    int getSet4MaxForce(){
      return(m_Set4MaxForce);
    }

    /**
     * @brief Returns the min force of set 4.
     * @return Min force
     */
    int getSet4MinForce(){
      return(m_Set4MinForce);
    }

    /**
     * @brief Returns the maximum position distance.
     * @return The maximum position distance.
     */
    long getMaxPosDistance(void){
      return(m_MaxPosDistance);
    }

    /**
     * @brief Returns the mounting length.
     * @return The mounting length.
     */
    long getMountingLength(void){
      return(m_MountingLength);
    }

    /**
     * @brief Returns the gage length.
     * @return The gage length.
     */
    long getGageLength(void){
      return(m_GageLength);
    }

    /**
     * @brief Returns the minimum distance limit.
     * @return The minimum distance limit.
     */
    long getMinDistanceLimit(void){
      return(m_MinDistanceLimit);
    }

    /**
     * @brief Returns the maximum distance limit.
     * @return The maximum distance limit.
     */
    long getMaxDistanceLimit(void){
      return(m_MaxDistanceLimit);
    }

    /**
     * @brief Returns the minimum force limit.
     * @return The minimum force limit.
     */
    long getMinForceLimit(void){
      return(m_MinForceLimit);
    }

    /**
     * @brief Returns the maximum force limit.
     * @return The maximum force limit.
     */
    long getMaxForceLimit(void){
      return(m_MaxForceLimit);
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
     * @brief Sets the storage place for images and data
     * @param Storage place as a string
     */
    void setStoragePath(std::string storagePath){
      m_MeasurementValuesStoragePath = storagePath;
    }

    /**
     * @brief Sets the maximum position distance.
     * @param maxposdistance Maximum position distance.
     */
    void setMaxPosDistance(long maxposdistance){
      m_MaxPosDistance = maxposdistance;
    }

    /**
     * @brief Sets the mounting length.
     * @param mountinglength Mounting length
     */
    void setMountingLength(long mountinglength){
      m_MountingLength = mountinglength;
    }

    /**
     * @brief Sets the gage length.
     * @param gagelength Gage length.
     */
    void setGageLength(long gagelength){
      m_GageLength = gagelength;
    }

    /**
     * @brief Sets the minimum distance limit.
     * @param mindistancelimit Minimum distance limit.
     */
    void setMinDistanceLimit(long mindistancelimit){
      m_MinDistanceLimit = mindistancelimit;
    }

    /**
     * @brief Sets the maximum distance limit.
     * @param maxdistancelimit Maximum distance limit.
     */
    void setMaxDistanceLimit(long maxdistancelimit){
      m_MaxDistanceLimit = maxdistancelimit;
    }

    /**
     * @brief Sets the minimum force limit.
     * @param minforcelimit Minimum force limit.
     */
    void setMinForceLimit(long minforcelimit){
      m_MinForceLimit = minforcelimit;
    }

    /**
     * @brief Sets the maximum force limit.
     * @param maxforcelimit Maximum force limit.
     */
    void setMaxForceLimit(long maxforcelimit){
      m_MaxForceLimit = maxforcelimit;
    }

  private:

    libconfig::Config m_CurrentConfig;												/**< Current configuration */
    //const char *s_ConfigurationFileName;											/**< The file name for the configuration file */

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

    libconfig::Setting* m_StoragePathsSetting;								/**< The settings for the storage path */
    libconfig::Setting* m_ConfigurationStoragePathSetting;		/**< Storage path for the configuration file */
    libconfig::Setting* m_MeasurementValuesStoragePathSetting;/**< Storage path for the measurement values */

    libconfig::Setting* m_Set1Settings;
    libconfig::Setting* m_Set1NameSetting;
    libconfig::Setting* m_Set1MaxDistanceSetting;
    libconfig::Setting* m_Set1MinDistanceSetting;
    libconfig::Setting* m_Set1MaxForceSetting;
    libconfig::Setting* m_Set1MinForceSetting;

    libconfig::Setting* m_Set2Settings;
    libconfig::Setting* m_Set2NameSetting;
    libconfig::Setting* m_Set2MaxDistanceSetting;
    libconfig::Setting* m_Set2MinDistanceSetting;
    libconfig::Setting* m_Set2MaxForceSetting;
    libconfig::Setting* m_Set2MinForceSetting;

    libconfig::Setting* m_Set3Settings;
    libconfig::Setting* m_Set3NameSetting;
    libconfig::Setting* m_Set3MaxDistanceSetting;
    libconfig::Setting* m_Set3MinDistanceSetting;
    libconfig::Setting* m_Set3MaxForceSetting;
    libconfig::Setting* m_Set3MinForceSetting;

    libconfig::Setting* m_Set4Settings;
    libconfig::Setting* m_Set4NameSetting;
    libconfig::Setting* m_Set4MaxDistanceSetting;
    libconfig::Setting* m_Set4MinDistanceSetting;
    libconfig::Setting* m_Set4MaxForceSetting;
    libconfig::Setting* m_Set4MinForceSetting;

    libconfig::Setting* m_StartUpSettings;
    libconfig::Setting* m_MaxPosDistanceSetting;
    libconfig::Setting* m_MountingLengthSetting;
    libconfig::Setting* m_GageLengthSetting;
    libconfig::Setting* m_MinDistanceLimitSetting;
    libconfig::Setting* m_MaxDistanceLimitSetting;
    libconfig::Setting* m_MinForceLimitSetting;
    libconfig::Setting* m_MaxForceLimitSetting;

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

    std::string m_ConfigurationStoragePath;										/**< Folder where the configuration file will be saved */
    std::string m_MeasurementValuesStoragePath;								/**< Folder where the images and the data will be saved */

    std::string m_Set1Name;																		/**< Name for the first limit set. */
    int m_Set1MaxDistance;																		/**< Maximum distance limit value. */
    int m_Set1MinDistance;																		/**< Minimum distance limit value. */
    int m_Set1MaxForce;																				/**< Maximum force limit value. */
    int m_Set1MinForce;																				/**< Minimum force limit value. */

    std::string m_Set2Name;																		/**< Name for the second limit set. */
    int m_Set2MaxDistance;																		/**< Maximum distance limit value. */
    int m_Set2MinDistance;																		/**< Minimum distance limit value. */
    int m_Set2MaxForce;																				/**< Maximum force limit value. */
    int m_Set2MinForce;																				/**< Minimum force limit value. */

    std::string m_Set3Name;																		/**< Name for the third limit set. */
    int m_Set3MaxDistance;																		/**< Maximum distance limit value. */
    int m_Set3MinDistance;																		/**< Minimum distance limit value. */
    int m_Set3MaxForce;																				/**< Maximum force limit value. */
    int m_Set3MinForce;																				/**< Minimum force limit value. */

    std::string m_Set4Name;																		/**< Name for the fourth limit set. */
    int m_Set4MaxDistance;																		/**< Maximum distance limit value. */
    int m_Set4MinDistance;																		/**< Minimum distance limit value. */
    int m_Set4MaxForce;																				/**< Maximum force limit value. */
    int m_Set4MinForce;																				/**< Minimum force limit value. */

    long long m_MaxPosDistance;
    long long m_MountingLength;
    long long m_GageLength;
    long long m_MinDistanceLimit;
    long long m_MaxDistanceLimit;
    long long m_MinForceLimit;
    long long m_MaxForceLimit;

};

#endif // SETTINGS_H
