#ifndef MYBUTTON_H
#define MYBUTTON_H

// Includes
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/window.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/validate.h>

// Forward declaration
class MyFrame;

/**
 * @brief Button class which implements the EVT_MYBUTTON_DOWN and EVT_MYBUTTON_UP events.
 */
class wxMyButton: public wxButton
{
	public:
		wxMyButton() {}

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
    wxMyButton(wxWindow *parent, wxWindowID id,
           const wxString& label = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr);

    /**
     * @brief Registers the main frame which is needed to post the events to it.
     * @param myframe Pointer to the main frame.
     */
    void registerMyFrame(MyFrame *myframe);

  private:
    /**
     * @brief Method wich will be executed, when the button is down.
     * @param event Occuring event
     */
    void OnButtonDown(wxMouseEvent& event);

    /**
     * @brief Method wich will be executed, when the button is up.
     * @param event Occuring event
     */
    void OnButtonUp(wxMouseEvent& event);

    MyFrame *m_MyFrame;																			/**< Pointer to the main frame. */

    wxDECLARE_EVENT_TABLE();
};

wxDECLARE_EVENT(EVT_MYBUTTON_DOWN, wxCommandEvent);
wxDECLARE_EVENT(EVT_MYBUTTON_UP, wxCommandEvent);

#endif // MYBUTTON_H
