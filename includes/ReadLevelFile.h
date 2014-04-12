//CFile.h
#ifndef READ_FILE_H
#define READ_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "utility.h"

struct ENTITY_DATA{
  int type;
  int x;
  int y;
};

class CFile{
public:
  CFile();
  ~CFile();
  void loadLevel(std::string sFilename);
  
  int getType(int id);
  int getPosX(int id);
  int getPosY(int id);
  int size();
  bool isValid();
  void clearData();

private:
  void parseFile();
  std::string mFilename;
  std::vector<ENTITY_DATA> m_data;
  std::string m_filename;
  bool m_validFile;
};

#endif //READ_FILE_H