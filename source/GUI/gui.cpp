#include <wx/wx.h>
#include "mainframe.h"
#include "gui.h"
#include <string>

bool App::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	MainFrame* mainFrame = new MainFrame(wxString("Test App"), wxPoint(500,300), wxSize(-1, -1));
	mainFrame->Show();
	return true;
}

wxIMPLEMENT_APP(App);


/*
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "validation.h"
#include "password.h"
#include "encryptionsession.h"
#include "decryptionsession.h"


int main() {
	try {
		//char user_input[25565];
		//std::cout << "Would you like to Encrypt, Decrypt, or Exit? \n";
		//std::cin.getline(user_input, 25565);
		//std::cout << user_input << "\n";
	std::string password = createPassword();
	std::cout << password;
	std::string text = "Hello world! This is a pretty long string now.";
	std::string encryptfilepath = "C:\\Users\\shado\\Documents\\encrypted.txt";
	std::string decryptfilepath = "C:\\Users\\shado\\Documents\\decrypted.txt";
	EncryptionSession encrypt_session(text, password);;
	encrypt_session.saveToFile(encryptfilepath);
	DecryptionSession decrypt_session(encryptfilepath, password);
	decrypt_session.saveToFile(decryptfilepath);

	}
	catch (std::exception e) {
		std::cout << e.what() << "\n";
	};
	return 0;
};
*/