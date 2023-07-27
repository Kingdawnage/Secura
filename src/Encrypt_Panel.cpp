#include "Encrypt_Panel.h"
#include <wx/wx.h>
#include <wx/graphics.h>

Encrypt_Panel::Encrypt_Panel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow()
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

void Encrypt_Panel::BindEvents()
{
	this->Bind(wxEVT_PAINT, &Encrypt_Panel::OnPaint, this);
	this->Bind(wxEVT_ENTER_WINDOW, &Encrypt_Panel::OnMouseEnterWindow, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &Encrypt_Panel::OnMouseLeaveWindow, this);
}

void Encrypt_Panel::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);

	auto gc = wxGraphicsContext::Create(dc);
	if (gc) {
		DrawOnContext(*gc);
		delete gc;
	}
	evt.Skip();
}

void Encrypt_Panel::DrawOnContext(wxGraphicsContext& gc)
{
	wxColour bgcolor;
	wxFont textfont;
	if (isHovered) {
		bgcolor = wxColour(102, 154, 199);
		textfont = wxFontInfo({ 0, GetSize().GetHeight() / 3 }).FaceName("Comic Sans MS");
		gc.SetFont(textfont, wxColour(255, 255, 255));
	}
	else
	{
		bgcolor = wxColour(18, 112, 195);
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

void Encrypt_Panel::OnMouseEnterWindow(wxMouseEvent& evt)
{
	isHovered = true;
	Refresh();
	evt.Skip();
}

void Encrypt_Panel::OnMouseLeaveWindow(wxMouseEvent& evt)
{
	isHovered = false;
	Refresh();
	evt.Skip();
}
