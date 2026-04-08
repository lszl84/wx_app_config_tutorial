#pragma once

#include <wx/wx.h>
#include <wx/filehistory.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();
    ~MainFrame();

private:
    void BuildMenuBar();
    void BuildUI();
    void RestoreSettings();
    void SaveSettings();
    void OpenFile(const wxString &path);
    void ApplyFontSize(int size);

    void OnOpen(wxCommandEvent &event);
    void OnRecentFile(wxCommandEvent &event);
    void OnFontSizeChange(wxCommandEvent &event);
    void OnClose(wxCloseEvent &event);

    wxTextCtrl *textCtrl = nullptr;
    wxMenu *fileMenu = nullptr;
    wxFileHistory *fileHistory = nullptr;

    int fontSize = 12;

    enum
    {
        ID_FONT_INCREASE = wxID_HIGHEST + 1,
        ID_FONT_DECREASE,
        ID_FONT_RESET,
    };
};
