#ifndef GAME_STATE_INTRO_H
#define GAME_STATE_INTRO_H
#include <iostream>
using std::cout;
using std::endl;
#include "..\resource.h"

//#include <windows.h>
#include "..\includes\cgamestateobject.h"
#include "..\includes\CTimer.h"
#include <sstream>
#include <string>

class cGameStateIntro :  public cGameStateObject{
public:
  cGameStateIntro(void);
  ~cGameStateIntro(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  //void render(HWND &hWnd, HDC &hdc);
  void render();
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  HDC m_hdcBB;  //Device Context for Backbuffer
  HBITMAP m_hBitmap;
  HBITMAP m_hOldSurface;
  HBITMAP m_hSurface;
  RECT m_RC;
  int m_width;
  int m_height;
  HBITMAP m_hIntroTile;
  HBITMAP m_hLoader;
  CTimer m_timer;
  bool m_bDone;
  int m_doneCount;
  int m_event;
  
  /*
  HDC hdcClient;
  HDC hdcWindow;
  HBITMAP ghBitMap;
  HDC bmHDC;
  BITMAP bitmap;//stores image
  HBITMAP hBitmap;
  HBITMAP hOldBitmap;
  HBITMAP oldBM;
  HDC hdcBackBuffer;
  */

};

#endif