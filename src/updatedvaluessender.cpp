#include "updatedvaluessender.h"

UpdatedValuesSender::UpdatedValuesSender()
{
}

/**
 * @brief Registers the update methods, which will be called, when the value changes.
 * @param updateMethod Method which updates a value.
 * @param updateClass Pointer to the object, to which the method belongs.
 * @return Id of the callback method.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator UpdatedValuesSender::registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass){
  return(m_UpdateMethodList.insert(m_UpdateMethodList.end(), std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2)));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void UpdatedValuesSender::unregisterUpdateMethod(std::list<std::function<void(long, UpdatedValuesReceiver::ValueType)>>::iterator id){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
  m_UpdateMethodList.erase(id);
}
