//cGameStatePlay.cpp
#include "..\includes\cgamestateplay.h"
#include "..\includes\keystatus.h"
#include "..\includes\log_templates.h"

//defined in main.cpp WndProc
extern HWND	     g_hMainWnd;
extern UINT      g_Msg;
extern WPARAM    g_wParam;
extern LPARAM    g_lParam;
extern HINSTANCE g_hAppInst;
extern KEY_SELECTION g_KeySelection;



cGameStatePlay::cGameStatePlay(void)
{
}

cGameStatePlay::~cGameStatePlay(void)
{
  ::SelectObject(m_hdcBB, m_hOldSurface);
  ::DeleteObject(m_hSurface);
  ::DeleteObject(m_hOldSurface);
  ::DeleteDC(m_hdcBB);
  ::DeleteObject(m_hBitmap);
  //::DeleteObject(m_hHill);
  //::DeleteObject(m_hHillMask);
  //::DeleteObject(m_hBitmapMask);
  //::DeleteObject(m_hBitmapMask2);
  //::DeleteObject(m_hTileset);
  ::DeleteObject(m_hOldBrush);
  ::DeleteObject(m_hBrush);
  ::DeleteObject(m_hRedBrush);
  ::DeleteObject(m_hBlueBrush);
  ::DeleteObject(m_hBlackBrush);
  ::DeleteObject(m_hYellowBrush);
}

void cGameStatePlay::initialize(){ 
  ::GetWindowRect(g_hMainWnd,&m_RC);
  m_width = m_RC.right - m_RC.left;
  m_height = m_RC.bottom - m_RC.top;
    
  HDC hdc = ::GetDC(g_hMainWnd);
  m_hdcBB = ::CreateCompatibleDC(hdc);//backbuffer DC

  m_hSurface = ::CreateCompatibleBitmap(hdc, m_width, m_height);//drawing surface
  ::ReleaseDC(g_hMainWnd,hdc);
  m_hOldSurface = (HBITMAP)::SelectObject(m_hdcBB, m_hSurface);

  HBRUSH black = (HBRUSH)::GetStockObject(BLACK_BRUSH);
  HBRUSH oldBrush = (HBRUSH)::SelectObject(m_hdcBB, m_hSurface);//select white brush
  ::Rectangle(m_hdcBB, 0, 0, m_width, m_height);//clears with white
  ::SelectObject(m_hdcBB, oldBrush);
  
  m_hBitmap = ::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP2));
  m_hBling=::LoadBitmap(g_hAppInst,MAKEINTRESOURCE(IDB_BITMAP7));

  m_timer.initialize();
  
  //brushes for filling rectangles
  m_lb.lbStyle=BS_SOLID;
  m_lb.lbColor=RGB(0,210,210);//cyan
  m_hBrush = ::CreateBrushIndirect(&m_lb);  

  m_lb.lbColor=RGB(255, 255,0); //yellow
  m_hYellowBrush = ::CreateBrushIndirect(&m_lb);

  m_lb.lbColor=RGB(0,0,0);//black
  m_hBlackBrush = ::CreateBrushIndirect(&m_lb);

  m_lb.lbColor=RGB(0, 80, 0);//dark green
  m_hBlueBrush = ::CreateBrushIndirect(&m_lb);  

  m_lb.lbColor=RGB(155,0,0); //redish 
  m_hRedBrush = ::CreateBrushIndirect(&m_lb);  

  loadSprites();//this loads mapping data for IDB_BITMAP2
  m_file.loadLevel("levels\\level1.dat");
  addLevel(1);
  m_bPlaybackMode = false;
  Log("cGameStatePlay initialized...");
  m_score = 0;
  m_bGameActivated = false;
  m_bPaused = false;
  m_Sound.play(SOUND_PENGO);

}

void cGameStatePlay::activate(){}

