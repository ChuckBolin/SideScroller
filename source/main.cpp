//=========================================================================
// ChuckSideScroller - Written by Chuck Bolin, July 2007
// GameInstitute.com Game Challenge #2
// main.cpp - Contains WinMain and WinProc functions
//=========================================================================

//============================================================
// Includes
//============================================================
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "resource.h"
#include "..\includes\cGameStateObject.h"
#include "..\includes\cGameStateIntro.h"
#include "..\includes\cGameStateMain.h"
#include "..\includes\cGameStatePause.h"
#include "..\includes\cGameStatePlay.h"
#include "..\includes\cGameStateHelp.h"
#include "..\includes\cGameStateScore.h"
#include "..\includes\cGameStateQuit.h"
#include "..\includes\keystatus.h"
//#include "..\includes\log_templates.h"
#include "..\includes\CTimer.h"
#include "..\includes\CHighScore.h"
#include "..\includes\CSkunkSetup.h"

//============================================================
// Globals
//============================================================
HWND			g_hMainWnd = 0;
UINT      g_Msg = 0;
WPARAM    g_wParam;
LPARAM    g_lParam;
HINSTANCE	g_hAppInst = 0;
HMENU			ghMainMenu = 0;
HDC				ghDC = 0;
HWND      ghDlg = 0;
int       g_frameRate = 0;
HWND      g_DialogScore = 0;//High score dialog box
std::string gWndCaption = "..Skunked.. - v0.19 August 11, 2007 by Chuck Bolin";

template<typename T> void Log(T value1);

//game state creation
cGameStateObject* p_GS_Intro = new cGameStateIntro;
cGameStateObject* p_GS_Main = new cGameStateMain;
cGameStateObject* p_GS_Pause = new cGameStatePause;
cGameStateObject* p_GS_Play = new cGameStatePlay;
cGameStateObject* p_GS_Help = new cGameStateHelp;
cGameStateObject* p_GS_Quit = new cGameStateQuit;
cGameStateObject* p_GS_Score = new cGameStateScore;
cGameStateObject* p_Current = 0;
cGameStateObject* p_Last = 0; 
cGameStateObject* p_Next = 0;

//tracking high score
CHighScore g_HighScore("scores.txt",10);
CSkunkSetup g_Setup;
std::string g_PlayerName;
KEY_SELECTION g_KeySelection;

//displays FPS in caption bar
void DrawFPS(int frameCount){
  std::ostringstream oss;
  oss <<" " << frameCount << " FPS ";
  std::string newCaption = gWndCaption + oss.str();
  SetWindowText(g_hMainWnd, newCaption.c_str());
}

