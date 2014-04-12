//CGraphics.h
#ifndef C_GRAPHICS_H
#define C_GRAPHICS_H

#include <windows.h>
#include <vector>
//#include "..\resource.h"

struct SPRITE_DATA{
  int type;
  int x;
  int y;
  int width;
  int height;
  int mask_x;
  int mask_y;
  int kind;
};

class CGraphics{
public:
  CGraphics();
  ~CGraphics();
  void addSprite(int type, int x, int y, int width, int height, int mask_x, int mask_y, int kind);
  int getSpriteX(int type);
  int getSpriteY(int type);
  int getSpriteWidth(int type);
  int getSpriteHeight(int type);
  int getSpriteMaskX(int type);
  int getSpriteMaskY(int type);
  int getSpriteKind(int type);
private:
  std::vector<SPRITE_DATA> m_sprite;
};

#endif