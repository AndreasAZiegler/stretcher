#ifndef UPDATEDVALUESSENDER_H
#define UPDATEDVALUESSENDER_H

// Includes
#include <functional>
#include <list>
#include <updatedvaluesreceiver.h>

// Method pointer typedef
typedef void (UpdatedValuesReceiver::*updateValue)(long, UpdatedValuesReceiver::ValueType);
typedef std::function<void(long, UpdatedValuesReceiver::ValueType)> mp;

/**
 * @brief Abstract class used as an interface, that the message handlers can use the updateValue() method of the receiver in a standard way.
 */
class UpdatedValuesSender
{
  public:
    UpdatedValuesSender();

    /**
     * @brief Registers the update methods, which will be called, when the value changes.
     * @param updateMethod Method which updates a value.
     * @param updateClass Pointer to the object, to which the method belongs.
     * @return Id of the callback method.
     */
    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass);

    /**
     * @brief Unregisters the update methods, which will be called, when the value changes.
     * @param id Id of the callback method
     */
    void unregisterUpdateMethod(std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator id);

  protected:
    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>> m_UpdateMethodList;	/**< List containing struct with pointers to the MyFrame object and the update method*/
};

#endif // UPDATEDVALUESSENDER_H
