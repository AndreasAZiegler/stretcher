#ifndef XH_WXMYBUTTONXMLHANDLER_H
#define XH_WXMYBUTTONXMLHANDLER_H

#include <wx/xrc/xmlres.h>
#include <wx/xrc/xmlreshandler.h>

class wxMyButtonXmlHandler : public wxXmlResourceHandler
{
  public:
    wxMyButtonXmlHandler();

    // Creates the control and returns a pointer to it.
    virtual wxObject *DoCreateResource();

    // Returns true if we know how to create a control for the given node.
    virtual bool CanHandle(wxXmlNode *node);

    // Register with wxWidgets' dynamic class subsystem.
    DECLARE_DYNAMIC_CLASS(wxMyButtonXmlHandler)
};

#endif // XH_WXMYBUTTONXMLHANDLER_H
