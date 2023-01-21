#include <wx/wx.h>
#include "mainframe.h"
#include "gui.h"
#include <string>

bool App::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	MainFrame* mainFrame = new MainFrame(wxString("Encryption App"), wxPoint(500,300), wxSize(-1, -1));
	mainFrame->Show();
	return true;
}

wxIMPLEMENT_APP(App);

