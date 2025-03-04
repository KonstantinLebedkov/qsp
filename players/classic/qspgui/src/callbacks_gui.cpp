// Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org)
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "callbacks_gui.h"
#include "comtools.h"

QSPFrame *QSPCallBacks::m_frame;
bool QSPCallBacks::m_isHtml;
FMOD_SYSTEM *QSPCallBacks::m_sys;
QSPSounds QSPCallBacks::m_sounds;
float QSPCallBacks::m_volumeCoeff;
QSPVersionInfoValues QSPCallBacks::m_versionInfo;

void QSPCallBacks::Init(QSPFrame *frame)
{
    m_frame = frame;
    m_volumeCoeff = 1.0;

    FMOD_System_Create(&m_sys);
    wxString soundPath(QSPTools::GetResourcePath(QSP_SOUNDPLUGINS));
    FMOD_System_SetPluginPath(m_sys, wxConvFile.cWX2MB(soundPath.c_str()));
    #ifdef __WXMSW__
        FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_DSOUND);
    #elif __WXOSX__
        FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_COREAUDIO);
    #else
        FMOD_System_SetOutput(m_sys, FMOD_OUTPUTTYPE_ALSA);
    #endif
    FMOD_System_Init(m_sys, 32, FMOD_INIT_NORMAL, 0);

    QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&SetTimer);
    QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&RefreshInt);
    QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&SetInputStrText);
    QSPSetCallBack(QSP_CALL_ISPLAYINGFILE, (QSP_CALLBACK)&IsPlay);
    QSPSetCallBack(QSP_CALL_PLAYFILE, (QSP_CALLBACK)&PlayFile);
    QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&CloseFile);
    QSPSetCallBack(QSP_CALL_SHOWMSGSTR, (QSP_CALLBACK)&Msg);
    QSPSetCallBack(QSP_CALL_SLEEP, (QSP_CALLBACK)&Sleep);
    QSPSetCallBack(QSP_CALL_GETMSCOUNT, (QSP_CALLBACK)&GetMSCount);
    QSPSetCallBack(QSP_CALL_SHOWMENU, (QSP_CALLBACK)&ShowMenu);
    QSPSetCallBack(QSP_CALL_INPUTBOX, (QSP_CALLBACK)&Input);
    QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&ShowImage);
    QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&ShowPane);
    QSPSetCallBack(QSP_CALL_OPENGAME, (QSP_CALLBACK)&OpenGame);
    QSPSetCallBack(QSP_CALL_OPENGAMESTATUS, (QSP_CALLBACK)&OpenGameStatus);
    QSPSetCallBack(QSP_CALL_SAVEGAMESTATUS, (QSP_CALLBACK)&SaveGameStatus);
    QSPSetCallBack(QSP_CALL_VERSION, (QSP_CALLBACK)&Version);

    /* Prepare version values */
    m_versionInfo["player"] = "Classic";
    m_versionInfo["platform"] = QSPTools::GetPlatform();
}

void QSPCallBacks::DeInit()
{
    CloseFile(qspStringFromPair(0, 0));
    FMOD_System_Close(m_sys);
    FMOD_System_Release(m_sys);
}

void QSPCallBacks::SetTimer(int msecs)
{
    if (m_frame->IsQuit()) return;
    if (msecs)
        m_frame->GetTimer()->Start(msecs);
    else
        m_frame->GetTimer()->Stop();
}

