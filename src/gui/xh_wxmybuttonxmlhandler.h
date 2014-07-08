/**
 * @file xh_wxmybuttonxmlhandler.h
 * @brief The XML handler for the wxMyButton element.
 * @author Andreas Ziegler
 */

#ifndef XH_WXMYBUTTONXMLHANDLER_H
#define XH_WXMYBUTTONXMLHANDLER_H

// Includes
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xmlreshandler.h>

/**
 * @class wxMyButtonXmlHandler xh_wxmybuttonxmlhandler.h "gui/xh_wxmybuttonxmlhandler.h"
 * @brief The XML handler for the MyButton element.
 */
class wxMyButtonXmlHandler : public wxXmlResourceHandler
{
  public:
    /**
     * @brief Adds support for all wxWidgets class styles.
     */
    wxMyButtonXmlHandler();

    /**
     * @brief Creates the control and returns a pointer to it.
     * @return The Object
     */
    virtual wxObject *DoCreateResource();

    // Returns true if we know how to create a control for the given node.
    /**
     * @brief Tells the XRC system that this handler can parse the tags.
     * @param node The XML node
     * @return True it the handler can parse the tags / False otherwise
     */
    virtual bool CanHandle(wxXmlNode *node);

    // Register with wxWidgets' dynamic class subsystem.
    DECLARE_DYNAMIC_CLASS(wxMyButtonXmlHandler)
};

#endif // XH_WXMYBUTTONXMLHANDLER_H
