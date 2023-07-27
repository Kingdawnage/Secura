#include "MainFrame.h"
#include "Cryptograph.h"
#include "Encrypt_Panel.h"
#include "Decrypt_Panel.h"
#include <fstream>
#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>


MainFrame::MainFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindEvents();

}

void MainFrame::CreateControls() //UI design function
{
	this->SetMaxSize(wxSize(-1, 1000));

	auto panel_right = new wxPanel(this, wxID_ANY);
	panel_right->SetBackgroundColour(wxColour(30, 30, 30));
	auto panel_in_right = new wxPanel(panel_right, wxID_ANY);
	panel_in_right->SetBackgroundColour(wxColour(30, 30, 30));

	auto panel_left = new wxPanel(this, wxID_ANY);
	panel_left->SetBackgroundColour(wxColour(30, 30, 30));
	auto panel_in_left = new wxPanel(panel_left, wxID_ANY);
	panel_in_left->SetBackgroundColour(wxColour(30, 30, 30));

	auto main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(panel_right, 1, wxEXPAND | wxALL, 0);
	main_sizer->Add(panel_left, 1, wxEXPAND | wxALL, 0);

	encryptbtn = new Encrypt_Panel(panel_in_right, wxID_ANY, wxPoint(-1, -1), wxSize(400, 150));
	encryptbtn->text = "Encrypt";
	encryptbtn->Layout();

	decryptbtn = new Decrypt_Panel(panel_in_left, wxID_ANY, wxPoint(-1, -1), wxSize(400, 150));
	decryptbtn->text = "Decrypt";
	decryptbtn->Layout();

	auto right_sizer = new wxBoxSizer(wxVERTICAL);
	right_sizer->Add(panel_in_right, 1, wxEXPAND | wxALL, 150);
	panel_right->SetSizerAndFit(right_sizer);

	auto right_sizer2 = new wxBoxSizer(wxVERTICAL);
	panel_in_right->SetSizerAndFit(right_sizer2);
	right_sizer2->Add(encryptbtn, 1, wxEXPAND | wxALL, 0);

	auto left_sizer = new wxBoxSizer(wxVERTICAL);
	left_sizer->Add(panel_in_left, 1, wxEXPAND | wxALL, 150);
	panel_left->SetSizerAndFit(left_sizer);

	auto left_sizer2 = new wxBoxSizer(wxVERTICAL);
	panel_in_left->SetSizerAndFit(left_sizer2);
	left_sizer2->Add(decryptbtn, 1, wxEXPAND | wxALL, 0);

	this->SetSizerAndFit(main_sizer);
}

void MainFrame::BindEvents()
{
	encryptbtn->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnEncryptButtonClicked, this);
	decryptbtn->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnDecryptButtonClicked, this);
}

std::string MainFrame::OnOpen()
{
	wxFileDialog fileDiag(this, "Select Open File", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	int result = fileDiag.ShowModal();

	if (result == wxID_CANCEL) {
		return "";
	}

	wxFileInputStream input_stream(fileDiag.GetPath());
	if (!input_stream.IsOk()) {
		wxLogError("Cannot open file '%s'.", fileDiag.GetPath());
		return "";
	}

	wxString OpenPathStringData = fileDiag.GetPath();
	std::string OpenPath = OpenPathStringData.ToStdString();
	return OpenPath;
}

std::string MainFrame::OnSave()
{
	wxFileDialog fileDiag(this, "Select Save Directory", "", "", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int result = fileDiag.ShowModal();

	if (result == wxID_CANCEL) {
		return "";
	}

	wxFileOutputStream output_stream(fileDiag.GetPath());
	if (!output_stream.IsOk()) {
		wxLogError("Cannot save current contents in file '%s'.", fileDiag.GetPath());
		return "";
	}

	wxString SavePathStringData = fileDiag.GetPath();
	std::string SavePath = SavePathStringData.ToStdString();
	return SavePath;
}


void MainFrame::OnEncryptButtonClicked(wxMouseEvent& evt)
{
	wxString password = wxGetPasswordFromUser("Enter the encryption password: ", "Encryption Password", wxEmptyString, this);
	if (password.IsEmpty()) {
		return;
	}

	std::vector<unsigned char> salt(EVP_MAX_KEY_LENGTH);
	if (RAND_bytes(salt.data(), EVP_MAX_KEY_LENGTH) != 1) {
		wxLogError("Failed to generate salt.");
		return;
	}

	std::vector<unsigned char> key = Keygen(password.ToStdString(), salt);
	std::string openPath = OnOpen();
	if (openPath.empty()) {
		return;
	}

	std::string savePath = OnSave();
	if (savePath.empty()) {
		return;
	}

	FileEncrypt(openPath, savePath, password.ToStdString());
	evt.Skip();
}

void MainFrame::OnDecryptButtonClicked(wxMouseEvent& evt)
{
	wxString password = wxGetPasswordFromUser("Enter the decryption password: ", "Encryption Password", wxEmptyString, this);
	if (password.IsEmpty()) {
		return;
	}

	std::string openPath = OnOpen();
	if (openPath.empty()) {
		return;
	}

	std::string savePath = OnSave();
	if (savePath.empty()) {
		return;
	}

	FileDecrypt(openPath, savePath, password.ToStdString());
	evt.Skip();
}