void QSPCallBacks::RefreshInt(QSP_BOOL isRedraw)
{
    static int oldFullRefreshCount = 0;
    int i, numVal;
    bool isScroll, isCanSave;
    QSPString strVal;
    QSPListItem items[MAX_LIST_ITEMS];
    if (m_frame->IsQuit()) return;
    // -------------------------------
    isScroll = !(QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("DISABLESCROLL")), 0, &numVal, &strVal) && numVal);
    isCanSave = !(QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("NOSAVE")), 0, &numVal, &strVal) && numVal);
    m_isHtml = QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("USEHTML")), 0, &numVal, &strVal) && numVal;
    // -------------------------------
    m_frame->GetVars()->SetIsHtml(m_isHtml);
    if (QSPIsVarsDescChanged())
    {
        QSPString varsDesc = QSPGetVarsDesc();
        m_frame->GetVars()->SetText(wxString(varsDesc.Str, varsDesc.End), isScroll);
    }
    // -------------------------------
    int fullRefreshCount = QSPGetFullRefreshCount();
    if (oldFullRefreshCount != fullRefreshCount)
    {
        isScroll = false;
        oldFullRefreshCount = fullRefreshCount;
    }
    m_frame->GetDesc()->SetIsHtml(m_isHtml);
    if (QSPIsMainDescChanged())
    {
        QSPString mainDesc = QSPGetMainDesc();
        m_frame->GetDesc()->SetText(wxString(mainDesc.Str, mainDesc.End), isScroll);
    }
    // -------------------------------
    m_frame->GetActions()->SetIsHtml(m_isHtml);
    m_frame->GetActions()->SetIsShowNums(m_frame->IsShowHotkeys());
    if (QSPIsActionsChanged())
    {
        int actionsCount = QSPGetActions(items, MAX_LIST_ITEMS);
        m_frame->GetActions()->BeginItems();
        for (i = 0; i < actionsCount; ++i)
            m_frame->GetActions()->AddItem(wxString(items[i].Image.Str, items[i].Image.End), wxString(items[i].Name.Str, items[i].Name.End));
        m_frame->GetActions()->EndItems();
    }
    m_frame->GetActions()->SetSelection(QSPGetSelActionIndex());
    m_frame->GetObjects()->SetIsHtml(m_isHtml);
    if (QSPIsObjectsChanged())
    {
        int objectsCount = QSPGetObjects(items, MAX_LIST_ITEMS);
        m_frame->GetObjects()->BeginItems();
        for (i = 0; i < objectsCount; ++i)
            m_frame->GetObjects()->AddItem(wxString(items[i].Image.Str, items[i].Image.End), wxString(items[i].Name.Str, items[i].Name.End));
        m_frame->GetObjects()->EndItems();
    }
    m_frame->GetObjects()->SetSelection(QSPGetSelObjectIndex());
    // -------------------------------
    if (QSPGetVarValues(QSP_STATIC_STR(QSP_FMT("BACKIMAGE")), 0, &numVal, &strVal) && strVal.Str && strVal.Str != strVal.End)
        m_frame->GetDesc()->LoadBackImage(wxString(strVal.Str, strVal.End));
    else
        m_frame->GetDesc()->LoadBackImage(wxEmptyString);
    // -------------------------------
    m_frame->ApplyParams();
    if (isRedraw)
    {
        m_frame->EnableControls(false, true);
        m_frame->Update();
        wxTheApp->Yield(true);
        if (m_frame->IsQuit()) return;
        m_frame->EnableControls(true, true);
    }
    m_frame->GetGameMenu()->Enable(ID_SAVEGAMESTAT, isCanSave);
}

void QSPCallBacks::SetInputStrText(QSPString text)
{
    if (m_frame->IsQuit()) return;
    m_frame->GetInput()->SetText(wxString(text.Str, text.End));
}

QSP_BOOL QSPCallBacks::IsPlay(QSPString file)
{
    FMOD_BOOL playing = FALSE;
    wxString fileName(file.Str, file.End);
    QSPSounds::iterator elem = m_sounds.find(fileName.Upper());
    if (elem != m_sounds.end())
        FMOD_Channel_IsPlaying(((QSPSound *)(&elem->second))->Channel, &playing);
    return (playing == TRUE);
}

void QSPCallBacks::CloseFile(QSPString file)
{
    if (file.Str)
    {
        wxString fileName(file.Str, file.End);
        QSPSounds::iterator elem = m_sounds.find(fileName.Upper());
        if (elem != m_sounds.end())
        {
            ((QSPSound *)(&elem->second))->Free();
            m_sounds.erase(elem);
        }
    }
    else
    {
        for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
            ((QSPSound *)(&i->second))->Free();
        m_sounds.clear();
    }
}

void QSPCallBacks::PlayFile(QSPString file, int volume)
{
    FMOD_SOUND *newSound;
    FMOD_CHANNEL *newChannel;
    QSPSound snd;
    if (SetVolume(file, volume)) return;
    CloseFile(file);
    wxString fileName(file.Str, file.End);
    wxString filePath(m_frame->ComposeGamePath(fileName));
    #if defined(__WXMSW__) || defined(__WXOSX__)
    if (!FMOD_System_CreateSound(m_sys, wxConvFile.cWX2MB(filePath.c_str()), FMOD_SOFTWARE | FMOD_CREATESTREAM, 0, &newSound))
    #else
    FMOD_CREATESOUNDEXINFO exInfo;
    memset(&exInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    exInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    wxString dlsPath(QSPTools::GetResourcePath(QSP_SOUNDPLUGINS, QSP_MIDIDLS));
    wxCharBuffer dlsCharPath(wxConvFile.cWX2MB(dlsPath.c_str()));
    exInfo.dlsname = dlsCharPath;
    if (!FMOD_System_CreateSound(m_sys, wxConvFile.cWX2MB(filePath.c_str()), FMOD_SOFTWARE | FMOD_CREATESTREAM, &exInfo, &newSound))
    #endif
    {
        UpdateSounds();
        FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE, newSound, FALSE, &newChannel);
        snd.Channel = newChannel;
        snd.Sound = newSound;
        snd.Volume = volume;
        m_sounds.insert(QSPSounds::value_type(fileName.Upper(), snd));
        SetVolume(file, volume);
    }
}