cGameStateObject* cGameStatePlay::update(){
  m_Sound.update();

  if(m_bPaused == true)
    return 0;    

  POINT player;
  POINT object;
  int type;
  int up;
  int floor;

  if(m_game.isPlayback() == false)
    m_bPlaybackMode = false;

  if(m_bGameActivated == false){
    m_game.newGame();
    m_Sound.stopAll();
    m_Sound.play(SOUND_SEGWAY);
    m_file.loadLevel("levels\\level1.dat");
    addLevel(1);
    m_bGameActivated = true;
  }

  if(m_game.gameOver()){
    //m_game.newGame();
    m_event = GO_MAIN;
    m_bGameActivated = true;
  }
  if(m_game.levelComplete()){
    m_Sound.play(SOUND_LEVEL);
        
    switch(m_game.getLevel()){
    case 1:
      m_file.loadLevel("levels\\level1.dat");
      addLevel(1);
      break;
    case 2:
      m_file.loadLevel("levels\\level2.dat");
      addLevel(2);
      break;
    case 3:
      m_file.loadLevel("levels\\level3.dat");
      addLevel(3);
      break;
    }
    m_game.newLevel();
  }
  else if(m_game.lifeOver()){

    m_game.newLife();
  }

  if(m_bPlaybackMode == false){

    //process all objects and determine if there is a new bottom
    player = m_game.getPlayerPosition();
    player.x += 16;    
    floor = 1600;
    up = m_game.getVertVel();
    POINT pt;

    //let's update position of moving platforms
    for(int i = 0; i < m_objects.size(); i++){
      if(m_objects.getType(i) > 2000)
        m_objects.update(i);
    }

    if(up >= 0){//segway is dropping
      for(int i = 0; i < m_objects.size(); i++){
        type = m_objects.getType(i);
        object.x = m_objects.getPosX(i);  //segway is in same hor pos as object
        object.y = m_objects.getPosY(i) - 32;

        //determine new floor value   
        if(type < 2000){//stationary platforms
          if( player.x >= object.x && player.x <= object.x + 32){
            if(type == 109 || type == 1009 || type == 603){//hard surfaces
              if(player.y <= object.y  && object.y <= floor){
                floor = object.y;        
              }//if
            }//if
          }//if
        }
        else{//moving platforms...this works if platform moves down
          //but if platform moves up then player.y needs to be changed
          if( player.x >= object.x && player.x <= object.x + 32){
            if(type > 2000 && type < 2010){//vertical
              if(player.y <= object.y + 3){
                player.y = object.y - 3;               
                floor = object.y;        
              }//if
            }//if

            else if(type > 2100 && type < 2125){//horizontal movement
              if(player.y <= object.y  && object.y <= floor){
                floor = object.y;       
                if(m_objects.getDir(i) == 1){ //moving right
                  pt = m_game.getPlayerPosition();
                  pt.x += 3;
                  m_game.setPlayerPosition(pt);                
                }
                else{//moving left
                  pt = m_game.getPlayerPosition();
                  pt.x -= 3;
                  m_game.setPlayerPosition(pt);                
                }
              }
            }//if
          }//if
        }
      }//for
    }//if(up

    m_game.setBottom(floor);
    if(player.y > floor + 2){
      m_game.setVertVel(0);
      player = m_game.getPlayerPosition();
      player.y = floor;
      m_game.setPlayerPosition(player);
    }//if
  }//if(m_playbackMode

    //collect tokens and battery
    player = m_game.getPlayerPosition();
    player.x += 16;
    player.y += 16;
   
    if(player.y > 768){
      m_Sound.stopAll();
      m_Sound.resetCountDown();
      m_Sound.play(SOUND_SPLASH);      
    }

    for(int i = 0; i < m_objects.size(); i++){      
      type = m_objects.getType(i);
      object.x = m_objects.getPosX(i);  //segway is in same hor pos as object
      object.y = m_objects.getPosY(i);
      
      if((type == 402 || type == 701 || type == 703 || type == 1205) && m_objects.isActive(i) == 1){
        if(player.x > m_objects.getPosX(i)){
          if(player.x < m_objects.getPosX(i) + 32){
            if(player.y > m_objects.getPosY(i)){
              if(player.y < m_objects.getPosY(i) + 32){ 
                m_objects.disable(i);
                if(type == 1205)
                  m_game.incLevel();
                else if(type == 402){
                  m_Sound.stop(SOUND_GOLD);
                  m_Sound.play(SOUND_GOLD);
                  m_game.addToken();
                }
                else if(type == 701){
                  m_Sound.stop(SOUND_SILVER);
                  m_Sound.play(SOUND_SILVER);
                  m_game.addSilverToken();
                }
                else if(type == 703){
                  m_Sound.stop(SOUND_BATTERY);
                  m_Sound.play(SOUND_BATTERY);
                  m_game.resetEnergy();

                }
              }//if
            }//if
          }//if
        }//if
      }//if(type
    }//for

    //look for contact with skunks
    player = m_game.getPlayerPosition();
    player.x += 16;//center of segway wheel
    player.y += 32;//wheel point of contact

    //skunk sprite is 3 sections: top (12 pix), middle row(10 pix),bottom row(10 pix)
    //segway bottom of wheel kills skunk if it is in middle row
    //if segway bottom of wheel is in bottom row then segway is killed
    for(int i = 0; i < m_objects.size(); i++){      
      type = m_objects.getType(i);
      object.x = m_objects.getPosX(i);  //segway is in same hor pos as object
      object.y = m_objects.getPosY(i);

      //play skunk sound if close to active skunk  
      if(type > 3000 && m_objects.isActive(i) == 1){
        if(player.x > m_objects.getPosX(i) - 64 ){
          if(player.x < m_objects.getPosX(i) + 64){
            if(player.y > m_objects.getPosY(i) - 64){
              if(player.y < m_objects.getPosY(i) + 64){ 
                m_Sound.stop(SOUND_SKUNK);
                m_Sound.play(SOUND_SKUNK);
              }
            }
          }
        }       
      }

      if(type > 3000 && m_objects.isActive(i) == 1){

        if(player.x > m_objects.getPosX(i)){
          if(player.x < m_objects.getPosX(i) + 32){
            if(player.y > m_objects.getPosY(i)+ 2){
              if(player.y < m_objects.getPosY(i) + 22){ 
                m_objects.disable(i); //skunk killed
                if(type < 3125){//black skunk
                  m_Sound.stop(SOUND_SQUISH);
                  m_Sound.play(SOUND_SQUISH);
                  m_game.addBlackSkunkScore();
                }
                else{
                  m_Sound.stop(SOUND_CRUNCH);  
                  m_Sound.play(SOUND_CRUNCH);
                  m_game.addRedSkunkScore();
                }

              }//if
              else if(player.y >= m_objects.getPosY(i) + 22 && player.y < m_objects.getPosY(i) + 54){ 
                m_Sound.stop(SOUND_SCREAM);
                m_Sound.play(SOUND_SCREAM);
                m_game.killSegway(); //segway killed
              }//if

            }//if
          }//if
        }//if
      }//if(type
    }//for


  m_game.update();

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      if(m_event == GO_MAIN)
        m_bGameActivated = false;//this allows reentry with new game      
      return m_TE[i].p_gso;
    }//if
  }//for
  
  return 0;
}

