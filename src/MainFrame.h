#pragma once
#include <wx/wx.h>
#include "Decrypt_Panel.h"
#include "Encrypt_Panel.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void BindEvents();

	std::string OnOpen();
	std::string OnSave();

	void OnEncryptButtonClicked(wxMouseEvent& evt);
	void OnDecryptButtonClicked(wxMouseEvent& evt);

	Decrypt_Panel* decryptbtn;
	Encrypt_Panel* encryptbtn;
};

