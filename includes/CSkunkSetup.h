#ifndef CSKUNKSETUP_H
#define CSKUNKSETUP_H

#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

struct KEY_SELECTION{
  int moveLeft;
  int moveRight;
  int moveJump;
  int moveBrake;
};

//console graphics functions
class CSkunkSetup{
public:
  CSkunkSetup();
  void setColor(int foreColor, int backColor);
  void posCursor( int x, int y);
  void fullScreen(void);
  void writeLine(std::string sLine);
  
  std::string readLine(int x, int y);  
  int getKeyPress(int x, int y);
  void openConsole();
  void closeConsole();
  void setupKeyboard();
  void selectDefaultKeyboard();
  std::string getPlayerName();
  KEY_SELECTION getKeyboardSelection();
  KEY_SELECTION commonSelection[3];
  void setCaption(std::string);

private:
  KEY_SELECTION m_keyboard;  //stores 4 key integers
  std::string m_keys[128];//0x60];           //stores names to display for key
};

#endif