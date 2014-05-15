
#ifndef FORCESENSOR_H
#define FORCESENSOR_H
#include "serialinterface.h"

#include "forcesensormessagehandler.h"

/**
 * @brief Class that provides access to the converter hardware.
 */
class ForceSensor : public SerialInterface
{
  public:

    /**
     * @brief Forwards the com port and the baud rate to SerialPort
     * @param comPort com port
     * @param baudrate baudrate
     */
    ForceSensor(UpdatedValuesReceiver::ValueType type, std::shared_ptr<std::condition_variable> waitmessagehandler, std::shared_ptr<std::mutex> waitmessagehandlermutex, unsigned int baudrate = 115200);

    /**
     * @brief Set the force sensor to the bipolar mode.
     */
    void setBipolarMode();

    ~ForceSensor();

   /**
     * @brief Updated scale factor parameters with the values given from the GUI.
     * @param iNominalValue Nominal value
     * @param iMeasureEndValue Measure end value
     * @param iNominalForce Nominal force
     * @param iInputSensitivity Input sensitivity
     */
    void setScaleFactor(double nominalforce, double nominalvalue, double inputsensitivity, double measureendvalue, int zerovalue);

    /**
      * @brief Returns the pointer to the message handler.
      * @return Pointer to the message handler.
      */
    ForceSensorMessageHandler* getMessageHandler(void){
      return(&m_MessageHandler);
    }

    double getForce ();


  private:

    // Defined commands for the force sensor.
    const char *FORCE_SENSOR_SET_BIPOLAR;											/**< Command to get the device mode */

    /**
     * @brief Calculate scaling factor depending on the nominal value, the measurement end value, the nominal force and the input sensitivity.
     */
    void calculateScaleFactor();

    ForceSensorMessageHandler m_MessageHandler;		/**< Pointer to the MessageHandler object */

    double m_NominalForce;                   			/**< Nominal force to calculate the scaling factor */
    double m_NominalValue;                   			/**< Nominal value to calculate the scaling factor */
    double m_InputSensitivity;               			/**< Input sensitivity to calculate the scaling factor */
    double m_MeasureEndValue;                			/**< Measure end value to calculate the scaling factor */
    double m_ScalingFactor;                  			/**< Sensor scaling factor */
    double m_ZeroValue;                      			/**< Zero value */

};

#endif // FORCESENSOR_H
