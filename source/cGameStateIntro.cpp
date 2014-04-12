#include "..\includes\cgamestateintro.h"
#include "..\includes\keystatus.h"
#include "..\includes\CTimer.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;
extern HINSTANCE g_hAppInst;

cGameStateIntro::cGameStateIntro(void)
{

}

cGameStateIntro::~cGameStateIntro(void)
{
  ::SelectObject(m_hdcBB, m_hOldSurface);
  ::DeleteObject(m_hSurface);
  ::DeleteObject(m_hOldSurface);
  ::DeleteDC(m_hdcBB);
  ::DeleteObject(m_hBitmap);
  ::DeleteObject(m_hIntroTile);
  ::DeleteObject(m_hLoader);
}


void cGameStateIntro::initialize(){

  ::GetWindowRect(g_hMainWnd,&m_RC);
  m_width = m_RC.right - m_RC.left;
  m_height = m_RC.bottom - m_RC.top;
    
  HDC hdc = ::GetDC(g_hMainWnd);
  m_hdcBB = ::CreateCompatibleDC(hdc);//backbuffer DC
  m_hSurface = ::CreateCompatibleBitmap(hdc, m_width, m_height);//drawing surface
  ::ReleaseDC(g_hMainWnd,hdc);
  m_hOldSurface = (HBITMAP)::SelectObject(m_hdcBB, m_hSurface);

  HBRUSH black = (HBRUSH)::GetStockObject(BLACK_BRUSH);
  HBRUSH oldBrush = (HBRUSH)::SelectObject(m_hdcBB, m_hSurface);//select white brush
  ::Rectangle(m_hdcBB, 0, 0, m_width, m_height);//clears with white
  ::SelectObject(m_hdcBB, oldBrush);
  
  m_hBitmap = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP3));   //skunk


  m_hIntroTile = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP8));//spacebar

  m_hLoader = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP7));   //loader

  bool ret = m_timer.initialize();
  m_bDone = false;
  m_doneCount = 0;
}


void cGameStateIntro::activate(){
}


cGameStateObject* cGameStateIntro::update(){
  
  if(m_timer.getTimer(0.05)){
    m_doneCount++;
    if(m_doneCount> 20)
      m_bDone=true;
  }
  
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      if(m_bDone == true)
        return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateIntro::render(){
  HDC hdcBitmap = ::CreateCompatibleDC(NULL);
  ::SelectObject(hdcBitmap, m_hBitmap);

  PAINTSTRUCT ps;
  HDC hdc = ::BeginPaint(g_hMainWnd, &ps);

  //clear backbuffer with black
  ::BitBlt(m_hdcBB, 0, 0, m_width, m_height, 0, 0, 0, BLACKNESS);

  //copies stored bitmap to backbuffer - skunk photo
  ::BitBlt(m_hdcBB, 0, -12, m_width, m_height, hdcBitmap, 0, 0, SRCCOPY);

  ::SelectObject(hdcBitmap, m_hIntroTile);
  ::BitBlt(m_hdcBB, 50, 40, 275, 67, hdcBitmap, 2, 297, SRCAND);
  ::BitBlt(m_hdcBB, 50, 40, 275, 67, hdcBitmap, 2, 227, SRCPAINT);

  if(!m_bDone){
    //loader
    ::SelectObject(hdcBitmap, m_hLoader);
    ::BitBlt(m_hdcBB, 240,400,302,109,hdcBitmap,489, 223, SRCAND);
    ::BitBlt(m_hdcBB, 240,400,302,109,hdcBitmap,489, 1, SRCPAINT);
    ::BitBlt(m_hdcBB, 240,400,m_doneCount * 15,109,hdcBitmap,489, 334, SRCAND);  
    ::BitBlt(m_hdcBB, 240,400,m_doneCount * 15,109,hdcBitmap,489, 112, SRCPAINT);
  }
  else{
    //spacebar
    ::SelectObject(hdcBitmap, m_hIntroTile);
    ::BitBlt(m_hdcBB, 160,450,440,68,hdcBitmap,2, 155, SRCAND);
    ::BitBlt(m_hdcBB, 160,450,440,68,hdcBitmap,2, 83, SRCPAINT);
  }


  //copies backbuffer to front buffer
  ::BitBlt(hdc, 0, 0, m_width, m_height, m_hdcBB, 0, 0, SRCCOPY);

  ::ReleaseDC(g_hMainWnd, hdc);
  ::DeleteObject(hdcBitmap);
  ::EndPaint(g_hMainWnd,&ps);
}

//void cGameStateIntro::render(HWND &hWnd, HDC &hdc){}

void cGameStateIntro::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  m_event = GO_NO_WHERE;
  
  switch(wParam){
    case VK_SPACE:
      if(m_bDone == true)
        m_event = GO_MAIN;   
      break;
    default:
      m_event = GO_NO_WHERE;
      break;
  }
}

void cGameStateIntro::deactivate(){
}
void cGameStateIntro::resume(){
}
void cGameStateIntro::pause(){
}
void cGameStateIntro::save(){
}

void cGameStateIntro::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}