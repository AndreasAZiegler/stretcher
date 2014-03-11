/*************** Includes ************/
#include <wx/wx.h>
#include <wx/frame.h>
#include "main.h"
#include "./gui/myframe.h"

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
  wxXmlResource::Get()->InitAllHandlers();
  // Load all xrc files from the subfolder gui.
  if(false == wxXmlResource::Get()->LoadAllFiles("gui")){
    return(false);
  }

  // Create the main frame and show it
  MyFrame *myFrame = new MyFrame("Stretcher");
  myFrame->Show(true);
  return(true);
}

