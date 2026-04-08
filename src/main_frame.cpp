#include "main_frame.h"

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Settings Demo", wxDefaultPosition,
              wxDefaultSize)
{
    SetMinClientSize(FromDIP(wxSize(600, 400)));

    BuildMenuBar();
    BuildUI();
    RestoreSettings();

    CreateStatusBar();
    wxString configPath = wxFileConfig::GetLocalFileName(
        wxTheApp->GetAppName(),
        wxCONFIG_USE_SUBDIR | wxCONFIG_USE_XDG);
    SetStatusText("Config: " + configPath);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

MainFrame::~MainFrame()
{
    delete fileHistory;
}

void MainFrame::BuildMenuBar()
{
    fileMenu = new wxMenu();
    fileMenu->Append(wxID_OPEN, "&Open...\tCtrl+O");
    fileMenu->AppendSeparator();
    // Recent file items will be inserted here by wxFileHistory
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+F4");

    auto *viewMenu = new wxMenu();
    viewMenu->Append(ID_FONT_INCREASE, "Increase Font Size\tCtrl+=");
    viewMenu->Append(ID_FONT_DECREASE, "Decrease Font Size\tCtrl+-");
    viewMenu->Append(ID_FONT_RESET, "Reset Font Size\tCtrl+0");

    auto *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(viewMenu, "&View");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, [this](wxCommandEvent &) { Close(); }, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnRecentFile, this, wxID_FILE1, wxID_FILE9);
    Bind(wxEVT_MENU, &MainFrame::OnFontSizeChange, this, ID_FONT_INCREASE,
         ID_FONT_RESET);
}

void MainFrame::BuildUI()
{
    textCtrl =
        new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                       wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
}

void MainFrame::RestoreSettings()
{
    auto *config = wxConfigBase::Get();

    // Recent files
    fileHistory = new wxFileHistory(9);
    fileHistory->UseMenu(fileMenu);
    fileHistory->Load(*config);

    // Font size
    fontSize = config->ReadLong("/Preferences/FontSize", 12);
    ApplyFontSize(fontSize);
}

void MainFrame::SaveSettings()
{
    auto *config = wxConfigBase::Get();

    // Recent files
    fileHistory->Save(*config);

    // Font size
    config->Write("/Preferences/FontSize", fontSize);

    config->Flush();
}

void MainFrame::OpenFile(const wxString &path)
{
    if (!textCtrl->LoadFile(path))
    {
        wxMessageBox("Could not open file: " + path, "Error",
                     wxOK | wxICON_ERROR);
        return;
    }

    textCtrl->SetInsertionPoint(0);
    SetTitle("Settings Demo - " + wxFileName(path).GetFullName());

    fileHistory->AddFileToHistory(path);

    // Save last used directory
    wxConfigBase::Get()->Write("/Paths/LastDirectory", wxPathOnly(path));
}


void MainFrame::ApplyFontSize(int size)
{
    wxFont font(size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_NORMAL);
    textCtrl->SetFont(font);
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
    wxString lastDir = wxConfigBase::Get()->Read("/Paths/LastDirectory",
                                                 wxGetHomeDir());

    wxFileDialog dlg(this, "Open File", lastDir, "",
                     "Text files (*.txt)|*.txt|All files (*.*)|*.*",
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dlg.ShowModal() == wxID_OK)
        OpenFile(dlg.GetPath());
}

void MainFrame::OnRecentFile(wxCommandEvent &event)
{
    int index = event.GetId() - wxID_FILE1;
    wxString path = fileHistory->GetHistoryFile(index);

    if (!wxFileExists(path))
    {
        wxMessageBox("File not found: " + path, "Error",
                     wxOK | wxICON_WARNING);
        fileHistory->RemoveFileFromHistory(index);
        return;
    }

    OpenFile(path);
}

void MainFrame::OnFontSizeChange(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case ID_FONT_INCREASE:
        fontSize = std::min(fontSize + 2, 48);
        break;
    case ID_FONT_DECREASE:
        fontSize = std::max(fontSize - 2, 8);
        break;
    case ID_FONT_RESET:
        fontSize = 12;
        break;
    }

    ApplyFontSize(fontSize);
}

void MainFrame::OnClose(wxCloseEvent &event)
{
    SaveSettings();
    event.Skip();
}
