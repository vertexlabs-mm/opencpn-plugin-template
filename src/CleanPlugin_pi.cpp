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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <typeinfo>
#include "CleanPlugin_pi.h"
#include <wx/aui/framemanager.h>



// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin *create_pi(void *ppimgr) {
 
  return new CleanPlugin_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *p) { delete p; }


//---------------------------------------------------------------------------------------------------------
//
//    Deeprey PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

CleanPlugin_pi::CleanPlugin_pi(void *ppimgr) 
    : opencpn_plugin_119(ppimgr)
    , wxTimer(this)
    , m_pidc(nullptr)
    , m_pauimgr(nullptr)
    , m_gFrame(nullptr)
{
    // Basic icon init
    wxInitAllImageHandlers();
    wxImage panelIcon = LoadIMG(("deeprey_panel_icon1.png"), 32, 32);
    m_panelBitmap = wxBitmap(panelIcon);

}

/** ------------------------------------------------------------------
 *  PLUGIN LIFECYCLE: INIT
 *  ------------------------------------------------------------------ */

int CleanPlugin_pi::Init(void) {


    AddLocaleCatalog(_T("opencpn-cleanPlugin_pi"));
    

    if (!m_pidc) {
        m_pidc = new piDC();
        wxFont fo = *OCPNGetFont(_("Dialog"), 10);
        m_Font_DC = new wxFont(fo);
        m_pidc->SetFont(*m_Font_DC);
    }


    m_pconfig = GetOCPNConfigObject();
    LoadConfig();
   

    m_gFrame = static_cast<wxFrame *>(GetOCPNCanvasWindow()->GetParent()); 
 

    return (WANTS_CONFIG
          | WANTS_LATE_INIT
          | USES_AUI_MANAGER
          | WANTS_OPENGL_OVERLAY_CALLBACK
          | WANTS_PLUGIN_MESSAGING
          | WANTS_MOUSE_EVENTS);
}


void CleanPlugin_pi::LateInit(void) {
    

     // 1) Get the AUI manager
    m_pauimgr = GetFrameAuiManager();
    if (!m_pauimgr) return ;  // No AUI manager, no party

    //EnableSplitScreenLayout(true);

    InitLogger();  //Init custom logger

    EnableLegacyUi(true);

    DLOG("Clean Plugin Initialized Late init");
}

bool CleanPlugin_pi::DeInit(void) {

    DLOG("Clean Plugin DeInit called");

    SaveConfig();

    EnableLegacyUi(true);

    if (m_pidc) {
        delete m_pidc;
        m_pidc = nullptr;
    }
    if (m_Font_DC) {
        delete m_Font_DC;
        m_Font_DC = nullptr;
    }

    m_pauimgr->Update();

    DeInitLogger();

    return true;
}



bool CleanPlugin_pi::MouseEventHook(wxMouseEvent &event) {
    if (event.LeftUp()) {
        
        return true;
    }
    return false;
}


void CleanPlugin_pi::RenderHelloWorld(PlugIn_ViewPort *vp)
{
    // Vérifier que nous avons un DC valide
    if (!m_pidc) return;
    
    // Position du texte (centre de l'écran)
    int x = vp->pix_width / 2;
    int y = vp->pix_height / 2;
    
    // Texte à afficher
    wxString text = _T("Hello World");
    
    // Sauvegarder l'état actuel du DC
    wxPen oldPen = m_pidc->GetPen();
    wxBrush oldBrush = m_pidc->GetBrush();
    wxFont oldFont = m_pidc->GetFont();
    
    // Configurer la police (plus grande pour être visible)
    wxFont font = *m_Font_DC;
    font.SetPointSize(20);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    m_pidc->SetFont(font);
    
    // Mesurer le texte pour l'encadré
    int text_width, text_height;
    m_pidc->GetTextExtent(text, &text_width, &text_height);
    
    // Définir les marges pour l'encadré
    int margin = 10;
    int rect_x = x - text_width/2 - margin;
    int rect_y = y - text_height/2 - margin;
    int rect_width = text_width + 2*margin;
    int rect_height = text_height + 2*margin;
    
    // Dessiner le rectangle de fond (blanc semi-transparent)
    m_pidc->SetBrush(wxBrush(wxColour(255, 255, 255, 200)));
    m_pidc->SetPen(wxPen(wxColour(255, 0, 0), 3)); // Bordure rouge épaisse
    m_pidc->DrawRectangle(rect_x, rect_y, rect_width, rect_height);
    
    // Dessiner le texte en rouge
    m_pidc->SetTextForeground(wxColour(255, 0, 0)); // Rouge
    m_pidc->DrawText(text, x - text_width/2, y - text_height/2);
    
    // Restaurer l'état du DC
    m_pidc->SetPen(oldPen);
    m_pidc->SetBrush(oldBrush);
    m_pidc->SetFont(oldFont);
}

bool CleanPlugin_pi::RenderGLOverlayMultiCanvas(wxGLContext *pcontext,PlugIn_ViewPort *vp, int canvasIndex,int priority) 
{
    if (priority != 128 ) return false;
   
    RenderHelloWorld(vp);

    return true;
}


void CleanPlugin_pi::SetPluginMessage(wxString &message_id, wxString &message_body) {
    if (message_id == _T("WMM_VARIATION_BOAT") ||
        message_id == _T("OpenCPN Config") ||
        message_id == _T("OCPN_OPENGL_CONFIG")) {
    return;  
    }

    if (message_id == _T("GLOBAL_SETTINGS_UPDATED")) {

    }
    /*
    // Log du contenu du message reçu
    DLOG("------------------------Received From Plugin ""Message:-------------------");
    DLOG("Message ID: %s", message_id);
    DLOG("Message Body: %s", message_body);
    DLOG("--------------------------------------END------------------------------");
    */
}


