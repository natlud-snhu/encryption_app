#include "mainframe.h"
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/file.h>
#include <wx/popupwin.h>
#include <string>
#include <array>
#include <vector>
#include "validation.h"
#include "password.h"
#include "encryptionsession.h"
#include "decryptionsession.h"

static wxSimplebook* main_book;
static wxTextCtrl* encryptpassword_entry;
static wxTextCtrl* encrypttext_entry;
static wxTextCtrl* decryptpassword_entry;
static wxTextCtrl* decryptmemory_view;
static std::string decrypt_file_path;
static DecryptionSession decrypt_session;

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size):wxFrame(nullptr, wxID_ANY, title, pos, size) {
	//create main layout containers
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	main_book = new wxSimplebook(this, wxID_ANY);
	main_sizer->Add(main_book, 1, wxEXPAND);
	//welcome page creation
	wxPanel* welcome_panel = new wxPanel(main_book, wxID_ANY);
	wxBoxSizer* welcome_sizer_ver = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* welcome_sizer_hor = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* welcome_text = new wxStaticText(welcome_panel, wxID_ANY, "Welcome!");
	welcome_text->SetFont(welcome_text->GetFont().Scale(1.5));
	wxStaticText* choose_text = new wxStaticText(welcome_panel, wxID_ANY, "Choose mode");
	choose_text->SetFont(choose_text->GetFont().Scale(1.25));
	wxButton* encrypt_button = new wxButton(welcome_panel, wxID_ANY, "Encrypt");
	wxButton* decrypt_button = new wxButton(welcome_panel, wxID_ANY, "Decrypt");
	welcome_sizer_ver->Add(welcome_text, 1, wxALIGN_CENTER | wxTOP, 10);
	welcome_sizer_ver->Add(choose_text, 2, wxALIGN_CENTER | wxTOP, 2.5);
	welcome_sizer_ver->Add(welcome_sizer_hor, 1, wxEXPAND | wxALL, 5);
	welcome_sizer_hor->Add(encrypt_button, 1, wxLEFT, 10);
	welcome_sizer_hor->AddStretchSpacer();
	welcome_sizer_hor->Add(decrypt_button, 1, wxRIGHT, 10);

	welcome_panel->SetSizer(welcome_sizer_ver);
	main_book->AddPage(welcome_panel, "Welcome");

	//Encrypt Page creation
	wxPanel* encrypt_panel = new wxPanel(main_book, wxID_ANY);
	wxBoxSizer* encryptsizer_main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* encryptsizer_hor = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* encryptsizer_text = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* encryptsizer_password = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* encryptsizer_refresh = new wxBoxSizer(wxVERTICAL);
	wxButton* encryptback_button = new wxBitmapButton(encrypt_panel, wxID_ANY, wxBitmap("back-arrow.png", wxBITMAP_TYPE_PNG), wxPoint(0, 0), wxSize(30, 30));
	encryptsizer_main->Add(encryptback_button, 0);
	wxStaticText* encrypttext_label = new wxStaticText(encrypt_panel, wxID_ANY, "Text");
	wxStaticText* encryptpassword_label = new wxStaticText(encrypt_panel, wxID_ANY, "Password");
	wxButton* encryptpassword_refresh = new wxBitmapButton(encrypt_panel, wxID_ANY, wxBitmap("refresh.png", wxBITMAP_TYPE_PNG), wxPoint(-1, -1), wxSize(30, 30));
	encrypttext_entry = new wxTextCtrl(encrypt_panel, wxID_ANY, wxEmptyString, wxPoint(-1, -1), wxSize(-1, 120), wxTE_MULTILINE);
	encryptpassword_entry = new wxTextCtrl(encrypt_panel, wxID_ANY, wxEmptyString, wxPoint(-1, -1), wxSize(-1, 30));
	encryptpassword_entry->SetMaxLength(16); //max length for passwords is 16
	wxButton* encryptmain_button = new wxButton(encrypt_panel, wxID_ANY, "Encrypt");
	encryptsizer_main->Add(encryptsizer_text, 1, (wxTOP, 10) | wxEXPAND);
	encryptsizer_text->Add(encrypttext_label, 0, wxEXPAND);
	encryptsizer_text->Add(encrypttext_entry, 1, wxEXPAND);
	encryptsizer_main->Add(encryptsizer_hor, 1, wxALIGN_CENTER_HORIZONTAL);
	encryptsizer_hor->Add(encryptsizer_password, 0, wxTOP, 10);
	encryptsizer_hor->Add(encryptsizer_refresh, 0, wxTOP, 10);
	encryptsizer_refresh->Add(encryptpassword_refresh, 1, wxTOP, encryptpassword_label->GetSize().y);
	encryptsizer_password->Add(encryptpassword_label);
	encryptsizer_password->Add(encryptpassword_entry);
	encryptsizer_main->Add(encryptmain_button, 0, wxEXPAND);

	encrypt_panel->SetSizer(encryptsizer_main);
	main_book->AddPage(encrypt_panel, "Encrypt");

	//Decryption Page
	wxPanel* decrypt_panel = new wxPanel(main_book, wxID_ANY);
	wxBoxSizer* decryptsizer_main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* decryptsizer_password = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* decryptsizer_memory = new wxBoxSizer(wxVERTICAL);
	wxButton* decryptback_button = new wxBitmapButton(decrypt_panel, wxID_ANY, wxBitmap("back-arrow.png", wxBITMAP_TYPE_PNG), wxPoint(0, 0), wxSize(30, 30));
	wxStaticText* decryptpassword_label = new wxStaticText(decrypt_panel, wxID_ANY, "Password");
	decryptpassword_entry = new wxTextCtrl(decrypt_panel, wxID_ANY, wxEmptyString, wxPoint(-1, -1), wxSize(-1, 30), wxTE_PASSWORD);
	decryptpassword_entry->SetMaxLength(16); 
	wxStaticText* decryptmemory_label = new wxStaticText(decrypt_panel, wxID_ANY, "Encrypted Memory");
	decryptmemory_view = new wxTextCtrl(decrypt_panel, wxID_ANY, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), wxTE_READONLY | wxTE_MULTILINE | wxTE_WORDWRAP);
	wxButton* decryptmain_button = new wxButton(decrypt_panel, wxID_ANY, "Decrypt");
	decrypt_panel->SetSizer(decryptsizer_main);
	decryptsizer_main->Add(decryptback_button, 0);
	decryptsizer_main->Add(decryptsizer_password, 1, (wxTOP, 10) | wxEXPAND);
	decryptsizer_password->Add(decryptpassword_label, 0, wxEXPAND);
	decryptsizer_password->Add(decryptpassword_entry, 0, wxEXPAND);
	decryptsizer_main->Add(decryptsizer_memory, 1, wxEXPAND);
	decryptsizer_memory->Add(decryptmemory_label);
	decryptsizer_memory->Add(decryptmemory_view, 1, wxEXPAND);
	decryptsizer_main->Add(decryptmain_button, 0, wxEXPAND);
	main_book->AddPage(decrypt_panel, "Decrypt");
	//initialize main_book
	main_book->SetSelection(0);
	SetSizerAndFit(main_sizer);

	//set bindings
	encrypt_button->Bind(wxEVT_BUTTON, &MainFrame::onEncryptButton, this);
	decrypt_button->Bind(wxEVT_BUTTON, &MainFrame::onDecryptButton, this);
	encryptback_button->Bind(wxEVT_BUTTON, &MainFrame::onBackButton, this);
	decryptback_button->Bind(wxEVT_BUTTON, &MainFrame::onBackButton, this);
	encryptpassword_refresh->Bind(wxEVT_BUTTON, &MainFrame::onRefreshButton, this);
	encryptmain_button->Bind(wxEVT_BUTTON, &MainFrame::onEncryptMainButton, this);
	decryptmain_button->Bind(wxEVT_BUTTON, &MainFrame::onDecryptMainButton, this);
};