//============================================================
// WndProc - Processes messages
//============================================================
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_hMainWnd = hWnd;
  g_Msg = msg;
  g_wParam = wParam;
  g_lParam = lParam;
  static bool quit = false;
 
  switch(msg)
	{
	case WM_CREATE:
    p_Current = p_GS_Intro;
    p_Last = p_Current;
  
    p_GS_Intro->initialize();
    p_GS_Score->initialize();
    p_GS_Main->initialize();
    p_GS_Help->initialize();
    p_GS_Play->initialize();

    ::SetCursorPos(0,0);
    ::InvalidateRect(hWnd,0,0);
    Log("Program window created...");
		return 0;

	case WM_PAINT:
    p_Current->render();
		return 0;

  case WM_SIZE:
    break;

  case WM_COMMAND:
    break;
  
  case WM_KEYDOWN:
  case WM_KEYUP:
    p_Current->processEvent(hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
    delete p_GS_Intro;
    delete p_GS_Main;
    delete p_GS_Pause;
    delete p_GS_Play;
    delete p_GS_Help;
    delete p_GS_Quit;
    delete p_GS_Score;

 		PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK
EditDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
  char msgText[256]; 
  static HWND hEditBox = 0;
  switch(msg){
    case WM_INITDIALOG:
      hEditBox = ::GetDlgItem(hDlg, IDC_EDIT1);
      ::SetWindowText(hEditBox, "Enter your name here");
      return true;
    case WM_COMMAND:
      switch(LOWORD(wParam)){
        case IDC_BUTTON1:
          ::GetWindowText(hEditBox, msgText, 256);
          //::EndDialog(hDlg, 0);
          return true;    
        }
      return true;  
    case WM_CLOSE:
      ::DestroyWindow(hDlg);
      return true;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return true;
  }
  return false;
}

//=====================================================================
// WinMain( )
//
//All windows programs begin here at this function WinMain
//=====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR cmdLine, int showCmd)
{
  g_hAppInst = hInstance;
//  Log("*************************************");
 // Log(" Chuck's Sidescroller Game           ");
 // Log(" PROGRAM STARTING                    ");
  //LogDate();
  //LogTime();
 // Log("*************************************");
  
	//#2 - Fill out WNDCLASS instance
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;  //this must match function above 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hAppInst;
	wc.hIcon = LoadIcon(g_hAppInst, IDI_APPLICATION);//MAKEINTRESOURCE(IDI_ICON1));      //
  wc.hCursor = ::LoadCursor(g_hAppInst,IDC_ARROW);// LoadCursor(g_hAppInst, MAKEINTRESOURCE(IDC_CURSOR1));//IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//WHITE_BRUSH);//0
	wc.lpszMenuName = 0;
	wc.lpszClassName = "ChuckSideScroller";

	//#3 - Register WNDCLASS with windows
	RegisterClass(&wc);
	//Log("Register Class...Success!");
  
	//#4 - Create the window...doesn't show it visibly
	//HMENU hMenu = LoadMenu(g_hAppInst, MAKEINTRESOURCE(IDR_MENU));
	//ghMainMenu = hMenu;
  int width = GetSystemMetrics(SM_CXSCREEN);
  int height = GetSystemMetrics(SM_CYSCREEN);
 // Log("SM_CXSCREEN",width);
 // Log("SM_CYSCREEN",height);
  int leftOffset = 0;//default if screen is < 800x600
  int topOffset = 0;

  if(width > 800 && height > 600){//if screen is > 800x600
    leftOffset = (width - 800)/2;
    topOffset = (height - 600)/2;
    width = 800;
    height = 600;
  }

 // Log("App Window Width",width);
 // Log("App Window Height",height);

	HWND hWnd = CreateWindow("ChuckSideScroller", gWndCaption.c_str(),
    WS_OVERLAPPED | WS_SYSMENU,leftOffset, topOffset, width, height,
    0, ghMainMenu,g_hAppInst, 0);

	if(hWnd == 0) 
	{
		MessageBox(0,"Creation of Window has Failed!", 0,0);
    //Log("Creation of window has failed");
  //  Log("Terminating Program");

    //release memory on the heap for game state stuff
    delete p_GS_Intro;
    delete p_GS_Main;
    delete p_GS_Pause;
    delete p_GS_Play;
    delete p_GS_Help;
    delete p_GS_Quit;
    delete p_GS_Score;
		return 0; 
	}
  //console stuff
  g_Setup.openConsole();
  g_Setup.setCaption("Skunked Setup Screen");
  g_PlayerName = g_Setup.getPlayerName();
  g_Setup.setupKeyboard();
  g_KeySelection = g_Setup.getKeyboardSelection();
  g_Setup.closeConsole();

  //#5 - Show and update window
	ShowWindow(hWnd, showCmd);
	UpdateWindow(hWnd);

  //map game state information
  p_GS_Intro->addTransitionEvent(GO_MAIN, p_GS_Main);//
  p_GS_Main->addTransitionEvent(GO_PLAY, p_GS_Play);//
  p_GS_Main->addTransitionEvent(GO_HELP, p_GS_Help);//
  p_GS_Main->addTransitionEvent(GO_QUIT, p_GS_Quit);
  p_GS_Main->addTransitionEvent(GO_SCORE, p_GS_Score);//
  p_GS_Play->addTransitionEvent(GO_MAIN, p_GS_Main);//
  p_GS_Play->addTransitionEvent(GO_PAUSE, p_GS_Pause);//
  p_GS_Pause->addTransitionEvent(GO_PLAY, p_GS_Play);//
  p_GS_Help->addTransitionEvent(GO_MAIN, p_GS_Main);//
  p_GS_Score->addTransitionEvent(GO_MAIN, p_GS_Main);//

  // Get the performance timer frequency.
  CTimer mainTimer;
  mainTimer.initialize();
	CTimer frameCountTimer;
  frameCountTimer.initialize();
  int frameCount = 0;
  bool quit = false;
 
 // Log("Entering message loop!");
	MSG msg;
	ZeroMemory (&msg, sizeof(MSG));

  //ghDlg = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOGBAR),0, EditDlgProc);
  //::ShowWindow(ghDlg, showCmd);

  while(msg.message != WM_QUIT)
  {
    //if there is a windows messgage process it here 
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      if(mainTimer.getTimer(0.1)){
        p_Next = p_Current->update();

        if(p_Next == p_GS_Quit){
          quit = true;
        }
    
        if(p_Next != 0 && !quit){ //state change
          p_Last = p_Current;     
          p_Current = p_Next;
        }
        if(quit){
          DestroyWindow(hWnd);
          return 0;
        }
      }
    
     //update graphics as fast as possible
      frameCount++;
      if(frameCountTimer.secondTimer()){
        DrawFPS(frameCount);
        frameCount = 0;
      }
     ::InvalidateRect(g_hMainWnd,0,true);

      Sleep(20);//Give windows 20 msec to do its stuff
    }
  }
	return (int)msg.wParam;  //returns exit code to operating system
}

