
// Includes
#include <mutex>
#include "forcesensor.h"

using namespace std;

/**
 * @brief Forwards the com port and the baud rate to SerialPort
 * @param comPort com port
 * @param baudrate baudrate
 */
ForceSensor::ForceSensor(UpdatedValuesReceiver::ValueType type,
                         std::shared_ptr<std::condition_variable> waitmessagehandler,
                         std::shared_ptr<std::mutex> waitmessagehandlermutex,
                         std::shared_ptr<int> messagehandlerfinishednr,
                         unsigned int baudrate)
  : SerialInterface(type, baudrate),
    m_MessageHandler(&m_SerialPort, type, &m_ReadingSerialInterfaceMutex, waitmessagehandler, waitmessagehandlermutex, messagehandlerfinishednr),
    m_ScalingFactor(161380.83),
    m_NominalForce(20/*N*/),
    m_NominalValue(0.4965/*mv/V*/),
    m_InputSensitivity(1.0/*mv/V*/),
    m_MeasureEndValue(0xffffff/105*100),
    m_ZeroValue(0x800000),
    FORCE_SENSOR_SET_BIPOLAR("\x014")
{
}

ForceSensor::~ForceSensor(){
  //delete FORCE_SENSOR_SET_BIPOLAR;
}

/**
 * @return forceSensorMessageHandler*
 */
/*
ForceSensorMessageHandler* ForceSensor::getMessageHandler ()
{
}
*/


/**
 * @return double
 */
double ForceSensor::getForce()
{
  return(0);
}

/**
 * @brief Updated scale factor parameters with the values given from the GUI.
 * @param iNominalValue Nominal value
 * @param iMeasureEndValue Measure end value
 * @param iNominalForce Nominal force
 * @param iInputSensitivity Input sensitivity
 */
void ForceSensor::setScaleFactor(double nominalforce, double nominalvalue, double inputsensitivity, double measureendvalue, int zerovalue){
  m_NominalValue = nominalvalue;
  m_MeasureEndValue = measureendvalue;
  m_NominalForce = nominalforce;
  m_InputSensitivity = inputsensitivity;

  calculateScaleFactor();
  m_MessageHandler.setScaleFactor(m_ScalingFactor, m_ZeroValue);
}

/**
 * @brief Set the force sensor to the bipolar mode.
 */
void ForceSensor::setBipolarMode(){
  char buffer[1];

  memcpy(buffer, FORCE_SENSOR_SET_BIPOLAR, 1);

  lock_guard<mutex> lck1 {m_ReadingSerialInterfaceMutex};
  if(1 == m_SerialPort.IsOpen()){
    lock_guard<mutex> lck2 {m_WritingSerialInterfaceMutex};
    m_SerialPort.Writev(buffer, 1, 5/*ms*/);
  }

}

/**
 * @brief Calculate scaling factor depending on the nominal value, the measurement end value, the nominal force and the input sensitivity.
 */
void ForceSensor::calculateScaleFactor(){
  m_ScalingFactor = m_NominalValue * m_MeasureEndValue / ((m_NominalForce * 2) * m_InputSensitivity);
}
