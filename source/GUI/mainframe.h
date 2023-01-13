#pragma once
#include <wx/wx.h>
#include <string>

class MainFrame:public wxFrame {
private:
	void onDecryptButton(wxCommandEvent& event);
	void onEncryptButton(wxCommandEvent& event);
	void onRefreshButton(wxCommandEvent& event);
	void onBackButton(wxCommandEvent& event);
	void onEncryptMainButton(wxCommandEvent& event);
	void onDecryptMainButton(wxCommandEvent& event);
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};