/*************** Includes ************/
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xmlreshandler.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include "main.h"
#include "./gui/xh_wxspinctrldoublexmlhandler.h"
#include "./gui/xh_mathplotxmlhandler.h"
#include <wx/spinctrl.h>
#include "updatevalues.h"

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
  wxXmlResource::Get()->AddHandler(new mpWindowXmlHandler);
  wxXmlResource::Get()->InitAllHandlers();
  // Load all xrc files from the subfolder gui.
  if(false == wxXmlResource::Get()->LoadAllFiles("gui")){
    return(false);
  }

  // Create the settings
  //m_MySettings = new Settings();

  // Create the main frame and show it
  m_MyFrame = new MyFrame("Stretcher", &m_MySettings);
  m_MyFrame->Show(true);
  m_MyFrame->startup();

  // Create the linear motor objects.
  m_LinearStages.push_back(new LinearStage(UpdateValues::ValueType::Pos1, m_MySettings.getLinMot1BaudRate()));
  m_LinearStages.push_back(new LinearStage(UpdateValues::ValueType::Pos2, m_MySettings.getLinMot2BaudRate()));
  m_StageFrame.registerLinearStages(&m_LinearStages);
  m_LinearStages.at(0)->connect(m_MySettings.getLinMot1ComPort());
  m_LinearStages.at(1)->connect(m_MySettings.getLinMot2ComPort());
  m_LinearStages.at(0)->configure();
  m_LinearStages.at(1)->configure();
  m_MyFrame->registerLinearStage(&m_LinearStages, &m_StageFrame);

  // Get the message handlers for the linear stages.
  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(0))->getMessageHandler());
  m_LinearStagesMessageHandlers.push_back((m_LinearStages.at(1))->getMessageHandler());
  (m_LinearStages.at(0))->registerOtherMessageHandler(m_LinearStagesMessageHandlers.at(1));
  (m_LinearStages.at(1))->registerOtherMessageHandler(m_LinearStagesMessageHandlers.at(0));

  // Run the receivers of the linear stages in seperate threads.
  m_LinearStagesReceivers.push_back(std::thread(&LinearStageMessageHandler::receiver, m_LinearStagesMessageHandlers.at(0)));
  m_LinearStagesReceivers.push_back(std::thread(&LinearStageMessageHandler::receiver, m_LinearStagesMessageHandlers.at(1)));
  m_LinearStagesReceivers.at(0).detach();
  m_LinearStagesReceivers.at(1).detach();

  // Create the force sensor object
  m_ForceSensor = new ForceSensor(UpdateValues::ValueType::Force, m_MySettings.getForceSensorBaudRate());
  m_ForceSensor->connect(m_MySettings.getForceSensorComPort());
  m_ForceSensor->setBipolarMode();
  m_ForceSensor->setScaleFactor(m_MySettings.getForceSensorNominalForce(),
                                m_MySettings.getForceSensorNominalValue(),
                                m_MySettings.getForceSensorInputSensitivity(),
                                m_MySettings.getForceSensorMeasureEndValue(),
                                m_MySettings.getForceSensorZeroValue());
  m_MyFrame->registerForceSensor(m_ForceSensor);

  // Get the message handlers for the force sensor.
  m_ForceSensorMessageHandler = m_ForceSensor->getMessageHandler();

  // Run the receiver of the force sensor in seperate threads.
  m_ForceSensorReceiver = std::thread(&ForceSensorMessageHandler::receiver, m_ForceSensorMessageHandler);
  m_ForceSensorReceiver.detach();


  return(true);
}

