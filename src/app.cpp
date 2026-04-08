#include "app.h"
#include "main_frame.h"

#include <wx/fileconf.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    SetAppName("SettingsDemo");

    auto *config = new wxFileConfig(
        GetAppName(),
        wxEmptyString, wxEmptyString, wxEmptyString,
        wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR | wxCONFIG_USE_XDG);

    wxConfigBase::Set(config);

    // First-run detection
    wxString savedVersion;
    bool firstRun = !config->Read("/App/Version", &savedVersion);
    config->Write("/App/Version", wxString("1.0.0"));

    auto *frame = new MainFrame();
    frame->Show();

    if (firstRun)
    {
        wxString configPath = config->GetLocalFileName(GetAppName(),
            wxCONFIG_USE_SUBDIR | wxCONFIG_USE_XDG);

        wxMessageBox("Welcome to Settings Demo!\n\n"
                     "Open a text file to get started.\n"
                     "Your settings are stored in:\n" +
                         configPath,
                     "Welcome", wxOK | wxICON_INFORMATION, frame);
    }

    return true;
}
