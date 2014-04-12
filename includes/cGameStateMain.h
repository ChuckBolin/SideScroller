#ifndef GAME_STATE_MAIN_H
#define GAME_STATE_MAIN_H
#include "cgamestateobject.h"
#include "..\includes\CTimer.h"
#include "..\resource.h"
#include <iostream>
#include <mmsystem.h>
using std::cout;
using std::endl;

class cGameStateMain :  public cGameStateObject{
public:
  cGameStateMain(void);
  ~cGameStateMain(void);
  void initialize();
  void activate();
  cGameStateObject* update();
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
  int m_event;
  CTimer m_timer;
  bool m_bActivated;
};

#endif