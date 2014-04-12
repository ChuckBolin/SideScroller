//cGameStateScore.cpp
#include "..\includes\cgamestatescore.h"
#include "..\includes\keystatus.h"
//#include "..\includes\log_templates.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;
extern HINSTANCE g_hAppInst;
extern CHighScore g_HighScore;

cGameStateScore::cGameStateScore(void)
{
}

cGameStateScore::~cGameStateScore(void)
{
   ::SelectObject(m_hdcBB, m_hOldSurface);
  ::DeleteObject(m_hSurface);
  ::DeleteObject(m_hOldSurface);
  ::DeleteDC(m_hdcBB);
  ::DeleteObject(m_hBitmap);
  //::DeleteObject(m_hHill);
  //::DeleteObject(m_hHillMask);
  //::DeleteObject(m_hBitmapMask);
  //::DeleteObject(m_hBitmapMask2);
  //::DeleteObject(m_hTileset);
  ::DeleteObject(m_hOldBrush);
  ::DeleteObject(m_hBrush);
  ::DeleteObject(m_hRedBrush);
  ::DeleteObject(m_hBlueBrush);
  ::DeleteObject(m_hBlackBrush);
  ::DeleteObject(m_hYellowBrush);
}

void cGameStateScore::initialize(){
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
    
  m_hBitmap = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP4));
  
  //brushes for filling rectangles
  m_lb.lbStyle=BS_SOLID;
  m_lb.lbColor=RGB(0,210,210);//cyan
  m_hBrush = ::CreateBrushIndirect(&m_lb);  

  m_lb.lbColor=RGB(255, 255,0); //yellow
  m_hYellowBrush = ::CreateBrushIndirect(&m_lb);

  m_lb.lbColor=RGB(0,0,0);//black
  m_hBlackBrush = ::CreateBrushIndirect(&m_lb);

  m_lb.lbColor=RGB(0, 80, 0);//dark green
  m_hBlueBrush = ::CreateBrushIndirect(&m_lb);  

  m_lb.lbColor=RGB(155,0,0); //redish 
  m_hRedBrush = ::CreateBrushIndirect(&m_lb);  
}

void cGameStateScore::activate(){
}


cGameStateObject* cGameStateScore::update(){

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }
  return 0;
}

void cGameStateScore::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  m_event = GO_NO_WHERE;
  
  if(keyDown(VK_SPACE))
    m_event = GO_MAIN;

  if(keyDown('S'))
    m_event = GO_NO_WHERE;
}

std::string cGameStateScore::convertInteger(int num){
  std::ostringstream oss;
  std::string s;
  oss.str("");
  oss << num;
  s = oss.str();
  return s;
}

void cGameStateScore::render(){
  int nLimit = 10;
  std::string sScore;
  HDC hdcBitmap = ::CreateCompatibleDC(NULL);
  PAINTSTRUCT ps;
  HDC hdc = ::BeginPaint(g_hMainWnd, &ps);

  //clear backbuffer with black
  ::BitBlt(m_hdcBB, 0, 0, m_width, m_height, 0, 0, 0, BLACKNESS);
  ::SelectObject(m_hdcBB,m_hBlackBrush);
  ::Rectangle(m_hdcBB,0,0, m_width, m_height);//top border

  //display bitmap for scoreboard
  ::SelectObject(hdcBitmap, m_hBitmap);
  ::BitBlt(m_hdcBB,  0, 0, 800, 600, hdcBitmap,0, 0,SRCCOPY);
  
  if(g_HighScore.size() < nLimit)
    nLimit = g_HighScore.size();

  //print scores
  ::SetBkColor(m_hdcBB,RGB(0,0,0));
  for(int i = 0; i < nLimit; i++){
    ::SetTextColor(m_hdcBB,RGB(255,0,0));
    ::TextOut(m_hdcBB, 235, 225 + (i * 22), g_HighScore.getName(i).c_str(), g_HighScore.getName(i).size() );  
    ::SetTextColor(m_hdcBB,RGB(0,255,0));
    sScore = convertInteger(g_HighScore.getScore(i));
    ::TextOut(m_hdcBB, 525, 225 + (i * 22),  sScore.c_str(), sScore.size() );  
  }

  //copies backbuffer to front buffer
  ::BitBlt(hdc, 0, 0, m_width, m_height, m_hdcBB, 0, 0, SRCCOPY);
  ::ReleaseDC(g_hMainWnd, hdc);
  ::DeleteObject(hdcBitmap);
  ::EndPaint(g_hMainWnd,&ps); 
}

void cGameStateScore::deactivate(){
}
void cGameStateScore::resume(){
}
void cGameStateScore::pause(){
}
void cGameStateScore::save(){
}

void cGameStateScore::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}