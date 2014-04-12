//CGame.h
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <windows.h>
#include "..\resource.h"
#include "..\includes\CTimer.h"
#include "..\includes\CHighScore.h"
#include <vector>
using std::cout;
using std::endl;

const int STATE_NOTHING = 0;
const int STATE_PLAY = 1;
const int STATE_DEAD = 2;
const int STATE_WIN_LEVEL = 3;
const int STATE_GAME_OVER = 4;
const int STATE_NEW_GAME = 5;
const int STATE_WIN_GAME = 6;


const int C_MAX_ENERGY = 150;
const int C_TOKEN_POINTS = 10;
const int C_SILVERTOKEN_POINTS = 250;
const int C_TIME_LEVEL = 60;
const int C_HOR_MAX_SPEED = 16;
const int C_GRAVITY = 5;
const int C_BOTTOM = 736;
const int C_PLAYER_X = 108;
const int C_PLAYER_Y = 660;
const int C_MAX_VEL_UP = -40;
const int C_MAX_LIVES = 3;
const int C_MAX_LEVEL = 3;
const int C_LIVES_POINTS = 1000;
const int C_RED_SKUNK = 250;
const int C_BLACK_SKUNK = 750;

struct GAME_FRAME{
  int posX;
  int posY;
};

class CGame{
public:
  CGame();  
  ~CGame();
  void setView(RECT rc);
  void setActiveRegion(RECT rc);
  void setWorld(RECT rc);
  void setPlayerPosition(POINT pt);
  void setPlayerInitialPosition(POINT pt);
  void movePlayerRight(int distance);
  void movePlayerLeft(int distance);
  void movePlayerUp(int distance);
  void movePlayerDown(int distance);
  void movePlayerStop();
  void setExit(POINT pt);
  POINT getExit();
  void newGame();
  void newLevel();
  void newLife();
  void update();
  int getVertVel();
  void setVertVel(int vel);
  int getFrameNum();
  int getDirection();
  void setBottom(int bottom);
  int getBottom();
  bool gameOver();
  bool lifeOver();
  void killSegway();
  bool levelComplete();
  int getEnergy();
  void resetEnergy();
  int getLives();
  int getLevel();
  void incLevel();
  int getTime();
  void setLevel(int level);
  void playBack();
  bool isPlayback();
  int getScore();
  void addToken();
  int getTokens();
  void clearTokens();
  void addSilverToken();
  int getSilverTokens();
  void clearSilverTokens();
  void addBlackSkunkScore();
  void addRedSkunkScore();

  RECT getView();
  RECT getActiveRegion();
  RECT getWorld();
  POINT getPlayerPosition();
  RECT m_activeRegion; //600x600
  int getFrameCount();

private:
  RECT m_view;         //400x300

  RECT m_world;        //2400x600
  POINT m_player;      //player position
  POINT m_initPlayer; //for start of level
  POINT m_exit;       //location of exit
  int m_level;        //current level
  int m_time;         //remaining time
  int m_score;        //current score
  int m_lives;        //number of lives
  int m_state;        //cGameStatePlay has several sub-states
  bool m_bReady;      //true if okay to continue to next level
  int m_gravity;      //constantly applied gravity velocity
  int m_energy;
  bool m_bGameOver;
  bool m_bLifeOver;
  bool m_bLevelComplete;
  int m_velUp;
  int m_velHor;
  int m_frame;
  int m_horMaxSpeed;
  int m_dir;          //0 = pointing to the right, 1 = pointing to the left
  CTimer m_timer;
  CTimer m_energyTimer;
  int m_bottom;
  std::vector<GAME_FRAME> m_record;
  bool m_bPlayback;
  int m_playbackFrame;
  int m_tokens;
  int m_silverTokens;
  bool m_bSkunkBite;
  bool m_bEntry;
};

#endif //GAME_h
