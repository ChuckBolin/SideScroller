//CGame.cpp
#include "..\includes\CGame.h"
//#include "..\includes\log_templates.h"
//template<typename T> void Log(T value1);
//template<typename T1,typename T2 > void Log(T1 value1, T2 value2);
extern CHighScore g_HighScore;
extern std::string g_PlayerName;

CGame::CGame(){
  //default values
  //world
  m_world.left = 0;
  m_world.top = 0;
  m_world.right = 2400;
  m_world.bottom = 800;

  //active region
  m_activeRegion.left=0;
  m_activeRegion.top = 0;
  m_activeRegion.right = 800;
  m_activeRegion.bottom = 800;

  //view port
  m_view.left = 100;
  m_view.top = 300;
  m_view.right = 500;
  m_view.bottom = 600;

  m_player.x = 32;//C_PLAYER_X;
  m_player.y = 660;//C_PLAYER_Y;

  m_gravity = C_GRAVITY;
  m_velUp = 0;
  m_velHor = 0;
  m_frame = 0;
  m_energy = C_MAX_ENERGY;
  m_timer.initialize();
  m_horMaxSpeed = C_HOR_MAX_SPEED;
  m_dir = 0;  //default facing right
  m_bottom = 736;
  m_bGameOver = false;
  m_bLifeOver = false;
  m_level = 1;
  m_lives = C_MAX_LIVES;

  m_energyTimer.initialize();
  m_bEntry = false; 
}

CGame::~CGame(){
}

bool CGame::gameOver(){
  return m_bGameOver;
}
int CGame::getVertVel(){
  return m_velUp;
}

void CGame::setVertVel(int vel){
  m_velUp = vel;
}

void CGame::setView(RECT rc){
  m_view = rc;
}

RECT CGame::getView(){
  return m_view;
}
void CGame::setActiveRegion(RECT rc){
  m_activeRegion = rc;
}
RECT CGame::getActiveRegion(){
  return m_activeRegion;
}

void CGame::setWorld(RECT rc){
  m_world = rc;
}

RECT CGame::getWorld(){
  return m_world;
}

void CGame::setPlayerPosition(POINT pt){
  m_player = pt;
}

void CGame::setPlayerInitialPosition(POINT pt){
  m_initPlayer = pt;
}

POINT CGame::getPlayerPosition(){
  return m_player;
}

void CGame::movePlayerRight(int distance){
  if(m_player.y > m_bottom - 2){
    m_velHor += distance;
    if(m_velHor > m_horMaxSpeed)
      m_velHor = m_horMaxSpeed;
   
    if(m_velHor > 0)
      m_dir = 0;
  }
  return;
}

void CGame::movePlayerLeft(int distance){
  if(m_player.y > m_bottom - 2){
    m_velHor -= distance;
    if(m_velHor < -m_horMaxSpeed)
      m_velHor = -m_horMaxSpeed;

    if(m_velHor < 0)
      m_dir = 1;

  }
  return;
}

void CGame::movePlayerUp(int distance){
  if(m_player.y > m_bottom - 2){
    m_velUp = -distance - ((float)abs(m_velHor) * .50);// * 2);
    m_player.y -= 2;
  }
  return;
}

void CGame::movePlayerStop(){
   
  if(m_player.y >= m_bottom){
    if(m_velHor > 0)
      m_velHor--;
    else if(m_velHor < 0)
      m_velHor++;
  }
}

int CGame::getFrameNum(){
  return m_frame;
}

void CGame::playBack(){
  m_bPlayback = true;
  m_playbackFrame = 0;
}

int CGame::getScore(){
  return m_score;
}

int CGame::getTime(){
    return m_time;
}

int CGame::getDirection(){
  return m_dir;
}

int CGame::getEnergy(){
  return m_energy;
}

void CGame::resetEnergy(){
  m_energy = C_MAX_ENERGY;
}

bool CGame::levelComplete(){
  return m_bLevelComplete;
}

bool CGame::isPlayback(){
  return m_bPlayback;
}

int CGame::getFrameCount(){
  return m_record.size();  
}

