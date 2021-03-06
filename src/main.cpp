/**
 * @file main.cpp
 * @brief The main file.
 * @author Andreas Ziegler
 */

/*************** Includes ************/
#include <memory>
#include <sched.h>
#include <cstring>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xmlreshandler.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include "main.h"
#include "./gui/xh_wxspinctrldoublexmlhandler.h"
#include "./gui/xh_mathplotxmlhandler.h"
#include "./gui/xh_wxmybuttonxmlhandler.h"
#include <wx/spinctrl.h>
#include "updatedvaluesreceiver.h"
#include "./hardware/linearstage.h"
#include "./hardware/linearstagemessagehandler.h"
#include "./hardware/forcesensor.h"
#include "./hardware/forcesensormessagehandler.h"
#include "./hardware/serialtrigger.h"

// An deleter which doesn't do anything, required for passing shared_ptr.
template<typename T>
void do_nothing_deleter(T *ptr){return;}

// Define main app.
IMPLEMENT_APP(MyApp)

/**
 * @brief Initialization
 * @return true if successful
 */
bool MyApp::OnInit(){
  if(!wxApp::OnInit()){
    return(false);
  }

  // Get all handlers
  //  wxXmlResource::AddHandler(new wxIconXmlHandler);
  wxXmlResource::Get()->AddHandler(new wxSpinCtrlDoubleXmlHandler);
  //wxXmlResource::Get()->AddHandler(new mpWindowXmlHandler);
  wxXmlResource::Get()->AddHandler(new wxMyButtonXmlHandler);
  wxXmlResource::Get()->InitAllHandlers();
  ::wxInitAllImageHandlers();
  // Load all xrc files from the subfolder gui.
  if(false == wxXmlResource::Get()->LoadAllFiles("gui")){
    return(false);
  }

  // Create the settings
  //m_MySettings = new Settings();

  // Create the main frame and show it
  MyFrame *m_MyFrame = new MyFrame("Stretcher", &m_MySettings);
  m_MyFrame->Show(true);

  // Create the linear motor objects.
  std::vector<std::shared_ptr<LinearStage>> m_LinearStages;
  m_LinearStages.push_back(std::make_shared<LinearStage>(UpdatedValuesReceiver::ValueType::Pos1,
                                           m_MyFrame->getMessageHandlersWait(),
                                           m_MyFrame->getMessageHandlersWaitMutex(),
                                           m_MyFrame->getMessageHandlersFinishedNumber(),
                                           m_MySettings.getLinMot1BaudRate()));
  m_LinearStages.push_back(std::make_shared<LinearStage>(UpdatedValuesReceiver::ValueType::Pos2,
                                           m_MyFrame->getMessageHandlersWait(),
                                           m_MyFrame->getMessageHandlersWaitMutex(),
                                           m_MyFrame->getMessageHandlersFinishedNumber(),
                                           m_MySettings.getLinMot2BaudRate()));
  m_StageFrame.registerLinearStages(m_LinearStages);
  m_LinearStages.at(0)->connect(m_MySettings.getLinMot1ComPort());
  m_LinearStages.at(1)->connect(m_MySettings.getLinMot2ComPort());
  m_LinearStages.at(0)->configure();
  m_LinearStages.at(1)->configure();
  // Create shared_ptr which is needed to pass as parameter.
  std::shared_ptr<StageFrame> stageframe(&m_StageFrame, do_nothing_deleter<StageFrame>);
  m_MyFrame->registerLinearStage(m_LinearStages, stageframe);

  // Get the message handlers for the linear stages.
  std::vector<std::shared_ptr<LinearStageMessageHandler>> m_LinearStagesMessageHandlers;
  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(0))->getMessageHandler());
  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(1))->getMessageHandler());
  m_MyFrame->registerLinearStageMessageHandlers(m_LinearStagesMessageHandlers);

  // Run the receivers of the linear stages in seperate threads.
  m_LinearStagesReceivers.push_back(std::thread(&LinearStageMessageHandler::receiver, m_LinearStagesMessageHandlers.at(0)));
  m_LinearStagesReceivers.push_back(std::thread(&LinearStageMessageHandler::receiver, m_LinearStagesMessageHandlers.at(1)));
  // Set thread priority to max
  /*
  sched_param sch;
  int policy;
  pthread_getschedparam(m_LinearStagesReceivers[0].native_handle(), &policy, &sch);
  sch.sched_priority = 99;
  if(pthread_setschedparam(m_LinearStagesReceivers[0].native_handle(), SCHED_RR, &sch)){
    std::cout << "Failed to setschedparam: " << std::strerror(errno) << std::endl;
  }
  pthread_getschedparam(m_LinearStagesReceivers[1].native_handle(), &policy, &sch);
  sch.sched_priority = 99;
  if(pthread_setschedparam(m_LinearStagesReceivers[1].native_handle(), SCHED_RR, &sch)){
    std::cout << "Failed to setschedparam: " << std::strerror(errno) << std::endl;
  }
  */
  m_LinearStagesReceivers.at(0).detach();
  m_LinearStagesReceivers.at(1).detach();

  // Create the force sensor object
  std::shared_ptr<ForceSensor> m_ForceSensor;
  m_ForceSensor = std::make_shared<ForceSensor>(UpdatedValuesReceiver::ValueType::Force,
                                  m_MyFrame->getMessageHandlersWait(),
                                  m_MyFrame->getMessageHandlersWaitMutex(),
                                  m_MyFrame->getMessageHandlersFinishedNumber(),
                                  m_MySettings.getForceSensorBaudRate());
  m_ForceSensor->connect(m_MySettings.getForceSensorComPort());
  m_ForceSensor->setBipolarMode();
  m_ForceSensor->setScaleFactor(m_MySettings.getForceSensorNominalForce(),
                                m_MySettings.getForceSensorNominalValue(),
                                m_MySettings.getForceSensorInputSensitivity(),
                                m_MySettings.getForceSensorMeasureEndValue(),
                                m_MySettings.getForceSensorZeroValue());
  m_MyFrame->registerForceSensor(m_ForceSensor);

  // Get the message handlers for the force sensor.
  std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;
  m_ForceSensorMessageHandler = m_ForceSensor->getMessageHandler();

  // Run the receiver of the force sensor in seperate threads.
  m_ForceSensorReceiver = std::thread(&ForceSensorMessageHandler::receiver, m_ForceSensorMessageHandler);
  m_ForceSensorReceiver.detach();

  // Create the serial trigger
  std::shared_ptr<SerialTrigger> m_SerialTrigger;
  m_SerialTrigger = std::make_shared<SerialTrigger>(m_MySettings.getSerialTriggerBaudRate());
  m_SerialTrigger->connect(m_MySettings.getSerialTriggerComPort());
  m_MyFrame->registerSerialTrigger(m_SerialTrigger);

  m_MyFrame->startup();

  return(true);
}

