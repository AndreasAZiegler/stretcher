/**
 * @file updatedvaluessender.cpp
 * @brief The update values sender.
 * @author Andreas Ziegler
 */

// Includes
#include "updatedvaluessender.h"

UpdatedValuesSender::UpdatedValuesSender(){}

UpdatedValuesSender::~UpdatedValuesSender(){}

/**
 * @brief Registers the update methods, which will be called, when the value changes.
 * @param updateMethod Method which updates a value.
 * @param updateClass Pointer to the object, to which the method belongs.
 * @return Id of the callback method.
 */
//void MessageHandler::registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass){
std::list<std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator UpdatedValuesSender::registerUpdateMethod(updateValue updateMethod, UpdatedValuesReceiver *updateClass){
  // Save the return of bin temporary to avoid memory leaks.
  auto tmp = std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2);
  std::lock_guard<std::mutex> lck{m_AccessListMutex};
  return(m_UpdateMethodList.insert(m_UpdateMethodList.end(), tmp));
}

/**
 * @brief Unregisters the update methods, which will be called, when the value changes.
 */
void UpdatedValuesSender::unregisterUpdateMethod(std::list<std::function<void(UpdatedValues::MeasurementValue, UpdatedValuesReceiver::ValueType)>>::iterator id){
  //m_UpdateMethodList.remove(std::bind(updateMethod, updateClass, std::placeholders::_1, std::placeholders::_2));
  std::lock_guard<std::mutex> lck{m_AccessListMutex};
  m_UpdateMethodList.erase(id);
}