void CGame::update(){

  //evaluate remaining energy and time
  if(m_energyTimer.secondTimer()){
    m_time--;
    if(m_velHor > 0 && m_velHor < 5)
      m_energy -= 1;
    else if(m_velHor >= 5 && m_velHor < 9)
      m_energy -= 2;
    else if(m_velHor >= 9 && m_velHor < 13)
      m_energy -= 3;
    else if(m_velHor >= 13)
      m_energy -= 4;

    if (m_energy < 0 || m_time < 0){
      m_lives--;
      m_bLifeOver = true;
      if(m_lives < 0)
        m_bGameOver = true;
      else{
        m_energy = C_MAX_ENERGY;
        m_time = (1 + m_level) * C_TIME_LEVEL;
      }
    }
  }

  bool bReady = m_timer.getTimer(0.1);

  if(m_player.y >= 800 || m_bSkunkBite == true){
    m_lives--;
    m_bLifeOver = true;
    if(m_lives<=0)
      m_bGameOver = true;

    m_bSkunkBite = false;
  }
 
  //saves data
  if(m_bGameOver ==true){
    if(getScore() > g_HighScore.getLowScore()){
      //m_bEntry = true;
      g_HighScore.addScore(g_PlayerName, getScore());
      return;
    }
  }

  if(m_bPlayback == true){
    m_playbackFrame++;
    if(m_playbackFrame < m_record.size()){
      m_player.x = m_record[m_playbackFrame].posX;
      m_player.y = m_record[m_playbackFrame].posY;
    }
    else
      m_bPlayback == false;
  }
  else if(bReady == true){// && m_bPlayback == true){
    //segway animation
    if(m_velHor < 0 || m_velHor > 0){
      if(m_dir == 0){
        m_frame++;
        if(m_frame > 3)
          m_frame = 0;
      }
      else if(m_dir == 1){
        m_frame--;
        if(m_frame < 0)
          m_frame = 3;
      }
    }

    //left,right and top boundaries of map 2400x800
    if(m_player.x < 0){//left
      m_player.x = 0;
      m_velHor = -(m_velHor/2);
    }
   if(m_player.x > 2372){//right
      m_player.x = 2372;
      m_velHor = -(m_velHor/2);
    }
    if(m_player.y < 32){//top
      m_player.y = 32;
      m_velUp = 0;
    }

    m_velUp += m_gravity;
    if(m_velUp < C_MAX_VEL_UP)
      m_velUp = C_MAX_VEL_UP;
  
    m_player.x = m_player.x + m_velHor;  
    m_player.y = m_player.y + m_velUp;

    if(m_player.y > m_bottom){
      m_player.y = m_bottom ;
      m_velUp = 0;
    }

    if(m_player.y > 800){
      m_player.y = 800;
      m_velHor = 0;
      m_velUp = 0;
    }
    GAME_FRAME rec;
    rec.posX = m_player.x;
    rec.posY = m_player.y;    
    m_record.push_back(rec);   
  }
}

bool CGame::lifeOver(){
  return m_bLifeOver;  
}

void CGame::killSegway(){
  m_bSkunkBite = true;
}

int CGame::getLives(){
  return m_lives;
}
void CGame::setBottom(int bottom){
  m_bottom = bottom;
}

int CGame::getBottom(){
  return m_bottom;
}
void CGame::movePlayerDown(int distance){
}

void CGame::newLife(){
  m_time = (1 + m_level) * C_TIME_LEVEL;
  m_state = STATE_NOTHING;
  m_bReady = false;
  m_energy = C_MAX_ENERGY;
  m_player.x = m_initPlayer.x;
  m_player.y = m_initPlayer.y;  
  m_velUp = 0;
  m_velHor = 0;
  m_frame = 0;
  m_horMaxSpeed = C_HOR_MAX_SPEED;
  m_dir = 0;  //default facing right
  m_bottom = C_BOTTOM;
  m_bGameOver = false;
  m_bLifeOver = false;
  m_record.clear(); //clears stored player movement
  m_bPlayback = false;
}

void CGame::newGame(){
  m_level = 1;
  m_time = (1 + m_level) * C_TIME_LEVEL;
  m_score = 0;
  m_lives = C_MAX_LIVES;
  m_state = STATE_NOTHING;
  m_bReady = false;
  m_gravity = C_GRAVITY;
  m_energy = C_MAX_ENERGY;
  m_player.x = m_initPlayer.x;
  m_player.y = m_initPlayer.y;  
  m_velUp = 0;
  m_velHor = 0;
  m_frame = 0;
  m_horMaxSpeed = C_HOR_MAX_SPEED;
  m_dir = 0;  //default facing right
  m_bottom = C_BOTTOM;
  m_bGameOver = false;
  m_bLifeOver = false;
  m_bLevelComplete = false;
  m_record.clear();
  m_playbackFrame = 0;
  m_bPlayback = false;
  m_tokens = 0;
}
void CGame::addBlackSkunkScore(){
  m_score += (m_level * C_BLACK_SKUNK);
}

void CGame::addRedSkunkScore(){
  m_score += (m_level * C_RED_SKUNK);
}

void CGame::addToken(){
  m_tokens++;
  m_score += (m_level * C_TOKEN_POINTS);
}

int CGame::getTokens(){
  return m_tokens;
}

void CGame::clearTokens(){
  m_tokens = 0;
}

void CGame::addSilverToken(){
  m_silverTokens++;
  m_score += (m_level * C_SILVERTOKEN_POINTS);
}

int CGame::getSilverTokens(){
 return m_silverTokens;
}

void CGame::clearSilverTokens(){
 m_silverTokens = 0;
}

int CGame::getLevel(){
  return m_level;
}

void CGame::setLevel(int level){
  m_level = level;
}

void CGame::incLevel(){
  m_score += C_LIVES_POINTS * m_lives * m_level;

  m_level++;
  if(m_level > C_MAX_LEVEL){
    m_bGameOver = true;
    return;
  }
  else{
    m_bLevelComplete = true;
  }
}

void CGame::newLevel(){

  m_time = (1 + m_level) * C_TIME_LEVEL;  
  m_state = STATE_NOTHING;
  m_bReady = false;
  m_gravity = C_GRAVITY;
  m_energy = C_MAX_ENERGY;
  m_lives = C_MAX_LIVES;  //start with 3 lives each level...change later
  m_player.x = m_initPlayer.x;
  m_player.y = m_initPlayer.y;  
  m_velUp = 0;
  m_velHor = 0;
  m_frame = 0;
  m_horMaxSpeed = C_HOR_MAX_SPEED;
  m_dir = 0;  //default facing right
  m_bottom = C_BOTTOM;
  m_bGameOver = false;
  m_bLifeOver = false;
  m_record.clear(); //clears stored player movement
  m_bPlayback = false;
  m_bLevelComplete = false;
}

void CGame::setExit(POINT pt){
  m_exit = pt;
}

POINT CGame::getExit(){
  return m_exit;
}
