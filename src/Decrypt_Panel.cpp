#include "Decrypt_Panel.h"
#include <wx/wx.h>
#include <wx/graphics.h>

Decrypt_Panel::Decrypt_Panel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow()
{
	this->SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
	wxWindow::Create(parent, id, pos, size);

	//This was necessary to set a tranparent background for the button
#if defined(__WXMSW__)
	int extendedStyle = GetWindowLong(GetHWND(), GWL_EXSTYLE);
	SetWindowLong(GetHWND(), GWL_EXSTYLE, extendedStyle | WS_EX_TRANSPARENT);
#endif

	BindEvents();
}

void Decrypt_Panel::BindEvents()
{
	this->Bind(wxEVT_PAINT, &Decrypt_Panel::OnPaint, this);
	this->Bind(wxEVT_ENTER_WINDOW, &Decrypt_Panel::OnMouseEnterWindow, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Decrypt_Panel::OnMouseLeaveWindow, this);
}

void Decrypt_Panel::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);

	auto gc = wxGraphicsContext::Create(dc);
	if (gc) {
		DrawOnContext(*gc);
		delete gc;
	}
	evt.Skip();
}

//Button Design
void Decrypt_Panel::DrawOnContext(wxGraphicsContext& gc)
{
	wxColour bgcolor;
	wxFont textfont;
	if (isHovered) {
		bgcolor = wxColour(200, 90, 90);
		textfont = wxFontInfo({ 0, GetSize().GetHeight() / 3 }).FaceName("Comic Sans MS");
		gc.SetFont(textfont, wxColour(255, 255, 255));
	}
	else
	{
		bgcolor = wxColour(195, 18, 18);
		textfont = wxFontInfo({ 0, GetSize().GetHeight() / 3 }).FaceName("Comic Sans MS");
		gc.SetFont(textfont, wxColour(255, 255, 255));
	}

	wxPoint pnlOrigin = this->GetPosition();
	wxSize pnlSize = this->GetSize();

	int rad = wxMin(GetSize().GetWidth(), GetSize().GetHeight()) / 4;
	gc.SetBrush(wxBrush(bgcolor));
	gc.DrawRoundedRectangle(pnlOrigin.x, pnlOrigin.y, pnlSize.GetWidth(), pnlSize.GetHeight(), rad);

	double textWidth, textHeight;
	gc.GetTextExtent(this->text, &textWidth, &textHeight);
	gc.Clip(pnlOrigin.x, pnlOrigin.y, pnlSize.GetWidth(), pnlSize.GetHeight());
	gc.DrawText(this->text, pnlOrigin.x + pnlSize.GetWidth() / 2.0 - textWidth / 2.0, pnlOrigin.y + pnlSize.GetHeight() / 2.0 - textHeight / 2.0);
}

void Decrypt_Panel::OnMouseEnterWindow(wxMouseEvent& evt)
{
	isHovered = true;
	Refresh();
	evt.Skip();
}

void Decrypt_Panel::OnMouseLeaveWindow(wxMouseEvent& evt)
{
	isHovered = false;
	Refresh();
	evt.Skip();
}
