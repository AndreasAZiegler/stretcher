/**
 * @file xh_wxspinctrldoublexmlhandler.h
 * @brief The XML handler for the wxSpinCtrlDouble.
 * @author Andreas Ziegler
 */

#ifndef XH_WXSPINCTRLDOUBLEXMLHANDLER_H
#define XH_WXSPINCTRLDOUBLEXMLHANDLER_H

// Includes
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xmlreshandler.h>

/**
 * @class wxSpinCtrlXmlHandler xh_wxspinctrlxmlhandler.h "gui/xh_wxspinctrlxmlhandler.h"
 * @brief The XML handler for the wxSpinCtrlDouble element.
 */
class wxSpinCtrlDoubleXmlHandler : public wxXmlResourceHandler
{
  public:
    /**
     * @brief Adds support for all wxWidgets class styles.
     */
    wxSpinCtrlDoubleXmlHandler();

    // Creates the control and returns a pointer to it.
    virtual wxObject *DoCreateResource();

    // Returns true if we know how to create a control for the given node.
    virtual bool CanHandle(wxXmlNode *node);

    // Register with wxWidgets' dynamic class subsystem.
    DECLARE_DYNAMIC_CLASS(wxSpinCtrlDoubleXmlHandler)
};

#endif // XH_WXSPINCTRLDOUBLEXMLHANDLER_H
