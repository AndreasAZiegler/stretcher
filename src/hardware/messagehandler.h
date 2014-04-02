
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

// Includes
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include <mutex>
#include <list>
#include <../updatevalues.h>

// Method pointer typedef
typedef void (UpdateValues::*updateValue)(int, UpdateValues::ValueType);

/**
 * @brief Abstract class for the message handler
 */
class MessageHandler
{
  public:

    /**
     * @brief Initializes the pointer to the serial port.
     * @param serialPort Pointer to the serial port owned by the serial interface class
     */
    MessageHandler(wxSerialPort *serialPort, UpdateValues::ValueType type, std::mutex *readingSerialInterfaceMutex);

    /**
     * @brief Receiving method (Should be executed in a own thread). Listen to the serial port and forwards the received messages to the handler.
     */
    void receiver(void);

    /**
     * @brief Registers the update methods, which will be called, when the value changes.
     */
    void registerUpdateMethod(updateValue updateMethod, UpdateValues *updateClass);

    /**
     * @brief Unregisters the update methods, which will be called, when the value changes.
     */
    void unregisterUpdateMethod(updateValue mp, UpdateValues *updatevalues);

  protected:

    /**
     * @brief Struct to store a pointer to the MyFrame object and a pointer to the updateStagePosValue method.
     */
    struct UpdateValuesStruct{
        /**
         * @brief Initializes method and myframe.
         * @param imethod Pointer to the updateStagePosValue method.
         * @param imyframe Pointer to the MyFrame object.
         */
        UpdateValuesStruct(updateValue imethod, UpdateValues *iupdatevalues){
          method = imethod;
          updatevalues = iupdatevalues;
        }
        UpdateValuesStruct(const UpdateValuesStruct& other){
          method = other.method;
          updatevalues = other.updatevalues;
        }

        bool operator==(UpdateValuesStruct i){
         if((this->method == i.method) && (this->updatevalues == i.updatevalues)){
           return(true);
         }else{
           return(false);
         }
        }

        updateValue method;
        UpdateValues *updatevalues;
    };

    /**
     * @return bool
     * @param  message
     */
    virtual void handler(char *message) = 0;


    wxSerialPort	*m_SerialPort;									/**< Pointer to the serial port object */
    UpdateValues::ValueType m_Type;								/**< Defines if serial interface represents linear stage 1, 2 or the force sensor */
    std::mutex *m_ReadingSerialInterfaceMutex;		/**< Pointer to the mutex to protect serial interface of simultanously writing. */
    std::list<UpdateValuesStruct> m_UpdateMethodList; /**< List containing struct with pointers to the MyFrame object and the update method*/


    struct TestStruct{
      TestStruct(int ivalue){
        value = ivalue;
      }

      int value;
    };

    std::list<TestStruct> test {1, 2};
    UpdateValues *m_UpdateClass;
    updateValue m_UpdateMethod;
};

#endif // MESSAGEHANDLER_H
