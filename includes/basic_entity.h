//basic_entity.h
#ifndef BASIC_ENTITY_H
#define BASIC_ENTITY_H
#include <windows.h>
#include <vector>

struct BASIC_ENTITY{
  int type;
  int x;
  int y;
  int active;
  int initX; //for moving entities
  int initY;
  int speed;
  int delta; //how much it will travel
  int dir;   //0 is up, 1 is down, 0 is left, 1 is right
  int frame; //animation
};

class CBasicEntity{
public:
  CBasicEntity();
  ~CBasicEntity();
  void enable(int id);
  void disable(int id);
  //void enableActiveRegion();
  //void disableActiveRegion();
  int isActive(int id) const;
  //bool inActiveRegion();
  void setArea(RECT rc);
  void setType(int type);
  void render();
  void update(int id);
  int getType(int id);
  int getPosX(int id);
  int getPosY(int id);
  int getDir(int id);
  int getFrame(int id);
  void setPos(int id, int x, int y);
  void addEntity(int type, int x, int y);  
  int size();
  void clearAll();

private:
  RECT m_area;         //rectangular area for entity
  POINT m_pos;         //key position of entity...top-left corner
  int m_type;          //this corresponds to type of entity
  //bool m_activeRegion; //true if within active region
  bool m_active;       //true if still alive
  std::vector<BASIC_ENTITY> m_entity;
};

#endif //