void cGameStatePlay::render(){
  HDC hdcBitmap = ::CreateCompatibleDC(NULL);
  PAINTSTRUCT ps;
  HDC hdc = ::BeginPaint(g_hMainWnd, &ps);

  //clear backbuffer with black
  ::BitBlt(m_hdcBB, 0, 0, m_width, m_height, 0, 0, 0, BLACKNESS);
  ::SelectObject(m_hdcBB,m_hBlackBrush);
  ::Rectangle(m_hdcBB,0,0, m_width, m_height);//top border

  //copies stored bitmap to backbuffer
  RECT vrc = m_game.getView();
  POINT p = m_game.getPlayerPosition();
  int type;
  m_hOldBrush=(HBRUSH)::SelectObject(m_hdcBB, m_hBrush);//cyan
  ::Rectangle(m_hdcBB,400 - p.x, 300 - p.y, 400 + (2400 - p.x), 300 + (800 - p.y));

  //draws entities
  ::SelectObject(hdcBitmap, m_hBitmap);
  for(int i = 0; i < m_objects.size(); i++){
    //if(m_objects.isActive(i) == true){
      if(m_objects.getPosX(i) > p.x - 440 && m_objects.getPosX(i) < p.x + 440 &&
        m_objects.getPosY(i) > p.y - 340 && m_objects.getPosY(i) < p.y + 340){
        type = m_objects.getType(i);
        if(type > 2000 && type < 3000) //this is a moving platform brick 1009
          type = 1009;

        //32x32 objects
        if(type == 106 || type == 109 || type == 201){
            ::BitBlt(m_hdcBB,  (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, m_graphics.getSpriteWidth(type), m_graphics.getSpriteHeight(type), hdcBitmap,m_graphics.getSpriteX(type),m_graphics.getSpriteY(type),SRCCOPY);
        }

        //sprites that have masks
        else if (type == 100 || type == 102 || type == 104 || type == 608||
                type == 305 ||  type == 601 || type == 603 || type == 1205 ||
                type == 1100 || type == 1109 ||type == 1009){
                  ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, m_graphics.getSpriteWidth(type), m_graphics.getSpriteHeight(type), hdcBitmap,m_graphics.getSpriteMaskX(type),m_graphics.getSpriteMaskY(type),SRCAND);
                  ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, m_graphics.getSpriteWidth(type), m_graphics.getSpriteHeight(type), hdcBitmap,m_graphics.getSpriteX(type),m_graphics.getSpriteY(type),SRCPAINT);
        }
        else if (type == 402 || type == 701 || type == 703){//gold token,silver token, battery
          if(m_objects.isActive(i) == 1){
            ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, m_graphics.getSpriteWidth(type), m_graphics.getSpriteHeight(type), hdcBitmap,m_graphics.getSpriteMaskX(type),m_graphics.getSpriteMaskY(type),SRCAND);
            ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, m_graphics.getSpriteWidth(type), m_graphics.getSpriteHeight(type), hdcBitmap,m_graphics.getSpriteX(type),m_graphics.getSpriteY(type),SRCPAINT);
          }
        }
        else if (type > 3000 && type < 3125){//marching of the black skunks
          if(m_objects.isActive(i) == 1){
            if(m_objects.getDir(i) == 1){
              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 160 + (32 * m_objects.getFrame(i)), 288 ,SRCAND);//mask
              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 32 + (32 * m_objects.getFrame(i)), 288,SRCPAINT);
            }
            else if(m_objects.getDir(i) == 0){
              if(m_objects.getFrame(i) < 3)
                ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 224 + (32 * m_objects.getFrame(i)), 256 ,SRCAND);//mask
              else
                ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 0, 288 ,SRCAND);//mask

              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 96 + (32 * m_objects.getFrame(i)), 256,SRCPAINT);
            }
          }
        }
        else if (type > 3200 && type < 3325){//marching of the red skunks
          if(m_objects.isActive(i) == 1){
            if(m_objects.getDir(i) == 1){
              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 160 + (32 * m_objects.getFrame(i)), 288 ,SRCAND);//mask
              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 160 + (32 * m_objects.getFrame(i)), 320,SRCPAINT);
            }
            else if(m_objects.getDir(i) == 0){
              if(m_objects.getFrame(i) < 3)
                ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 224 + (32 * m_objects.getFrame(i)), 256 ,SRCAND);//mask
              else
                ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 0, 288 ,SRCAND);//mask

              ::BitBlt(m_hdcBB, (m_objects.getPosX(i) - p.x) + 400, (m_objects.getPosY(i) - p.y) + 300, 32, 32,hdcBitmap, 32 + (32 * m_objects.getFrame(i)), 320,SRCPAINT);
            }
          }
        }

        }//if(m_objects
    //}//if(m_objects
  }//for

  //draws segway
  int anim = m_game.getFrameNum();
  if(p.y < 768){
  switch(anim){
    case 0:
      if(m_game.getDirection() == 0){
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 0, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 64, 32, SRCPAINT);
      }
      else{
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 32, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 288, 32, SRCPAINT);
      }
      break;
    case 1:
      if(m_game.getDirection() == 0){
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 0, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 96, 32, SRCPAINT);
      }
      else{
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 32, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 256, 32, SRCPAINT);
      }
      break;
    case 2:
      if(m_game.getDirection() == 0){
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 0, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 128, 32, SRCPAINT);
      }
      else{
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 32, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 224, 32, SRCPAINT);
      }
      break;
    case 3:
      if(m_game.getDirection() == 0){
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 0, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 160, 32, SRCPAINT);
      }
      else{
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 32, 64, SRCAND);
        ::BitBlt(m_hdcBB, 400, 300, 32, 32, hdcBitmap, 192, 32, SRCPAINT);
      }
      break;
    }
  }

  //draws overlay
  ::SelectObject(m_hdcBB,m_hBlueBrush);
  ::Rectangle(m_hdcBB,0,0, m_width, 100);//top border
  ::Rectangle(m_hdcBB,0, m_height - 120, m_width,m_height);//bottom border
  ::SelectObject(m_hdcBB,m_hOldBrush);

  //draws map
  int mapX = 40;
  int mapY = m_height - 110;
  POINT player = m_game.getPlayerPosition();
  POINT exitLevel = m_game.getExit();
  player.x /= 20;
  player.y /= 20;
  exitLevel.x /= 20;
  exitLevel.y /= 20;
  ::SelectObject(m_hdcBB,m_hBlackBrush); //black 160x70 = 3200x1400
  ::Rectangle(m_hdcBB, mapX, mapY, mapX + 160, mapY + 70);

  ::SelectObject(m_hdcBB, m_hBrush); //blue 120x40 = 2400x800
  ::Rectangle(m_hdcBB, mapX + 20, mapY + 15, mapX + 140, mapY + 55);

  ::SelectObject(m_hdcBB,m_hYellowBrush);//player position
  ::Rectangle(m_hdcBB,20 + mapX + player.x - 3 , 15 + mapY + player.y - 3, 20 + mapX + player.x + 3 , 15 + mapY + player.y + 3);
  ::SelectObject(m_hdcBB,m_hOldBrush);

  ::SelectObject(m_hdcBB,m_hRedBrush);//exit position
  ::Rectangle(m_hdcBB,20 + mapX + exitLevel.x - 3 , 15 + mapY + exitLevel.y - 3, 20 + mapX + exitLevel.x + 3 , 15 + mapY + exitLevel.y + 3);
  ::SelectObject(m_hdcBB,m_hOldBrush);

  ::SelectObject(m_hdcBB,m_hYellowBrush);//viewport
  RECT view;
  view.left = 20 + mapX + player.x - 20;
  view.top = 15 + mapY + player.y - 15; 
  view.right = view.left + 40; 
  view.bottom = view.top + 30;
  ::FrameRect(m_hdcBB,&view, m_hYellowBrush);


  //draws power grid
  ::SelectObject(hdcBitmap, m_hBling);
  ::BitBlt(m_hdcBB, 10, 10, m_game.getEnergy() , 77, hdcBitmap, 156, 289, SRCAND);
  ::BitBlt(m_hdcBB, 10, 10, m_game.getEnergy() , 77, hdcBitmap, 1, 289, SRCPAINT);

  //power label
  ::BitBlt(m_hdcBB, 30, 60, 108, 47, hdcBitmap, 1, 93, SRCAND);
  ::BitBlt(m_hdcBB, 30, 60, 108, 47, hdcBitmap, 1, 1, SRCPAINT);

  //level
  GRAPHIC_DIGIT glevel = getFontData(m_game.getLevel());  
  //digit
  ::BitBlt(m_hdcBB, 475, 10, 35, 50, hdcBitmap, glevel.digit[0], 237, SRCAND);//mask
  ::BitBlt(m_hdcBB, 475, 10, 35, 50, hdcBitmap, glevel.digit[0], 185, SRCPAINT);//font
  //word level
  ::BitBlt(m_hdcBB, 450, 60, 94, 39, hdcBitmap, 118, 141, SRCAND);
  ::BitBlt(m_hdcBB, 450, 60, 94, 39, hdcBitmap, 118, 49, SRCPAINT);

  //displays lives
  int lives = m_game.getLives();
  if(lives > 0){
    ::BitBlt(m_hdcBB, 240, 10, 35, 54, hdcBitmap, 452, 57, SRCAND);
    ::BitBlt(m_hdcBB, 240, 10, 35, 54, hdcBitmap, 415, 1, SRCPAINT);
  }
  if(lives > 1){
    ::BitBlt(m_hdcBB, 280, 10, 35, 54, hdcBitmap, 452, 57, SRCAND);
    ::BitBlt(m_hdcBB, 280, 10, 35, 54, hdcBitmap, 415, 57, SRCPAINT);
  }
  if(lives > 2){
    ::BitBlt(m_hdcBB, 320, 10, 35, 54, hdcBitmap, 452, 57, SRCAND);
    ::BitBlt(m_hdcBB, 320, 10, 35, 54, hdcBitmap, 415, 113, SRCPAINT);
  }
  ::BitBlt(m_hdcBB, 250, 60, 96, 46, hdcBitmap, 117, 93, SRCAND);
  ::BitBlt(m_hdcBB, 250, 60, 96, 46, hdcBitmap, 117, 1, SRCPAINT);

  //my name
  ::BitBlt(m_hdcBB, 303, 545, 194, 34, hdcBitmap, 1, 483, SRCAND);
  ::BitBlt(m_hdcBB, 303, 545, 194, 34, hdcBitmap, 1, 447, SRCPAINT);

  //prints numbers
  ::SelectObject(hdcBitmap, m_hBling);
  
  //display score
  GRAPHIC_DIGIT g = getFontData(m_game.getScore());  
  int maxDigit = 5;
  
  if(m_game.getScore() < 10)
    maxDigit = 1;
  else if(m_game.getScore() < 100)
    maxDigit = 2;
  else if(m_game.getScore() < 1000)
    maxDigit = 3;
  else if(m_game.getScore() < 10000)
    maxDigit = 4;

  for(int i=0; i<maxDigit;i++){
    ::BitBlt(m_hdcBB, 600 + ((5 - maxDigit)*32) + (i * 35), 525, 35, 50, hdcBitmap,g.digit[i], 237, SRCAND);//mask
    ::BitBlt(m_hdcBB, 600 + ((5 - maxDigit)*32) + (i * 35), 525, 35, 50, hdcBitmap,g.digit[i], 185, SRCPAINT);//font
  }
  ::BitBlt(m_hdcBB, 650, 485, 99, 45, hdcBitmap,1, 138, SRCAND);//mask
  ::BitBlt(m_hdcBB, 650, 485, 99, 45, hdcBitmap,1, 46, SRCPAINT);//font
  
  //display remaining time
  GRAPHIC_DIGIT gs = getFontData(m_game.getTime());  
  maxDigit = 3;

  if(m_game.getTime() < 10)
    maxDigit = 1;
  else if(m_game.getTime() < 100)
    maxDigit = 2;

  for(int i=0; i<maxDigit;i++){
    ::BitBlt(m_hdcBB, 625 + ((3 - maxDigit)*32) + (i * 35), 10, 35, 50, hdcBitmap,gs.digit[i], 237, SRCAND);//mask
    ::BitBlt(m_hdcBB, 625 + ((3 - maxDigit)*32) + (i * 35), 10, 35, 50, hdcBitmap,gs.digit[i], 185, SRCPAINT);//font
  }
  ::BitBlt(m_hdcBB, 635, 60, 89, 44, hdcBitmap, 238, 93, SRCAND);//mask
  ::BitBlt(m_hdcBB, 635, 60, 89, 44, hdcBitmap, 238, 1, SRCPAINT);//font

  //display player coordinates
  POINT pt = m_game.getPlayerPosition();
  ::SetTextColor(m_hdcBB,RGB(255,255,255));
  ::SetBkColor(m_hdcBB,RGB(0,80,0));
  PrintText(m_hdcBB,"X: ", pt.x,210,505);
  PrintText(m_hdcBB,"Y: ", pt.y,210,535);

  //display GI challenge
  //::SetTextColor(m_hdcBB,RGB(255,255,0));
  //PrintText(m_hdcBB,"GI Game Challenge 2",330,505);
  //PrintText(m_hdcBB,"<July 13 - August 12, 2007>",310,530);
  ::BitBlt(m_hdcBB, 272, 505, 252, 45, hdcBitmap, 198, 448, SRCAND);//mask
  ::BitBlt(m_hdcBB, 272, 505, 252, 45, hdcBitmap, 198, 449, SRCPAINT);//font

  //display pause image if paused
  if(m_bPaused == true){
    ::BitBlt(m_hdcBB, 332, 230, 164, 78, hdcBitmap, 311, 368, SRCAND);//mask
    ::BitBlt(m_hdcBB, 332, 230, 164, 78, hdcBitmap,311, 289, SRCPAINT);//font
  }

  //copies backbuffer to front buffer
  ::BitBlt(hdc, 0, 0, m_width, m_height, m_hdcBB, 0, 0, SRCCOPY);
  ::ReleaseDC(g_hMainWnd, hdc);
  ::DeleteObject(hdcBitmap);
  ::EndPaint(g_hMainWnd,&ps); 
}