void QSPCallBacks::ShowPane(int type, QSP_BOOL isShow)
{
    if (m_frame->IsQuit()) return;
    switch (type)
    {
    case QSP_WIN_ACTS:
        m_frame->ShowPane(ID_ACTIONS, isShow != QSP_FALSE);
        break;
    case QSP_WIN_OBJS:
        m_frame->ShowPane(ID_OBJECTS, isShow != QSP_FALSE);
        break;
    case QSP_WIN_VARS:
        m_frame->ShowPane(ID_VARSDESC, isShow != QSP_FALSE);
        break;
    case QSP_WIN_INPUT:
        m_frame->ShowPane(ID_INPUT, isShow != QSP_FALSE);
        break;
    }
}

void QSPCallBacks::Sleep(int msecs)
{
    if (m_frame->IsQuit()) return;
    bool isSave = m_frame->GetGameMenu()->IsEnabled(ID_SAVEGAMESTAT);
    bool isBreak = false;
    m_frame->EnableControls(false, true);
    int i, count = msecs / 50;
    for (i = 0; i < count; ++i)
    {
        wxThread::Sleep(50);
        m_frame->Update();
        wxTheApp->Yield(true);
        if (m_frame->IsQuit() ||
            m_frame->IsKeyPressedWhileDisabled())
        {
            isBreak = true;
            break;
        }
    }
    if (!isBreak)
    {
        wxThread::Sleep(msecs % 50);
        m_frame->Update();
        wxTheApp->Yield(true);
    }
    m_frame->EnableControls(true, true);
    m_frame->GetGameMenu()->Enable(ID_SAVEGAMESTAT, isSave);
}

int QSPCallBacks::GetMSCount()
{
    static wxStopWatch stopWatch;
    int ret = stopWatch.Time();
    stopWatch.Start();
    return ret;
}

void QSPCallBacks::Msg(QSPString str)
{
    if (m_frame->IsQuit()) return;
    RefreshInt(QSP_FALSE);
    QSPMsgDlg dialog(m_frame,
        wxID_ANY,
        m_frame->GetDesc()->GetBackgroundColour(),
        m_frame->GetDesc()->GetForegroundColour(),
        m_frame->GetDesc()->GetTextFont(),
        _("Info"),
        wxString(str.Str, str.End),
        m_isHtml,
        m_frame
    );
    m_frame->EnableControls(false);
    dialog.ShowModal();
    m_frame->EnableControls(true);
}

int QSPCallBacks::ShowMenu(QSPListItem *items, int count)
{
    if (m_frame->IsQuit()) return -1;
    m_frame->EnableControls(false);
    m_frame->DeleteMenu();
    for (int i = 0; i < count; ++i)
        m_frame->AddMenuItem(wxString(items[i].Name.Str, items[i].Name.End), wxString(items[i].Image.Str, items[i].Image.End));
    int index = m_frame->ShowMenu();
    m_frame->EnableControls(true);
    return index;
}

void QSPCallBacks::Input(QSPString text, QSP_CHAR *buffer, int maxLen)
{
    if (m_frame->IsQuit()) return;
    RefreshInt(QSP_FALSE);
    QSPInputDlg dialog(m_frame,
        wxID_ANY,
        m_frame->GetDesc()->GetBackgroundColour(),
        m_frame->GetDesc()->GetForegroundColour(),
        m_frame->GetDesc()->GetTextFont(),
        _("Input data"),
        wxString(text.Str, text.End),
        m_isHtml,
        m_frame
    );
    m_frame->EnableControls(false);
    dialog.ShowModal();
    m_frame->EnableControls(true);
    #ifdef _UNICODE
        wcsncpy(buffer, dialog.GetText().c_str(), maxLen);
    #else
        strncpy(buffer, dialog.GetText().c_str(), maxLen);
    #endif
}

void QSPCallBacks::Version(QSPString param, QSP_CHAR *buffer, int maxLen)
{
    wxString result;
    wxString request(param.Str, param.End);

    if (request.IsEmpty())
    {
        QSPString libVersion = QSPGetVersion();
        result = QSPTools::GetVersion(wxString(libVersion.Str, libVersion.End));
    }
    else
    {
        QSPVersionInfoValues::iterator value = m_versionInfo.find(request.Lower());
        if (value != m_versionInfo.end())
            result = value->second;
    }

#ifdef _UNICODE
    wcsncpy(buffer, result.c_str(), maxLen);
#else
    strncpy(buffer, result.c_str(), maxLen);
#endif
}

void QSPCallBacks::ShowImage(QSPString file)
{
    if (m_frame->IsQuit()) return;
    if (file.Str)
    {
        wxString imgFullPath(m_frame->ComposeGamePath(wxString(file.Str, file.End)));
        m_frame->ShowPane(ID_VIEWPIC, m_frame->GetImgView()->OpenFile(imgFullPath));
    }
    else
    {
        m_frame->ShowPane(ID_VIEWPIC, false);
    }
}

