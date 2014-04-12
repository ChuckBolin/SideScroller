#include "..\includes\cgamestatemain.h"
#include "..\includes\keystatus.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;
extern HINSTANCE g_hAppInst;

cGameStateMain::cGameStateMain(void)
{
}

cGameStateMain::~cGameStateMain(void)
{
  ::SelectObject(m_hdcBB, m_hOldSurface);
  ::DeleteObject(m_hSurface);
  ::DeleteObject(m_hOldSurface);
  ::DeleteDC(m_hdcBB);
  ::DeleteObject(m_hBitmap);
}

void cGameStateMain::initialize(){
  
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
  
  m_hBitmap = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP6));

  m_bActivated = true;

}

void cGameStateMain::activate(){
}

cGameStateObject* cGameStateMain::update(){

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      if(m_event == GO_PLAY){        
        //::mciSendString("play track4.mid", NULL, 0, NULL);
        m_event = GO_NO_WHERE;
        return m_TE[i].p_gso;
      }
      else{
        //::mciSendString("stop track4.mid", NULL, 0, NULL);
        return m_TE[i].p_gso;
      }
    }
  }

  return 0;
}

void cGameStateMain::render(){
  HDC hdcBitmap = ::CreateCompatibleDC(NULL);
  ::SelectObject(hdcBitmap, m_hBitmap);

  PAINTSTRUCT ps;
  HDC hdc = ::BeginPaint(g_hMainWnd, &ps);

  //copies stored bitmap to backbuffer
  ::BitBlt(m_hdcBB, 0, -20, m_width, m_height, hdcBitmap, 0, 0, SRCCOPY);

  //copies backbuffer to front buffer
  ::BitBlt(hdc, 0, 0, m_width, m_height, m_hdcBB, 0, 0, SRCCOPY);

  ::ReleaseDC(g_hMainWnd, hdc);
  ::DeleteObject(hdcBitmap);
  ::EndPaint(g_hMainWnd,&ps);
}

void cGameStateMain::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  m_event = GO_NO_WHERE;
  
  switch(wParam){
    case VK_RETURN:
      m_event = GO_PLAY;   
      break;
    case 'S':
      m_event = GO_SCORE;   
      break;
    case 'H':
      m_event = GO_HELP;   
      break;
    case VK_SPACE:
      m_event = GO_NO_WHERE;
      break;
    case VK_ESCAPE:
      m_event = GO_QUIT;   
      break;
    default:
      m_event = GO_NO_WHERE;
      break;
  } 
}

void cGameStateMain::deactivate(){
}
void cGameStateMain::resume(){
}
void cGameStateMain::pause(){
}
void cGameStateMain::save(){
}

void cGameStateMain::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}