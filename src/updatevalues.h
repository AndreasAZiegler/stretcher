#ifndef UPDATEVALUES_H
#define UPDATEVALUES_H

#include <list>
#include <functional>

/**
 * @brief Abstract class used as an interface, that the message handlers can use the updateValue() method of the receiver in a standard way.
 */
class UpdateValues
{
  public:
    /**
     * @brief Defines the 3 types of measurement values.
     */
    enum class ValueType{
      Pos1 = 0,
      Pos2 = 1,
      Force = 2
    };

    UpdateValues();

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValue(int value, UpdateValues::ValueType type) = 0;

  protected:
    std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator m_ForceId;	/**< Id of the force callback method */
    std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator m_Pos1Id;	/**< Id of the pos 1 callback method */
    std::list<std::function<void(int, UpdateValues::ValueType)>>::iterator m_Pos2Id;	/**< Id of the pos 2 callback method */
};

#endif // UPDATEVALUES_H
