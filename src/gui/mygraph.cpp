#include "mygraph.h"

wxMyGraph::wxMyGraph(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name),
			m_Graph(nullptr)
{
	//m_Plot = std::unique_ptr<realtimeplot::Plot>(new realtimeplot::Plot());
	m_Graph = new realtimeplot::Plot(true);

	Connect(this->GetId(), wxEVT_PAINT, wxPaintEventHandler(wxMyGraph::OnPaint));
	Connect(this->GetId(), wxEVT_SIZE, wxSizeEventHandler(wxMyGraph::OnResize));
}

wxMyGraph::~wxMyGraph(){
}

void wxMyGraph::OnPaint(wxPaintEvent &event){
	//m_Plot.line_add( 10, 10, 1 );
}

void wxMyGraph::OnResize(wxSizeEvent &event){
	Refresh();
	event.Skip();
}
