//cGameStateQuit.h
#ifndef GAME_STATE_QUIT_H
#define GAME_STATE_QUIT_H

#include "cgamestateobject.h"
#include <iostream>
using std::cout;
using std::endl;

class cGameStateQuit : public cGameStateObject{
public:
  cGameStateQuit(void);
  ~cGameStateQuit(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
    std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
};


#endif