void MainFrame::onEncryptButton(wxCommandEvent& event) {
	//resets fields
	encryptpassword_entry->ChangeValue(""); 
	encrypttext_entry->ChangeValue("");
	main_book->SetSelection(1);
};

void MainFrame::onDecryptButton(wxCommandEvent& event) {
	//choose encrypted file
	wxFileDialog dlg(this, "Open encrypted file...",
		"", "",
		"Encrypted File (*.tec) | *.tec", wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK) { //valid path was chosen
		decrypt_file_path = (std::string) dlg.GetPath();
	} else {
		return;
	}
	//create new decryption session
	decrypt_session = DecryptionSession(decrypt_file_path);
	//resets fields
	decryptpassword_entry->ChangeValue(""); 
	decryptmemory_view->ChangeValue("");
	//fill in memory view
	std::vector<std::string> converted_memory = decrypt_session.textToHex();
	int index = 0;
	for (int i = 0; i < converted_memory.size(); i++) {
		decryptmemory_view->AppendText(converted_memory.at(i));
		decryptmemory_view->AppendText(" ");
		if (index == 7) {
			index = 0;
			decryptmemory_view->AppendText("\n");
		}
		else {
			index++;
		}

	};
	main_book->SetSelection(2);
}

void MainFrame::onBackButton(wxCommandEvent& event) {
	main_book->SetSelection(0);
};