void cGameStatePlay::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  m_event = GO_NO_WHERE;
  
  if(m_bPlaybackMode == false){
    if(keyDown(g_KeySelection.moveLeft))
      m_game.movePlayerLeft(2);
    if(keyDown(g_KeySelection.moveRight))
      m_game.movePlayerRight(2);
    if(keyDown(g_KeySelection.moveJump)){
      m_Sound.stop(SOUND_BOUNCE2);
      m_Sound.play(SOUND_BOUNCE2);     
      m_game.movePlayerUp(22);
    }
    if(keyDown(g_KeySelection.moveBrake))
      m_game.movePlayerStop();
    if(keyDown(0x50)){ //P{
      if(m_bPaused == false)
        m_bPaused = true;
      else
        m_bPaused = false;
    }
  }

  if(keyDown(VK_ESCAPE)){ //M{
    m_Sound.stopAll(); 
    m_event = GO_MAIN;
  }
  
  /*if(keyDown(0x52)){//Q
    if(m_game.isPlayback() == false){
      m_game.playBack();
    }
  } */
}

GRAPHIC_DIGIT cGameStatePlay::getFontData(int value){
  std::ostringstream oss;
  std::string sNum;
  GRAPHIC_DIGIT gd;
  gd.digit[0]=1;
  gd.digit[1]=1;
  gd.digit[2]=1;
  gd.digit[3]=1;
  gd.digit[4]=1;

  oss << value;
  sNum = oss.str();
  int digit[]= {1,36,70,106,139,172,208,240,275,310};
  int number = 0;
  //Log("sNum",sNum);
  for(int i=0; i < sNum.size(); i++){
    number = 0;
    if (sNum.at(i) == '0')
      number = 0;
    else if(sNum.at(i) == '1')
      number = 1;
    else if(sNum.at(i) == '2')
      number = 2;
    else if(sNum.at(i) == '3')
      number = 3;
    else if(sNum.at(i) == '4')
      number = 4;
    else if(sNum.at(i) == '5')
      number = 5;
    else if(sNum.at(i) == '6')
      number = 6;
    else if(sNum.at(i) == '7')
      number = 7;
    else if(sNum.at(i) == '8')
      number = 8;
    else if(sNum.at(i) == '9')
      number = 9;

    switch(i){
      case 0:
        gd.digit[i] = digit[number];
        break;
      case 1:
        gd.digit[i] = digit[number];
        break;
      case 2:
        gd.digit[i] = digit[number];
        break;
      case 3:
        gd.digit[i] = digit[number];
        break;
      case 4:
        gd.digit[i] = digit[number];
        break;
      case 5:
        gd.digit[i] = digit[number];
        break;
      case 6:
        gd.digit[i] = digit[number];
        break;
      case 7:
        gd.digit[i] = digit[number];
        break;
      case 8:
        gd.digit[i] = digit[number];
        break;
      case 9:
        gd.digit[i] = digit[number];
        break;
    }
  }
  return gd;
}

