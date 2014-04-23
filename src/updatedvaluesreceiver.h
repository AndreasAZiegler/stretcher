#ifndef UPDATEDVALUESRECEIVER_H
#define UPDATEDVALUESRECEIVER_H

#include <list>
#include <functional>

/**
 * @brief Abstract class used as an interface, that the message handlers can use the updateValue() method of the receiver in a standard way.
 */
class UpdatedValuesReceiver
{
  public:
    /**
     * @brief Defines the 3 types of measurement values.
     */
    enum class ValueType{
      Pos1 = 0,
      Pos2 = 1,
      Distance = 2,
      Force = 3
    };

    UpdatedValuesReceiver();

    virtual ~UpdatedValuesReceiver();

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(long value, UpdatedValuesReceiver::ValueType type) = 0;

  protected:
    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>> m_UpdateMethodList;			/**< List containing struct with pointers to the MyFrame object and the update method*/

    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator m_ForceId;			/**< Id of the force callback method */
    std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator m_DistanceId;	/**< Id of the pos 1 callback method */
};

#endif // UPDATEDVALUESRECEIVER_H
