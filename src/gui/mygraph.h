#ifndef WXMYGRAPH_H
#define WXMYGRAPH_H

#include <wx/wx.h>
#include "realtimeplot/plot.h"

class wxMyGraph : public wxPanel {
 public:
  wxMyGraph(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxNO_BORDER, const wxString& name = wxPanelNameStr);

  wxMyGraph(wxMyGraph const &) = delete;
  wxMyGraph &operator=(wxMyGraph const &) = delete;

  ~wxMyGraph();

  void OnPaint(wxPaintEvent& event);
  void OnResize(wxSizeEvent& event);

	private:
		//std::unique_ptr<realtimeplot::PlotConfig> m_PlotConfig;
		//std::unique_ptr<realtimeplot::Plot> m_Plot;
		realtimeplot::Plot *m_Graph;
};

#endif // WXMYGRAPH_H