void cGameStatePlay::deactivate(){
}
void cGameStatePlay::resume(){
}
void cGameStatePlay::pause(){
}
void cGameStatePlay::save(){
}

void cGameStatePlay::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

//loads entities to be displayed on the level
void cGameStatePlay::addLevel(int level){
  POINT levelExit;

  if(level < 1 || level > 3)
    return; //legal levels are 1, 2 and 3

  m_game.setLevel(level);

  m_objects.clearAll();//clears entity data used by game play
  
  for(int i = 0; i < m_file.size(); i++){
    m_objects.addEntity( m_file.getType(i),
                          m_file.getPosX(i),
                          m_file.getPosY(i));
    if(m_file.getType(i) == 1205){ //location of level exit
      levelExit.x = m_file.getPosX(i);
      levelExit.y = m_file.getPosY(i);
      m_game.setExit(levelExit);
    }
  }
  m_file.clearData();  //clears previously stored file data

  //load player starting data
  POINT player;
  if(m_objects.getType(0) == 1){
    player.x = m_objects.getPosX(0);
    player.y = m_objects.getPosY(0);
    m_game.setPlayerInitialPosition(player);
  }
  else{
    player.x = 108;
    player.y = 660;
    m_game.setPlayerInitialPosition(player);

  }
  
}