void CleanPlugin_pi::PreShutdownHook() {
  

}



/*************************************************************************************************************/

int CleanPlugin_pi::GetAPIVersionMajor() { return atoi(API_VERSION); }

int CleanPlugin_pi::GetAPIVersionMinor() 
{
    std::string v(API_VERSION);
    size_t dotpos = v.find('.');
    return atoi(v.substr(dotpos + 1).c_str());
}

int CleanPlugin_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int CleanPlugin_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

int GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }

int GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; }

const char *GetPlugInVersionPre() { return PKG_PRERELEASE; }

const char *GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap *CleanPlugin_pi::GetPlugInBitmap() { return &m_panelBitmap; }

wxString CleanPlugin_pi::GetCommonName() { return _T("CleanPlugin"); }

wxString CleanPlugin_pi::GetShortDescription() 
{
    return _T("Clean Plugin for OpenCPN");
}

wxString CleanPlugin_pi::GetLongDescription() 
{
    return _T("Clean Plugin for OpenCPN.");
}
/**************************************************************************************************************************/

void CleanPlugin_pi::SetInterval(int interval) {
    m_interval = interval;
    if (IsRunning())                          // Timer started?
    Start(m_interval, wxTIMER_CONTINUOUS);  // restart timer with new interval
}

bool CleanPlugin_pi::LoadConfig(void) 
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;
    if (!pConf) {
        return false;
    }

    
    return true;
}

bool CleanPlugin_pi::SaveConfig(void) 
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;
    if (!pConf) {
        return false;
    }

    
    return true;
}

void CleanPlugin_pi::UpdateAuiStatus(void) 
{
    //    This method is called after the PlugIn is initialized
    //    and the frame has done its initial layout, possibly from a saved
    //    wxAuiManager "Perspective" It is a chance for the PlugIn to syncronize
    //    itself internally with the state of any Panes that were added to the
    //    frame in the PlugIn ctor.
}


void CleanPlugin_pi::Notify() {}



/*********************************************************** Utils *******************************************************************/

wxBitmap CleanPlugin_pi::LoadSVG(const wxString& filename, int width, int height)
{
    wxFileName fn;
    wxString tmp_path;
    tmp_path = GetPluginDataDir("CleanPlugin_pi");  // This depends on your plugin environment
    fn.SetPath(tmp_path);
    fn.AppendDir(_T("data"));
    fn.SetFullName(filename);
    wxSize svgSize(width, height);
    wxBitmapBundle bb = wxBitmapBundle::FromSVGFile(fn.GetFullPath(), svgSize);
    return bb.GetBitmap(svgSize);
}

//----------------------------------------------------------------------------------
// Loads an image (PNG/JPG/etc.) from a file. If not in LOCAL_DEV mode, it
// uses the plugin data directory. Then it optionally scales the image
// to the requested width/height.
//----------------------------------------------------------------------------------
wxImage CleanPlugin_pi::LoadIMG(const wxString& filename, int width, int height)
{
    wxFileName fn;
    wxString tmp_path;
    tmp_path = GetPluginDataDir("CleanPlugin_pi");
    fn.SetPath(tmp_path);
    fn.AppendDir(_T("data"));
    fn.SetFullName(filename);

    wxImage image;
    if (!image.LoadFile(fn.GetFullPath(), wxBITMAP_TYPE_ANY)) {
        // If the file could not be loaded, return an empty wxImage.
        return wxImage();
    }

    // Scale the image if width and height are specified (greater than 0).
    if (width > 0 && height > 0) {
        image = image.Scale(width, height, wxIMAGE_QUALITY_HIGH);
    }
    return image;
}

void CleanPlugin_pi::EnableLegacyUi(bool enable) 
{
    EnableMUIBar(enable, 0);
    EnableMUIBar(enable, 1);
    EnableCompassGPSIcon(enable, 0);
    EnableCompassGPSIcon(enable, 1);
    EnableChartBar(enable, 0);
    EnableChartBar(enable, 1);
    EnableStatusBar(enable);
    EnableMenu(enable);
    EnableTouchMode(!enable);
    SetEnableMainToolbar(enable);
    EnableCanvasFocusBar(false, 0);
    EnableCanvasFocusBar(false, 1);
}


bool CleanPlugin_pi::InitLogger()
{
    if (m_initializedLog)
        return true;

    wxFileName fn;

    wxString tmp_path = GetPluginDataDir("CleanPlugin_pi");
    fn.SetPath(tmp_path);
    fn.AppendDir(_T("data"));

    fn.SetFullName("clean_plugin.log");

    // Open the log file in append mode
    if (!m_DpLogFile.Open(fn.GetFullPath(), wxFile::write_append)) {
        wxLogError("Unable to open custom log file: %s", fn.GetFullPath());
        return false;
    }

    m_initializedLog = true;
    return true;
}

//----------------------------------------------------------------------------------
// Logger de-initialization. This closes the log file cleanly.
//----------------------------------------------------------------------------------
void CleanPlugin_pi::DeInitLogger()
{
    if (!m_initializedLog)
        return;

    m_initializedLog = false;
    if (m_DpLogFile.IsOpened())
        m_DpLogFile.Close();
}

void CleanPlugin_pi::DpLog(const wxString& file, int line, const wxColour& color, const wxString& message)
{
    if (!m_initializedLog)
        return;

    wxDateTime now = wxDateTime::Now();
    wxString timestamp = now.FormatISOCombined(' ');

    // Build the final log string
    wxString logMessage = wxString::Format("[%s] [%s:%d] %s", timestamp, file, line, message);

    if (m_DpLogFile.IsOpened()) {
        m_DpLogFile.Write(logMessage + "\n");
    }
}
