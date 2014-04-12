//CGraphics.cpp
#include "..\includes\CGraphics.h"
extern HINSTANCE g_hAppInst;

CGraphics::CGraphics(){

}
CGraphics::~CGraphics(){

}

void CGraphics::addSprite(int type, int x, int y, int width, int height, int mask_x, int mask_y, int kind){
  SPRITE_DATA sd;
  sd.type = type;
  sd.x = x;
  sd.y = y;
  sd.mask_x = mask_x;
  sd.mask_y = mask_y;
  sd.width = width;
  sd.height = height;
  sd.kind = kind;
  m_sprite.push_back(sd);
}

int CGraphics::getSpriteX(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].x;
  }
  return 0;
}

int CGraphics::getSpriteY(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].y;
  }
  return 0;
}

int CGraphics::getSpriteKind(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].kind;
  }
  return 0;}

int CGraphics::getSpriteWidth(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].width;
  }
  return 0;
}

int CGraphics::getSpriteHeight(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].height;
  }
  return 0;
}

int CGraphics::getSpriteMaskX(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].mask_x;
  }
  return 0;
}

int CGraphics::getSpriteMaskY(int type){
  for(int i = 0; i < m_sprite.size() ; i++){
    if(m_sprite[i].type == type)
      return m_sprite[i].mask_y;
  }
  return 0;
}
