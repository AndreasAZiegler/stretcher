
#ifndef FORCESENSORMESSAGEHANDLER_H
#define FORCESENSORMESSAGEHANDLER_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include "messagehandler.h"

/**
 * @brief Message handler for the linear stages
 */
class ForceSensorMessageHandler : virtual public MessageHandler
{
  public:

    /**
     * @brief Forwards the pointer to the serial port to the base class.
     * @param serialport Pointer to the serial port.
     */
    ForceSensorMessageHandler(wxSerialPort *serialport,
                              UpdatedValuesReceiver::ValueType type,
                              std::mutex *readingSerialInterfaceMutex,
                              std::shared_ptr<std::condition_variable> waitmessagehandler,
                              std::shared_ptr<std::mutex> waitmessagehandlermutex,
                              std::shared_ptr<int> messagehandlerfinishednr);

    ~ForceSensorMessageHandler();

    /**
     * @brief Updated scale factor parameters with the values given from the GUI.
     * @param scalingfactor Force sensor scaling factor
     * @param zerovalue Force sensor zero value
     */
    void setScaleFactor(double scalingfactor, double zerovalue);

    /**
     * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
     */
    virtual void receiver(void);

    /**
     * @brief Informs the related objects according to the received message
     * @param message Received message
     */
    void handler(char *message);

    /**
     * @brief Returns the latest position of the linear stage
     * @return latest position of the linear stage.
     */
    MeasurementValue getCurrentForce(){
      return(m_CurrentForce);
    }

  private:

    MeasurementValue m_CurrentForce;							/**< Struct containing the current force value and its time stamp. */

    double m_ScalingFactor;                  			/**< Sensor scaling factor */
    double m_ZeroValue;                      			/**< Zero value */

};

#endif // LINEARSTAGEMESSAGEHANDLER_H
