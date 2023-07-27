#pragma once
#include <wx/wx.h>

class Encrypt_Panel : public wxWindow
{
public:
	Encrypt_Panel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
	std::string text;
private:
	void BindEvents();

	void OnPaint(wxPaintEvent& evt);
	void DrawOnContext(wxGraphicsContext& gc);
	void OnMouseEnterWindow(wxMouseEvent& evt);
	void OnMouseLeaveWindow(wxMouseEvent& evt);

	bool isHovered;

};

