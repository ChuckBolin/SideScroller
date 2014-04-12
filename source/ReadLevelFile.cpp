//CFile.cpp
#include "..\includes\ReadLevelFile.h"
#include <iostream>
using std::cout;
using std::endl;

CFile::CFile(){
}

CFile::~CFile(){
}

void CFile::loadLevel(std::string sFilename){
  m_filename = GetCurrDir() + "\\" + sFilename;
  m_validFile = false;
  parseFile();
}

int CFile::getType(int id){
  for(int i = 0; i < m_data.size(); i++){
    if(i == id)
      return m_data[i].type;
  }
  return 0;
}

int CFile::getPosX(int id){
  for(int i = 0; i < m_data.size();i++){
    if(i == id)
      return m_data[i].x;
  }
  return 0;
}

int CFile::getPosY(int id){
  for(int i = 0; i < m_data.size();i++){
    if(i == id)
      return m_data[i].y ;
  }
  return 0;
}

bool CFile::isValid(){
  return m_validFile;
}

int CFile::size(){
  return m_data.size();
}

void CFile::clearData(){
  m_data.clear();
}

void CFile::parseFile(){
  std::string line;
  std::ostringstream oss;
  std::ifstream file(m_filename.c_str());
  ENTITY_DATA temp;
  int commaCount = 0;
  //bool comment = false;
  //int commentCount = 0;
  bool ignore = false;

  if(!file){
    return;    
  }

  char ch;
  std::string data[3];
  int count = -1;
  oss.str("");
  data[0] = "";
  data[1] = "";
  data[2] = "";

  while(file.get(ch)){
    if(ch == '\n'){
      if(ignore == false){
      if(commaCount < 2 || commaCount > 2)
        break;
      commaCount = 0;
      count++;
      data[count] = oss.str();
      oss.str("");
      commaCount = 0;
      }
      ignore = false;
    }
 
    else if(ch == ','){
      if(ignore == false){
        count++;
        data[count] = oss.str();
        oss.str("");
        commaCount++;
      }
    }
    else if(ch == '/')
      ignore = true;
 
    else{
      if(ignore == false)
        oss << ch;
    }

    if(count == 2){
      //cout << data[0] << ":" << data[1] << ":" << data[2] << endl;
      temp.type = atoi(data[0].c_str());
      temp.x = atoi(data[1].c_str());
      temp.y = atoi(data[2].c_str());
      m_data.push_back(temp);
      count = -1;
    }
  }
  file.close();  

  if(m_data.size() > 0)
    m_validFile = true;
}