void QSPCallBacks::OpenGame(QSPString file, QSP_BOOL isNewGame)
{
    if (m_frame->IsQuit()) return;
    wxString fullPath(m_frame->ComposeGamePath(wxString(file.Str, file.End)));
    if (wxFileExists(fullPath))
    {
        wxFile fileToLoad(fullPath);
        int fileSize = fileToLoad.Length();
        void *fileData = (void *)malloc(fileSize);
        if (fileToLoad.Read(fileData, fileSize) == fileSize)
        {
            if (QSPLoadGameWorldFromData(fileData, fileSize, isNewGame) && isNewGame)
                m_frame->UpdateGamePath(fullPath);
        }
        free(fileData);
    }
}

void QSPCallBacks::OpenGameStatus(QSPString file)
{
    if (m_frame->IsQuit()) return;
    wxString fullPath;
    if (file.Str)
    {
        fullPath = m_frame->ComposeGamePath(wxString(file.Str, file.End));
    }
    else
    {
        wxFileDialog dialog(m_frame, _("Select saved game file"), wxEmptyString, wxEmptyString, _("Saved game files (*.sav)|*.sav"), wxFD_OPEN);
        m_frame->EnableControls(false);
        int res = dialog.ShowModal();
        m_frame->EnableControls(true);
        if (res != wxID_OK)
            return;
        fullPath = dialog.GetPath();
    }
    if (wxFileExists(fullPath))
    {
        wxFile fileToLoad(fullPath);
        int fileSize = fileToLoad.Length();
        void *fileData = (void *)malloc(fileSize);
        if (fileToLoad.Read(fileData, fileSize) == fileSize)
            QSPOpenSavedGameFromData(fileData, fileSize, QSP_FALSE);
        free(fileData);
    }
}

void QSPCallBacks::SaveGameStatus(QSPString file)
{
    if (m_frame->IsQuit()) return;
    wxString fullPath;
    if (file.Str)
    {
        fullPath = m_frame->ComposeGamePath(wxString(file.Str, file.End));
    }
    else
    {
        wxFileDialog dialog(m_frame, _("Select file to save"), wxEmptyString, wxEmptyString, _("Saved game files (*.sav)|*.sav"), wxFD_SAVE);
        m_frame->EnableControls(false);
        int res = dialog.ShowModal();
        m_frame->EnableControls(true);
        if (res != wxID_OK)
            return;
        fullPath = dialog.GetPath();
    }
    int fileSize = 64 * 1024;
    void *fileData = (void *)malloc(fileSize);
    if (!QSPSaveGameAsData(fileData, &fileSize, QSP_FALSE))
    {
        if (fileSize)
        {
            fileData = (void *)realloc(fileData, fileSize);
            if (!QSPSaveGameAsData(fileData, &fileSize, QSP_FALSE))
            {
                free(fileData);
                return;
            }
        }
    }
    wxFile fileToSave(fullPath, wxFile::write);
    fileToSave.Write(fileData, fileSize);
    free(fileData);
}

bool QSPCallBacks::SetVolume(QSPString file, int volume)
{
    if (!IsPlay(file)) return false;
    wxString fileName(file.Str, file.End);
    QSPSounds::iterator elem = m_sounds.find(fileName.Upper());
    if (elem != m_sounds.end())
    {
        QSPSound *snd = (QSPSound *)&elem->second;
        snd->Volume = volume;
        FMOD_Channel_SetVolume(snd->Channel, (float)(m_volumeCoeff * volume) / 100);
    }
    return true;
}

void QSPCallBacks::SetOverallVolume(float coeff)
{
    QSPSound *snd;
    FMOD_BOOL playing = FALSE;
    if (coeff < 0.0)
        coeff = 0.0;
    else if (coeff > 1.0)
        coeff = 1.0;
    m_volumeCoeff = coeff;
    for (QSPSounds::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
    {
        snd = (QSPSound *)&i->second;
        FMOD_Channel_IsPlaying(snd->Channel, &playing);
        if (playing)
            FMOD_Channel_SetVolume(snd->Channel, (float)(m_volumeCoeff * snd->Volume) / 100);
    }
}

void QSPCallBacks::UpdateSounds()
{
    QSPSound *snd;
    FMOD_BOOL playing = FALSE;
    QSPSounds::iterator i = m_sounds.begin();
    while (i != m_sounds.end())
    {
        snd = (QSPSound *)&i->second;
        FMOD_Channel_IsPlaying(snd->Channel, &playing);
        if (playing)
            ++i;
        else
        {
            snd->Free();
            m_sounds.erase(i++);
        }
    }
}
