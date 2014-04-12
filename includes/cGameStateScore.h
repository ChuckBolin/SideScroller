//cGameStateScore.h
#ifndef GAME_STATE_SCORE_H
#define GAME_STATE_SCORE_H

#include "..\includes\cgamestateobject.h"
#include "..\includes\CHighScore.h"
#include "..\resource.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using std::cout;
using std::endl;

struct HIGH_SCORE{
  std::string name;
  int score;
};

class cGameStateScore : public cGameStateObject{
public:
  cGameStateScore(void);
  ~cGameStateScore(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;

  HDC m_hdcBB;  //Device Context for Backbuffer
  HBITMAP m_hBitmap;
  HBITMAP m_hOldSurface;
  HBITMAP m_hSurface;
  RECT m_RC;
  int m_width;
  int m_height;
  HBRUSH m_hBrush;
  HBRUSH m_hBlackBrush;
  HBRUSH m_hOldBrush;
  HBRUSH m_hBlueBrush;
  HBRUSH m_hRedBrush;
  HBRUSH m_hYellowBrush;
  LOGBRUSH m_lb;
  std::vector<HIGH_SCORE> m_HighScore;
  std::string convertInteger(int num);
};

#endif
