/**
 * @file updatedvaluessender.h
 * @brief The update values sender.
 * @author Andreas Ziegler
 */

#ifndef UPDATEDVALUESSENDER_H
#define UPDATEDVALUESSENDER_H

// Includes
#include <functional>
#include <mutex>
#include <list>
#include <chrono>
#include <updatedvalues.h>
#include <updatedvaluesreceiver.h>

// Method pointer typedef
typedef void (UpdatedValuesReceiver::*updateValue)(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType);
typedef std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)> mp;

/**
 * @brief Abstract class used as an interface, that the message handlers can use the updateValue() method of the receiver in a standard way.
 */
class UpdatedValuesSender : virtual public UpdatedValues
{
  public:

    UpdatedValuesSender();

    virtual ~UpdatedValuesSender();

    /**
     * @brief Registers the update methods, which will be called, when the value changes.
     * @param updateMethod Method which updates a value.
     * @param updateClass Pointer to the object, to which the method belongs.
     * @return Id of the callback method.
     */
    std::list<std::function<void(UpdatedValuesReceiver::MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass);

    /**
     * @brief Unregisters the update methods, which will be called, when the value changes.
     * @param id Id of the callback method
     */
    void unregisterUpdateMethod(std::list<std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator id);

  protected:
    std::list<std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)>> m_UpdateMethodList;	/**< List containing struct with pointers to the MyFrame object and the update method*/

    std::mutex m_AccessListMutex;				/**< Protect list */
};

#endif // UPDATEDVALUESSENDER_H
