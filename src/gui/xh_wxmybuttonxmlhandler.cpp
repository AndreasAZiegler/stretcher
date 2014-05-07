// Includes
#include <wx/defs.h>
#include "mybutton.h"
#include "xh_wxmybuttonxmlhandler.h"

// Register with wxWidgets' dynamic class subsystem.
IMPLEMENT_DYNAMIC_CLASS(wxMyButtonXmlHandler, wxXmlResourceHandler)

#include "xh_wxmybuttonxmlhandler.h"

wxMyButtonXmlHandler::wxMyButtonXmlHandler()
  : wxXmlResourceHandler()
{
  // this call adds support for all wxWidgets class styles
  // (e.g. wxBORDER_SIMPLE, wxBORDER_SUNKEN, wxWS_EX_* etc etc)
  AddWindowStyles();

  // if MyControl class supports e.g. MYCONTROL_DEFAULT_STYLE
  // you should use:
  //     XRC_ADD_STYLE(MYCONTROL_DEFAULT_STYLE);
}
wxObject *wxMyButtonXmlHandler::DoCreateResource()
{
    // the following macro will init a pointer named "control"
    // with a new instance of the MyControl class, but will NOT
    // Create() it!
    XRC_MAKE_INSTANCE(control, wxMyButton)

    // this is the point where you'll typically need to do the most
    // important changes: here the control is created and initialized.
    // You'll want to use the wxXmlResourceHandler's getters to
    // do most of your work.
    // If e.g. the MyControl::Create function looks like:
    //
    // bool MyControl::Create(wxWindow *parent, int id,
    //                        const wxBitmap &first, const wxPoint &posFirst,
    //                        const wxBitmap &second, const wxPoint &posSecond,
    //                        const wxString &theTitle, const wxFont &titleFont,
    //                        const wxPoint &pos, const wxSize &size,
    //                        long style = MYCONTROL_DEFAULT_STYLE,
    //                        const wxString &name = wxT("MyControl"));
    //
    // Then the XRC for your component should look like:
    //
    //    <object class="MyControl" name="some_name">
    //      <first-bitmap>first.xpm</first-bitmap>
    //      <second-bitmap>text.xpm</second-bitmap>
    //      <first-pos>3,3</first-pos>
    //      <second-pos>4,4</second-pos>
    //      <the-title>a title</the-title>
    //      <title-font>
    //        <!-- Standard XRC tags for a font: <size>, <style>, <weight>, etc -->
    //      </title-font>
    //      <!-- XRC also accepts other usual tags for wxWindow-derived classes:
    //           like e.g. <name>, <style>, <size>, <position>, etc -->
    //    </object>
    //
    // And the code to read your custom tags from the XRC file is just:
    control->Create(m_parentAsWindow, GetID(),
                    GetText(wxT("label")),
                    GetPosition(),
                    GetSize(),
                    GetStyle());

        SetupWindow(control);
        return(control);
}

bool wxMyButtonXmlHandler::CanHandle(wxXmlNode *node)
{
  // this function tells XRC system that this handler can parse
  // the <object class="MyControl"> tags
  return IsOfClass(node, wxT("wxMyButton"));
}
