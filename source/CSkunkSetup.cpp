/*====================================================================
File:    Graphics.cpp
Author:  Chuck Bolin, May 2006
Purpose: Defines windows based function for use with console
         programming.
====================================================================*/
#include "..\includes\CSkunkSetup.h"

CSkunkSetup::CSkunkSetup(){
  selectDefaultKeyboard();

  for(int i = 0; i < 0x60; i++)
    m_keys[i] = "Not used";
  
  m_keys[VK_SPACE] = "SPACEBAR";
  m_keys[VK_SHIFT] = "SHIFT";
  m_keys[VK_CONTROL] = "CTRL";
  m_keys[VK_LEFT] = "Left Arrow";
  m_keys[VK_RIGHT] = "Right Arrow";
  m_keys[VK_UP] = "Up Arrow";
  m_keys[VK_DOWN] = "Down Arrow";
  m_keys[VK_PRIOR] = "Page Up";
  m_keys[VK_NEXT] = "Page Down";
  m_keys[VK_HOME] = "Home";
  m_keys[VK_END] = "End";
  m_keys[VK_INSERT] = "Insert";
  m_keys[VK_DELETE] = "Delete";

  std::ostringstream oss;
  std::string ch;
  char c;
  for(int i = 0x30; i< 0x60; i++){
    c = i;
    oss.str("");
    oss << c;
    ch = oss.str();
    m_keys[i] = ch;
  }

  //arrows
  commonSelection[0] = m_keyboard;
  
  //Z,X,C,Space
  commonSelection[1].moveLeft = 'Z';
  commonSelection[1].moveJump = 'X';
  commonSelection[1].moveRight = 'C';
  commonSelection[1].moveBrake = VK_SPACE;

  //W,A,S,D
  commonSelection[2].moveLeft = 'A';
  commonSelection[2].moveJump = 'W';
  commonSelection[2].moveRight = 'D';
  commonSelection[2].moveBrake = 'S';
}

void CSkunkSetup::selectDefaultKeyboard(){
  m_keyboard.moveBrake = VK_DOWN;
  m_keyboard.moveJump = VK_UP;
  m_keyboard.moveLeft = VK_LEFT;
  m_keyboard.moveRight = VK_RIGHT;
}

std::string CSkunkSetup::getPlayerName(){
  setColor(15, 1);
  system("cls");
  posCursor(1,1);
  writeLine("**********************************************************************");
  posCursor(1,3);
  writeLine("**********************************************************************");
  
  posCursor(1,23);
  writeLine("**********************************************************************");
  for(int i = 2; i < 23; i++){
    posCursor(1, i);
    writeLine("*");
    posCursor(70, i);
    writeLine("*");
  }
  setColor(14, 1);
  posCursor(20, 2);
  writeLine("\"S K U N K E D\"    S E T U P");
  std::string sName;
  posCursor(3,5);
  writeLine("Enter name:");
  posCursor(15, 5);
  setColor(10, 1);
  sName = readLine(15, 5);
  sName = sName.substr(1);
  setCaption(sName.c_str());
  return sName;
}

void CSkunkSetup::setCaption(std::string sTitle){
  ::SetConsoleTitle(sTitle.c_str());
}

KEY_SELECTION CSkunkSetup::getKeyboardSelection(){
  return m_keyboard;
}

void CSkunkSetup::openConsole(){
  ::AllocConsole();
}
void CSkunkSetup::closeConsole(){
  ::FreeConsole();
}

//sets color of text - color range is 0 to 15
void CSkunkSetup::setColor(int foreColor, int backColor)
{
  HANDLE hOut;
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hOut, backColor * 16 + foreColor);
}

void CSkunkSetup::writeLine(std::string sLine){
  HANDLE hOut;
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  ::WriteConsole(hOut,(LPVOID) sLine.c_str(),sLine.size(), NULL, NULL);
}

int CSkunkSetup::getKeyPress(int x, int y){
  HANDLE hInput;
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD InRec;
  DWORD NumRead;
  std::string sLine = "";
  std::ostringstream oss;

  ReadConsoleInput(hInput,&InRec,1,&NumRead);
  return (int)InRec.Event.KeyEvent.uChar.AsciiChar;
}

std::string CSkunkSetup::readLine(int x, int y){
  HANDLE hInput;
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD InRec;
  DWORD NumRead;
  bool quit = false;
  std::string sLine = "";
  std::ostringstream oss;
  
  while (!quit){
    ReadConsoleInput(hInput,&InRec,1,&NumRead);

    switch (InRec.EventType)
    {
    case KEY_EVENT:
      
      if (InRec.Event.KeyEvent.uChar.AsciiChar == VK_ESCAPE){
        quit = true;
      }
      else if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_RETURN){
        return sLine;
      }
      else{
        if(InRec.Event.KeyEvent.bKeyDown == true){
          oss << InRec.Event.KeyEvent.uChar.AsciiChar;
          sLine += oss.str();
          oss.str("");
          posCursor(x, y);
          writeLine(sLine);
        }
      }
      break;
    }
  }  
  return "";  
}

//positions cursor in x,y coordinates
void CSkunkSetup::posCursor( int x, int y) 
{
  HANDLE hOut;
  COORD cursorpos;
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cursorpos.X = x - 1;
  cursorpos.Y = y - 1;
  SetConsoleCursorPosition(hOut, cursorpos); 
} 

