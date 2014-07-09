/**
 * @file updatedvaluesreceiver.h
 * @brief The update values receiver.
 * @author Andreas Ziegler
 */

#ifndef UPDATEDVALUESRECEIVER_H
#define UPDATEDVALUESRECEIVER_H

// Includes
#include <list>
#include <functional>
#include <chrono>
#include "updatedvalues.h"

/**
 * @class UpdatedValuesReceiver updatedvaluesreceiver.h "updatedvaluesreceiver.h"
 * @brief Abstract class used as an interface, that the message handlers can use the updateValue() method of the receiver in a standard way.
 */
class UpdatedValuesReceiver : virtual public UpdatedValues
{
  public:

    UpdatedValuesReceiver();

    virtual ~UpdatedValuesReceiver();

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type) = 0;

  protected:
    std::list<std::function<void(MeasurementValue, UpdatedValuesReceiver::ValueType)>> m_UpdateMethodList;			/**< List containing struct with pointers to the MyFrame object and the update method*/

    std::list<std::function<void(MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator m_ForceId;			/**< Id of the force callback method */
    std::list<std::function<void(MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator m_DistanceId;	/**< Id of the pos 1 callback method */
};

#endif // UPDATEDVALUESRECEIVER_H
