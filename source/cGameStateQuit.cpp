//cGameStateQuit.cpp
#include "..\includes\cgamestatequit.h"
#include "..\includes\keystatus.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;

cGameStateQuit::cGameStateQuit(void)
{
}

cGameStateQuit::~cGameStateQuit(void)
{
}

void cGameStateQuit::initialize(){
}

void cGameStateQuit::activate(){
}


cGameStateObject* cGameStateQuit::update(){
  int event = GO_NO_WHERE;

  /*
  switch(g_wParam){
    case VK_SPACE:
      event = GO_PLAY;   
      break;
  }*/

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == event)
      return m_TE[i].p_gso;
  }

  return 0;
}

void cGameStateQuit::render(){
  HDC dc = ::GetDC(g_hMainWnd);
  ::TextOut(dc, 200, 200, "Quit   ", sizeof("Quit   ")-1 );
  ::ReleaseDC(g_hMainWnd,dc);
}

void cGameStateQuit::deactivate(){
}
void cGameStateQuit::resume(){
}
void cGameStateQuit::pause(){
}
void cGameStateQuit::save(){
}

void cGameStateQuit::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}