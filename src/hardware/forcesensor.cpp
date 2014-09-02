/**
 * @file forcesensor.cpp
 * @brief The force sensor.
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <mutex>
#include "forcesensor.h"

// Namespaces
using namespace std;

// A deleter which doesn't do anything, required for passing shared_ptr.
void do_nothing_deleter(ForceSensorMessageHandler*){return;}

/**
 * @brief Creates a serial interface and initializes all the needed variables.
 * @param type Value type
 * @param waitmessagehandler Condition varaible to wait for message handlers.
 * @param waitmessagehandlermutex Mutex to protect waitmessagehandler.
 * @param messagehandlerfinishednr Amount of finished message handlers.
 * @param baudrate The baude rate of the com port.
 */
ForceSensor::ForceSensor(UpdatedValuesReceiver::ValueType type,
                         std::shared_ptr<std::condition_variable> waitmessagehandler,
                         std::shared_ptr<std::mutex> waitmessagehandlermutex,
                         std::shared_ptr<int> messagehandlerfinishednr,
                         unsigned int baudrate)
  : SerialInterface(baudrate),
    m_MessageHandler(&m_SerialPort,
                     type,
                     &m_ReadingSerialInterfaceMutex,
                     waitmessagehandler,
                     waitmessagehandlermutex,
                     messagehandlerfinishednr),
    m_ScalingFactor(161380.83),
    m_NominalForce(20/*N*/),
    m_NominalValue(0.4965/*mv/V*/),
    m_InputSensitivity(1.0/*mv/V*/),
    m_MeasureEndValue(0xffffff/105*100),
    m_ZeroValue(0x800000),
    FORCE_SENSOR_SET_BIPOLAR("\x014")
{
}

/**
 * @brief Destructor
 */
ForceSensor::~ForceSensor(){
  //std::cout << "ForceSensor destructor finished." << std::endl;
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
 * @brief Sets force to zero.
 */
void ForceSensor::setZeroForce(void){
  m_MessageHandler.setZeroForce();
}

/**
  * @brief Returns the pointer to the message handler.
  * @return Pointer to the message handler.
  */
std::shared_ptr<ForceSensorMessageHandler> ForceSensor::getMessageHandler(void){
  return(std::shared_ptr<ForceSensorMessageHandler>(&m_MessageHandler, do_nothing_deleter));
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
