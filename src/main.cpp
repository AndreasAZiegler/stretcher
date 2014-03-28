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

  // Create the linear motor objects
  m_LinearStages.push_back(new LinearStage(115200/*baudrate*/));
  m_LinearStages.push_back(new LinearStage(115200/*baudrate*/));
  m_LinearStages.at(0)->connect(m_MySettings.getLinMot1ComPort());
  m_LinearStages.at(1)->connect(m_MySettings.getLinMot2ComPort());
  m_MyFrame->registerLinearStage(&m_LinearStages);

  // Create the force sensor object
  /**
    @todo Creating force sensor
    */

  return(true);
}

