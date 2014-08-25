/**
 * @file forcesensor.h
 * @brief The force sensor.
 * @author Andreas Ziegler
 */

#ifndef FORCESENSOR_H
#define FORCESENSOR_H

// Includes
#include "serialinterface.h"
#include "forcesensormessagehandler.h"

/**
 * @class ForceSensor forcesensor.h "hardware/forcesensor.h"
 * @brief Class that provides access to the converter hardware.
 */
class ForceSensor : public SerialInterface
{
  public:

    /**
     * @brief Creates a serial interface and initializes all the needed variables.
     * @param type Value type
     * @param waitmessagehandler Condition varaible to wait for message handlers.
     * @param waitmessagehandlermutex Mutex to protect waitmessagehandler.
     * @param messagehandlerfinishednr Amount of finished message handlers.
     * @param baudrate The baude rate of the com port.
     */
    ForceSensor(UpdatedValuesReceiver::ValueType type,
                std::shared_ptr<std::condition_variable> waitmessagehandler,
                std::shared_ptr<std::mutex> waitmessagehandlermutex,
                std::shared_ptr<int> messagahandlerfinishednr,
                unsigned int baudrate = 115200);

    /**
     * @brief Set the force sensor to the bipolar mode.
     */
    void setBipolarMode();

    /**
     * @brief Destructor
     */
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
     * @brief Sets force to zero.
     */
    void setZeroForce(void);

    /**
      * @brief Returns the pointer to the message handler.
      * @return Pointer to the message handler.
      */
    std::shared_ptr<ForceSensorMessageHandler> getMessageHandler(void);

  private:

    // Defined commands for the force sensor.
    const char *FORCE_SENSOR_SET_BIPOLAR;					/**< Command to get the device mode */

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
