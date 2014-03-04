#include <wx/wx.h>
#include <wx/frame.h>
#include "main.h"
#include "./gui/myframe.h"
#include "./gui/resource.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(){
  if(!wxApp::OnInit()){
    return(false);
  }

  wxXmlResource::Get()->InitAllHandlers();
  if(false == wxXmlResource::Get()->LoadAllFiles("gui")){
    return(false);
  }

  MyFrame *myFrame = new MyFrame("Stretcher");
  myFrame->Show(true);
  return(true);
}

