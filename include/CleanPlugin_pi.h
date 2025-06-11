/******************************************************************************
 Project: Deeprey Plugin
 Purpose: Proprietary GUI plugin for OpenCPN
 Author: Deeprey Research Ltd

 Copyright (C) 2024 Deeprey Research Ltd
 All Rights Reserved

 This software is provided under a proprietary license. No part of this
 software may be reproduced, modified, distributed, or used in any manner
 without explicit written permission from Deeprey Research Ltd.

 This software is provided "AS IS", without warranty of any kind, express or
 implied, including but not limited to the warranties of merchantability,
 fitness for a particular purpose and non-infringement.
 *****************************************************************************/

#ifndef _DEEPREYPI_H_
#define _DEEPREYPI_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/fileconf.h>
#include <wx/filepicker.h>
#include <wx/file.h>
#include <wx/aui/aui.h>
#include <wx/aui/framemanager.h>



#include <set>


#include "ocpn_plugin.h"
#include "config.h"
#include "pidc.h"

#define DLOG(message, ...) DpLog(__FILE__, __LINE__, *wxBLACK, wxString::Format(message, ##__VA_ARGS__))

extern wxString _svg_deeprey_play;
extern wxString _svg_deeprey_toggled;
extern wxString _svg_deeprey_rollover;


class DpOverlayUIFactory;
class DpChartMenuManager;
class DpHomeMenu;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------


class CleanPlugin_pi : public opencpn_plugin_119, wxTimer{
public:

    CleanPlugin_pi(void *ppimgr);

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);
    void LateInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    bool RenderGLOverlayMultiCanvas(wxGLContext *pcontext, PlugIn_ViewPort *vp, int canvasIndex, int priority);
    void Notify();
    void SetInterval(int interval);

    //    The optional method overrides
    void UpdateAuiStatus(void);
    void PreShutdownHook();


private:

    wxBitmap LoadSVG(const wxString& filename, int width, int height);
    wxImage LoadIMG(const wxString& filename, int width, int height);
    bool InitLogger();
    void DeInitLogger();
    void DpLog(const wxString& file, int line, const wxColour& color, const wxString& message);
    bool LoadConfig(void);
    bool SaveConfig(void);
    void EnableLegacyUi(bool enable);
    void SetPluginMessage(wxString &message_id, wxString &message_body);
    bool MouseEventHook(wxMouseEvent &event);

    bool   m_initializedLog = false;
    wxFile m_DpLogFile;
    wxAuiManager *m_pauimgr;
    wxFileConfig *m_pconfig;
    int m_interval;
    wxBitmap m_panelBitmap;
    piDC *m_pidc;
    wxFont *m_Font_DC;
    wxFrame* m_gFrame;
 

    void RenderHelloWorld(PlugIn_ViewPort *vp);
    
};



#endif
