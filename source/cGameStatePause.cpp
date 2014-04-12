#include "..\includes\cgamestatepause.h"
#include "..\includes\keystatus.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;

cGameStatePause::cGameStatePause(void)
{
}

cGameStatePause::~cGameStatePause(void)
{
}

void cGameStatePause::initialize(){
  cout << "Pause initialized" << endl;
}

void cGameStatePause::activate(){
}


cGameStateObject* cGameStatePause::update(){


  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }

  return 0;
}

void cGameStatePause::render(){
  HDC dc = ::GetDC(g_hMainWnd);
  RECT rc;
  rc.left=250;
  rc.top = 25;
  rc.right=550;
  rc.bottom = 350;
  ::Rectangle(dc,rc.left,rc.top,rc.right,rc.bottom);
  
  char comment1[]="Game Paused ";
  char comment2[] = "Press spacebar to continue";
  ::SetTextColor(dc,RGB(150, 0, 0));
  ::TextOut(dc, 360, 50, comment1, sizeof(comment1) - 1 );
  ::SetTextColor(dc, RGB(0, 150, 0));
  ::TextOut(dc, 310, 100, comment2, sizeof(comment2) - 1 );
  ::ReleaseDC(g_hMainWnd,dc);
}

void cGameStatePause::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  m_event = GO_NO_WHERE;
  
  switch(wParam){
    case VK_SPACE:
      //m_event = GO_PLAY;   
      break;
  } 
}

void cGameStatePause::deactivate(){
}
void cGameStatePause::resume(){
}
void cGameStatePause::pause(){
}
void cGameStatePause::save(){
}

void cGameStatePause::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}