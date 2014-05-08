
// Includes
#include "mybutton.h"
#include "myframe.h"

// Event table
wxBEGIN_EVENT_TABLE(wxMyButton, wxButton)
  EVT_LEFT_DOWN(wxMyButton::OnButtonDown)
  EVT_LEFT_UP(wxMyButton::OnButtonUp)
wxEND_EVENT_TABLE()

/**
 * @brief Constuctor forwards the parameters to the base class wxButton.
 * @param parent Pointer to the parent object.
 * @param id Id of the object.
 * @param label Label of the object.
 * @param pos Position of the object.
 * @param size Size of the object.
 * @param style Style of the object.
 * @param validator Validator of the object.
 * @param name Name of the object.
 */
wxMyButton::wxMyButton(wxWindow *parent,
                       wxWindowID id,
                       const wxString& label,
                       const wxPoint& pos,
                       const wxSize& size, long style,
                       const wxValidator& validator,
                       const wxString& name)
: wxButton(parent, id, label, pos, size, style, validator, name)
{}

/**
 * @brief Registers the main frame which is needed to post the events to it.
 * @param myframe Pointer to the main frame.
 */
void wxMyButton::registerMyFrame(MyFrame *myframe){
  m_MyFrame = myframe;
}

/**
 * @brief Method wich will be executed, when the button is down.
 * @param event Occuring event
 */
void wxMyButton::OnButtonDown(wxMouseEvent& event){
  //std::cout << "wxMyButton button down." << std::endl;
  // Create event which will be sent.
  wxCommandEvent sendevent(EVT_MYBUTTON_DOWN, GetId());
  // Send event
  wxPostEvent(m_MyFrame, sendevent);
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the button is up.
 * @param event Occuring event
 */
void wxMyButton::OnButtonUp(wxMouseEvent& event){
  //std::cout << "wxMyButton button up." << std::endl;
  // Create event which will be sent.
  wxCommandEvent sendevent(EVT_MYBUTTON_UP, GetId());
  // Send event
  wxPostEvent(m_MyFrame, sendevent);
  event.Skip();
}
