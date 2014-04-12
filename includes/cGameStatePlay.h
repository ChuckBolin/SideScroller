//cGameStatePlay.h
#ifndef GAME_STATE_PLAY_H
#define GAME_STATE_PLAY_H

#include "cgamestateobject.h"
#include "..\includes\CGame.h"
#include "..\includes\CTimer.h"
#include "..\includes\Keystatus.h"
#include "..\includes\basic_entity.h"
#include "..\includes\CGraphics.h"
#include "..\includes\ReadLevelFile.h"
#include "..\includes\CSkunkSetup.h"
#include "..\includes\CSound.h"
#include "..\resource.h"
#include <iostream>
#include <string>
#include <sstream>
#include <mmsystem.h>
using std::cout;
using std::endl;

struct GRAPHIC_DIGIT{
  int digit[5];
};

class cGameStatePlay :  public cGameStateObject{
public:
  cGameStatePlay(void);
  ~cGameStatePlay(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  
private:

  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CGame m_game;
  HDC m_hdcBB;  //Device Context for Backbuffer
  HBITMAP m_hBitmap;
  HBITMAP m_hOldSurface;
  HBITMAP m_hSurface;
  HBITMAP m_hBling;
  RECT m_RC;
  int m_width;
  int m_height;
  HBITMAP m_hHill;
  HBITMAP m_hHillMask;
  HBRUSH m_hBrush;
  HBRUSH m_hBlackBrush;
  HBRUSH m_hOldBrush;
  HBRUSH m_hBlueBrush;
  HBRUSH m_hRedBrush;
  HBRUSH m_hYellowBrush;
  LOGBRUSH m_lb;
  int m_event;
  int m_score;
  void loadSprites();
  void addLevel(int level);
  GRAPHIC_DIGIT getFontData(int value);
  CTimer m_timer;
  CGraphics m_graphics;
  CBasicEntity m_objects;
  CFile m_file;
  bool m_bGameActivated;
  bool m_bPlaybackMode;
  bool m_bPaused;
  CSound m_Sound;

};

#endif