//loads sprites
void cGameStatePlay::loadSprites(){
  m_graphics.addSprite(100,0,0,32,32,32,0,3);//center dirt w/ grass lower
  m_graphics.addSprite(102,64,0,32,32,96,0,3);//right dirt w/grass lower
  m_graphics.addSprite(104,128,0,32,32,160,0,3);//left dirt w/grass lower
  m_graphics.addSprite(106,192,0,32,32,288,128,1);//32x32 dirt
  m_graphics.addSprite(109,288,0,32,32,288,128,1);//32x32 brick
  m_graphics.addSprite(201,32,32,32,32,288,128,1);//32x32 brick - alternative
  m_graphics.addSprite(305,160,64,32,32,32,0,3);//lower metal
  m_graphics.addSprite(402,64,96,32,32,96,96,5);//token gold
  m_graphics.addSprite(601,32,160,32,32,32,0,2);//color panel upper
  m_graphics.addSprite(603,96,160,32,32,128, 160,2);//color panel upper
  m_graphics.addSprite(608,256,160,32,32,288,160,6);//32x32 lava
  m_graphics.addSprite(701,32,192,32,32,64,192,5);//token silver
  m_graphics.addSprite(703,96,192,32,32,224,192,9); //battery
  m_graphics.addSprite(1009,288,288,32,32,128,160,2);//upper half brick
  m_graphics.addSprite(1100,0,320,32,32,32,0,3); //lower half brick
  m_graphics.addSprite(1109,288,320,32,32,0,352,6);//water full
  m_graphics.addSprite(1205,160,352,32,32,192,352,5);//exit
}