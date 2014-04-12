#include "..\includes\basic_entity.h"

CBasicEntity::CBasicEntity(){
}

CBasicEntity::~CBasicEntity(){
}
void CBasicEntity::enable(int id){
  m_entity[id].active = 1;
}
void CBasicEntity::disable(int id){
  m_entity[id].active = 0;
}
/*
void CBasicEntity::enableActiveRegion(){
  //m_activeRegion = true;
}
void CBasicEntity::disableActiveRegion(){
  //m_activeRegion = false;
}

bool CBasicEntity::inActiveRegion(){
  return true; //m_activeRegion;
}
*/
void CBasicEntity::setArea(RECT rc){
  //m_area = rc;
}
void CBasicEntity::setType(int type){
  //m_type = type;
}

int CBasicEntity::size(){
  return m_entity.size();
}

int CBasicEntity::getType(int id){
  return m_entity[id].type;
}

int CBasicEntity::getPosX(int id){
  return m_entity[id].x;
}
int CBasicEntity::getPosY(int id){
  return m_entity[id].y;
}

int CBasicEntity::getDir(int id){
  return m_entity[id].dir;
}

int CBasicEntity::getFrame(int id){
  return m_entity[id].frame;
}
int CBasicEntity::isActive(int id)const{

  if(m_entity[id].active == 1)
    return 1;
  else
    return 0;
}

void CBasicEntity::setPos(int id, int x, int y){
  m_entity[id].x = x;
  m_entity[id].y = y;
}

void CBasicEntity::addEntity(int type, int x, int y){
  BASIC_ENTITY be;
  be.type = type;
  be.x = x;
  be.y = y;
  be.active = 1;

  //load moving platforms
  if(type > 2000 && type < 2100){//vertical
    be.initX = be.x;
    be.initY = be.y;
    be.speed = 3;   //3 pixels per update
    be.delta = (type - 2000) * 100; //maximum movement vertical is 100
    be.dir = 1;   //start moving down
  }
  else if(type > 2100 && type < 2125){//horizontal
    be.initX = be.x;
    be.initY = be.y;
    be.speed = 3;   //3 pixels per update
    be.delta = (type - 2100) * 100; //maximum movement horizontal is 100
    be.dir = 1;   //start moving right
  }
  else if(type > 3000 && type < 3125){//black skunks
    be.initX = be.x;
    be.initY = be.y;
    be.speed = 3;
    be.delta = (type - 3000) * 100; //max hor movement
    be.dir = 1; //start moving right
    be.frame = 0; //4 frames for animation
  }
  else if(type > 3200 && type < 3325){//red skunks
    be.initX = be.x;
    be.initY = be.y;
    be.speed = 7;
    be.delta = (type - 3200) * 100; //max hor movement
    be.dir = 1; //start moving right
    be.frame = 0; //4 frames for animation
  }

  m_entity.push_back(be);
}

void CBasicEntity::clearAll(){
  m_entity.clear();
}

//updates moving entities
//2001 means vertical, 1 x 100 pixels = 100 pixels
//2006 means vertical, 6 x 100 pixels = 600 pixels
void CBasicEntity::update(int id){
  if(m_entity[id].type > 2000 && m_entity[id].type <  2100){ //vertically moving
    if(m_entity[id].dir == 1){//moving down
      m_entity[id].y += m_entity[id].speed;
      if(m_entity[id].y > m_entity[id].initY + m_entity[id].delta){
        m_entity[id].y = m_entity[id].initY + m_entity[id].delta;
        m_entity[id].dir = 0; //start moving up
      }
    }
    else if(m_entity[id].dir == 0){//moving up
      m_entity[id].y -= m_entity[id].speed;
      if(m_entity[id].y < m_entity[id].initY){
        m_entity[id].y = m_entity[id].initY;
        m_entity[id].dir = 1;//start moving down
      }
    }
  }

  else if(m_entity[id].type > 2100 && m_entity[id].type <  2125){ //vertically moving
    if(m_entity[id].dir == 1){//moving right
      m_entity[id].x += m_entity[id].speed;
      if(m_entity[id].x > m_entity[id].initX + m_entity[id].delta){
        m_entity[id].x = m_entity[id].initX + m_entity[id].delta;
        m_entity[id].dir = 0; //start moving left
      }
    }
    else if(m_entity[id].dir == 0){//moving left
      m_entity[id].x -= m_entity[id].speed;
      if(m_entity[id].x < m_entity[id].initX){
        m_entity[id].x = m_entity[id].initX;
        m_entity[id].dir = 1;//start moving right
      }
    }
  }
  else if(m_entity[id].type > 3000 && m_entity[id].type <  3325){
    if(m_entity[id].dir == 1){//moving right
      m_entity[id].x += m_entity[id].speed;
      if(m_entity[id].x > m_entity[id].initX + m_entity[id].delta){
        m_entity[id].x = m_entity[id].initX + m_entity[id].delta;
        m_entity[id].dir = 0; //start moving left
      }
    }
    else if(m_entity[id].dir == 0){//moving left
      m_entity[id].x -= m_entity[id].speed;
      if(m_entity[id].x < m_entity[id].initX){
        m_entity[id].x = m_entity[id].initX;
        m_entity[id].dir = 1;//start moving right
      }
    }
    m_entity[id].frame++;//supports animation
    if(m_entity[id].frame > 3)
      m_entity[id].frame = 0;
  }
}