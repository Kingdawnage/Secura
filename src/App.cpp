#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxInitAllImageHandlers();

    MainFrame* mainFrame = new MainFrame("Secura");
    mainFrame->SetClientSize(800, 400);
    mainFrame->Centre();
    mainFrame->Show();
    return true;
}
