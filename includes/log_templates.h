/*************************************************************************
 TEMPLATES.H - Written by Chuck Bolin
 Purpose: Contains various useful functions such as printing text and 
 numbers to the screen, logging data to a file, etc.
*************************************************************************/
#ifndef TEMPLATES_H
#define TEMPLATES_H

//include files
#include <sstream>
#include <string>
#include <fstream>
#include <windows.h>

//fileName for logfile
std::string fileName = "log.txt";

//Logs a variable value (any type) to logfile.txt
template<typename T>
void Log(T value1){
	std::ostringstream oss;
	std::ofstream file;
  
	file.open( fileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
		oss << value1 << std::endl;
    file <<  oss.str();
	} 
	file.close();
}

//Logs two variable values (any type) to logfile.txt
template<typename T1,typename T2 >
void Log(T1 value1, T2 value2){
	std::ostringstream oss;
	std::ofstream file;
  
	file.open( fileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
    oss << value1 << " : " << value2 << std::endl;
    file <<  oss.str();
	} 
	file.close();
}

//converts a data type to a string
template<typename T>
std::string convertToString(T value1){
	std::ostringstream oss;

  oss.str(""); 
  oss << value1;
  return oss.str();
}

//Prints text or numbers to the screen at point x,y
//Requires Device Context...used inside WM_PAINT
template<typename T>
void PrintText(HDC dc, T value, int x, int y)
{
  
  std::ostringstream os;
  std::string s;
  os.str("");
  os << value;
  s = os.str();
  ::TextOut(dc, x, y, s.c_str(), s.size() );
}

template<typename T1, typename T2>
void PrintText(HDC dc, T1 value1, T2 value2, int x, int y)
{
  
  std::ostringstream os;
  std::string s;
  os.str("");
  os << value1 << ": " << value2;
  s = os.str();
  ::TextOut(dc, x, y, s.c_str(), s.size() );
}

#endif
