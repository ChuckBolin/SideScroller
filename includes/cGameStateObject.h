//cGameStateObject.h
//base class...all other game states are derived from here
#ifndef GAME_STATE_OBJECT_H
#define GAME_STATE_OBJECT_H

#include <windows.h>
#include <vector>
//

class cGameStateObject;//forward initialization

struct TRANSITION_EVENT{
  int event;
  cGameStateObject* p_gso;
};

class cGameStateObject{
public:
  cGameStateObject();
  ~cGameStateObject();

  virtual void initialize();
  virtual void activate();
  virtual cGameStateObject* update();
  virtual void render();
  //void render(HWND &hWnd,HDC &hdc);
  virtual void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  virtual void deactivate();
  virtual void resume();
  virtual void pause();
  virtual void save();
  virtual void addTransitionEvent(int event, cGameStateObject* p_Next);

protected:
  //std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
private:
  
  

};

#endif //GAME_STATE_OBJECT_H