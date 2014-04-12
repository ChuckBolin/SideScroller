//CSound.h
//http://www.fmod.org/docs/HTML/
//tutorials http://www.robthebloke.org/
//link to fmod.lib
//add fmod.dll to current folder
#ifndef CSOUND_H
#define CSOUND_H
#include "fmod.h"
#include "fmod_errors.h"
#include <windows.h>
#include <string>
#include <sstream>
#include "..\includes\CTimer.h"

const int SOUND_SQUISH = 0;
const int SOUND_CRUNCH = 1;
const int SOUND_BOUNCE2 = 2;
const int SOUND_SPLASH = 3;
const int SOUND_MAT_MANIA = 4;
const int SOUND_PENGO = 5;
const int SOUND_TRACK1 = 6;
const int SOUND_TRACK2 = 7;
const int SOUND_TRACK3 = 8;
const int SOUND_TRACK4 = 9;
const int SOUND_SKUNK = 10;
const int SOUND_BATTERY = 11;
const int SOUND_GOLD = 12;
const int SOUND_SILVER = 13;
const int SOUND_LEVEL = 14;
const int SOUND_NOTIME = 15;
const int SOUND_SCREAM = 16;
const int SOUND_SEGWAY = 17;
const int SOUND_DIE = 18;


class CSound{
public:
  CSound();
  ~CSound();
  bool isValid();
  int getLength(int track);
  int getLengthMS(int track);
  void play(int track);
  void stop(int track);
  int getPosition(int track);
  void setPosition(int track, int pos);
  int getTime(int track);
  void setTime(int track, int pos);
  int getVolume(int track);
  void setVolume(int track, int volume);
  void stopAll();
  void update();
  void resetCountDown();

private:
  std::string getCurrDir();
  bool m_bValid;
  FSOUND_STREAM* m_pStream[19];
  std::string m_Filename[19];
  int m_maxSounds;
  int m_activeTrack;
  CTimer m_Timer;
  int m_countDown;
};

#endif