void MainFrame::onRefreshButton(wxCommandEvent& event) { 
	//uses the createPassword() function to set password field to random 16 alphanumerical characters
	encryptpassword_entry->ChangeValue(wxString(createPassword()));
};

void MainFrame::onEncryptMainButton(wxCommandEvent& event) {
	//get inputs
	std::string plain_text = (std::string)encrypttext_entry->GetValue();
	std::string password_text = (std::string)encryptpassword_entry->GetValue();
	//validate inputs
	if (!isInputValid(plain_text)) {
		wxMessageDialog* input_error = new wxMessageDialog(NULL,
			wxT("Invalid input. Text must be less than 1000 characters long and at least 1 character."), wxT("Error"), wxOK);
		input_error->ShowModal();
		return;
	}
	if (!isPasswordValid(password_text)) {
		wxMessageDialog* password_error = new wxMessageDialog(NULL,
			wxT("Invalid password. Password must be 16 characters long and only contain letters and numbers."), wxT("Error"), wxOK);
		password_error->ShowModal();
		return;
	};
	//begin writing data
	wxFileDialog dlg(this, "Save encrypted file...",
		"", "",
		"Encrypted File (*.tec) | *.tec", wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK) { //valid path was chosen
		std::string file_path = (std::string)dlg.GetPath();
		//create encryption session
		EncryptionSession encrypt_session(plain_text, password_text);
		//output encrypted file
		encrypt_session.saveToFile(file_path);
	} else {
		return;
	};
	wxMessageDialog* success = new wxMessageDialog(NULL,
		wxT("Successfully encrypted file"), wxT("Success"), wxOK);
	success->ShowModal();
	//reset to main page
	main_book->SetSelection(0);

};

void MainFrame::onDecryptMainButton(wxCommandEvent& event) {
	//get inputs
	std::string password_text = (std::string)decryptpassword_entry->GetValue();
	std::string save_file_path;
	//validate input
	if (!isPasswordValid(password_text)) {
		wxMessageDialog* password_error = new wxMessageDialog(NULL,
			wxT("Invalid password. Password must be 16 characters long and only contain letters and numbers."), wxT("Error"), wxOK);
		password_error->ShowModal();
		return;
	};
	//ask for save location
	wxFileDialog dlg(this, "Save decrypted file...",
	"", "",
		"Text file (*.txt) | *.txt", wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK) { //valid path was chosen
			save_file_path = (std::string)dlg.GetPath();
		} else {
			return;
		}
	//create decryption session
	try {
		decrypt_session.tryDecryption(password_text);
		if (!decrypt_session.decrypted) { //boolean if decryption was successful
			wxMessageDialog* password_error = new wxMessageDialog(NULL, "Password was incorrect.", wxT("Error"), wxOK);
			password_error->ShowModal();
			return;
		}
		decrypt_session.saveToFile(save_file_path);
	}
	catch (std::exception e) {
		std::string error_text = "Error saving file: \n ";
		error_text.append(decrypt_file_path);
		error_text.append("\n");
		error_text.append(e.what());
		wxMessageDialog* file_error = new wxMessageDialog(NULL, error_text, wxT("Error"), wxOK);
		file_error->ShowModal();
		return;
	};
	wxMessageDialog* success = new wxMessageDialog(NULL,
		wxT("Successfully decrypted file"), wxT("Success"), wxOK);
	success->ShowModal();
	//reset to main page
	main_book->SetSelection(0);
};