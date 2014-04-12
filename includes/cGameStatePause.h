#ifndef GAME_STATE_PAUSE_H
#define GAME_STATE_PAUSE_H

#include "..\includes\cgamestateobject.h"
#include <iostream>
using std::cout;
using std::endl;

class cGameStatePause : public cGameStateObject{
public:
  cGameStatePause(void);
  ~cGameStatePause(void);
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
  int m_event;
};


#endif