void CSkunkSetup::setupKeyboard(){
  HANDLE hInput;
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD InRec;
  DWORD NumRead;
  KEY_SELECTION ks = m_keyboard;  
  bool quit = false;
  std::string sLine = "";
  std::ostringstream oss;
  int x = 0;
  int y = 0;
  int numVert = 0;
  int numHor = 32;
  int key;
  bool bReturn = false;
  int common = 0; //arrow key controls

  setColor(14, 1);
  posCursor(3, 9);
  writeLine("Move Left:    ");
  posCursor(3, 11);
  writeLine("Move Right:   ");
  posCursor(3, 13);
  writeLine("Jump:         ");
  posCursor(3, 15);
  writeLine("Brake (slow): ");

  setColor(11, 1);
  posCursor(18, 9);
  writeLine(m_keys[ks.moveLeft]);
  setColor(10, 1);
  posCursor(18, 11);
  writeLine(m_keys[ks.moveRight]);;
  posCursor(18, 13);
  writeLine(m_keys[ks.moveJump]);
  posCursor(18, 15);
  writeLine(m_keys[ks.moveBrake]);
  std::ostringstream os;
  int lastKey = 0;
      
  //instructions
  setColor(15,1);
  posCursor(25,17);
  writeLine("<<< Instructions >>>");
  posCursor(15,19);
  writeLine("Left/right arrows - select key");
  posCursor(15,20);
  writeLine("Up/down arrows    - select action");
  posCursor(15,21);
  writeLine("Press X to accept, ESC to use default keys");
  setColor(13,1);
  posCursor(15,22);
  writeLine("Page Up/Page Down.........Quick Key Select");

  while (!quit){

    bReturn = ReadConsoleInput(hInput,&InRec,1,&NumRead);
    
    key = InRec.Event.KeyEvent.wVirtualKeyCode;
    if(key == lastKey){
      ::Sleep(25);
      lastKey = 0;
    }
    else{
      lastKey = key;

      if(key == VK_ESCAPE){
        return;
      }
      else if(key == 0){

      }
      else if(key == 'X'){
        
        m_keyboard = ks;
        return;
      }
      else if(key == VK_NEXT){//page up
        common++;
        if(common >2)
          common = 0;
        ks = commonSelection[common];
      }
      else if(key == VK_PRIOR){//page down
        common--;
        if(common < 0)
          common = 2;
        ks = commonSelection[common];
      }

      else if(key == VK_UP){
        
        numVert--;
        if(numVert<0)
          numVert = 3;
        switch(numVert){
        case 0:
          numHor = ks.moveLeft;
          break;
        case 1:
          numHor = ks.moveRight;
          break;
        case 2:
          numHor = ks.moveJump;
          break;
        case 3:
          numHor = ks.moveBrake;
          break;
        }
      }
      else if(key == VK_DOWN){
        numVert++;
        if(numVert > 3)
          numVert = 0;
        switch(numVert){
        case 0:
          numHor = ks.moveLeft;
          break;
        case 1:
          numHor = ks.moveRight;
          break;
        case 2:
          numHor = ks.moveJump;
          break;
        case 3:
          numHor = ks.moveBrake;
          break;
        }
      }
      else if(key == VK_LEFT){
        numHor--;
        if(numHor < 0x20)
          numHor = 0x20;
        switch(numVert){
        case 0:
          ks.moveLeft = numHor;
          break;
        case 1:
          ks.moveRight = numHor;
          break;
        case 2:
          ks.moveJump = numHor;
          break;
        case 3:
          ks.moveBrake = numHor;
          break;
        }      
      }
      else if(key == VK_RIGHT){
        numHor++;
        if(numHor > 0x5F)
          numHor = 0x5F;
        switch(numVert){
        case 0:
          ks.moveLeft = numHor;
          break;
        case 1:
          ks.moveRight = numHor;
          break;
        case 2:
          ks.moveJump = numHor;
          break;
        case 3:
          ks.moveBrake = numHor;
          break;
        }      
      }

      //print results
      if(numVert == 0)
        setColor(11,1);
      else
        setColor(10,1);
      posCursor(18, 9);
      writeLine("                            ");
      posCursor(18, 9);
      writeLine(m_keys[ks.moveLeft]);

      if(numVert == 1)
        setColor(11,1);
      else
        setColor(10,1);
      posCursor(18, 11);
      writeLine("                            ");
      posCursor(18, 11);
      writeLine(m_keys[ks.moveRight]);;

      if(numVert == 2)
        setColor(11,1);
      else
        setColor(10,1);
      posCursor(18, 13);
      writeLine("                            ");
      posCursor(18, 13);
      writeLine(m_keys[ks.moveJump]);

      if(numVert == 3)
        setColor(11,1);
      else
        setColor(10,1);
      posCursor(18, 15);
      writeLine("                            ");
      posCursor(18, 15);
      writeLine(m_keys[ks.moveBrake]);
    }  
  }
  m_keyboard = ks;
}

//sets full screen mode - text..no graphics
void CSkunkSetup::fullScreen(void){
  typedef BOOL (WINAPI *PROCSETCONSOLEDISPLAYMODE)(HANDLE,DWORD,LPDWORD);
  typedef BOOL (WINAPI *PROCGETCONSOLEDISPLAYMODE)(LPDWORD);
  PROCSETCONSOLEDISPLAYMODE SetConsoleDisplayMode;
  PROCGETCONSOLEDISPLAYMODE GetConsoleDisplayMode;
  HMODULE hKernel32 = GetModuleHandle("kernel32");
  SetConsoleDisplayMode = (PROCSETCONSOLEDISPLAYMODE )
  GetProcAddress(hKernel32,"SetConsoleDisplayMode");
  GetConsoleDisplayMode = (PROCGETCONSOLEDISPLAYMODE)
  GetProcAddress(hKernel32,"GetConsoleDisplayMode");
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwOldMode;
  SetConsoleDisplayMode(hOut,1,&dwOldMode);
}