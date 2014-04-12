//CSound.cpp
#include "..\includes\CSound.h"


CSound::CSound(){
  m_maxSounds = 18;
  m_Filename[SOUND_SQUISH]= getCurrDir() + "\\sounds\\squish.wav";
  m_Filename[SOUND_CRUNCH]= getCurrDir() + "\\sounds\\crunch.mp3";
  m_Filename[SOUND_BOUNCE2]= getCurrDir() + "\\sounds\\boing_1.wav";
  m_Filename[SOUND_SPLASH]= getCurrDir() + "\\sounds\\bubbles.mp3";
  m_Filename[SOUND_MAT_MANIA]= getCurrDir() + "\\sounds\\Mat_Mania.mid";
  m_Filename[SOUND_PENGO]= getCurrDir() + "\\sounds\\pengo1.mid";
  m_Filename[SOUND_TRACK1]= getCurrDir() + "\\sounds\\track1.mid";
  m_Filename[SOUND_TRACK2]= getCurrDir() + "\\sounds\\track2.mid";
  m_Filename[SOUND_TRACK3]= getCurrDir() + "\\sounds\\track3.mid";
  m_Filename[SOUND_TRACK4]= getCurrDir() + "\\sounds\\track4.mid";
  m_Filename[SOUND_SKUNK]= getCurrDir() + "\\sounds\\skunk.wav";
  m_Filename[SOUND_BATTERY]= getCurrDir() + "\\sounds\\battery.mp3";
  m_Filename[SOUND_GOLD]= getCurrDir() + "\\sounds\\gold.wav";
  m_Filename[SOUND_SILVER]= getCurrDir() + "\\sounds\\silver.wav";
  m_Filename[SOUND_LEVEL]= getCurrDir() + "\\sounds\\level.wav";
  m_Filename[SOUND_NOTIME]= getCurrDir() + "\\sounds\\notime.mp3";
  m_Filename[SOUND_SCREAM]= getCurrDir() + "\\sounds\\scream2.mp3";
  m_Filename[SOUND_SEGWAY]= getCurrDir() + "\\sounds\\segway.mp3";
  m_Filename[SOUND_DIE]= getCurrDir() + "\\sounds\\skunk_die.mp3";



	if( FSOUND_Init(44000,64,0) == false )
    m_bValid = false;
  else
    m_bValid = true;
	
  m_pStream[SOUND_SQUISH] = FSOUND_Stream_Open( m_Filename[SOUND_SQUISH].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_CRUNCH] = FSOUND_Stream_Open( m_Filename[SOUND_CRUNCH].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_BOUNCE2] = FSOUND_Stream_Open( m_Filename[SOUND_BOUNCE2].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_SPLASH] = FSOUND_Stream_Open( m_Filename[SOUND_SPLASH].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_MAT_MANIA] = FSOUND_Stream_Open( m_Filename[SOUND_MAT_MANIA].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_PENGO] = FSOUND_Stream_Open( m_Filename[SOUND_PENGO].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_TRACK1] = FSOUND_Stream_Open( m_Filename[SOUND_TRACK1].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_TRACK2] = FSOUND_Stream_Open( m_Filename[SOUND_TRACK2].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_TRACK3] = FSOUND_Stream_Open( m_Filename[SOUND_TRACK3].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_TRACK4] = FSOUND_Stream_Open( m_Filename[SOUND_TRACK4].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_SKUNK] = FSOUND_Stream_Open( m_Filename[SOUND_SKUNK].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_BATTERY] = FSOUND_Stream_Open( m_Filename[SOUND_BATTERY].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_GOLD] = FSOUND_Stream_Open( m_Filename[SOUND_GOLD].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_SILVER] = FSOUND_Stream_Open( m_Filename[SOUND_SILVER].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_LEVEL] = FSOUND_Stream_Open( m_Filename[SOUND_LEVEL].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_NOTIME] = FSOUND_Stream_Open( m_Filename[SOUND_NOTIME].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_SCREAM] = FSOUND_Stream_Open( m_Filename[SOUND_SCREAM].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_SEGWAY] = FSOUND_Stream_Open( m_Filename[SOUND_SEGWAY].c_str() , FSOUND_2D , 0 , 0 );
  m_pStream[SOUND_DIE] = FSOUND_Stream_Open( m_Filename[SOUND_DIE].c_str() , FSOUND_2D , 0 , 0 );

  m_activeTrack = 5;
  m_countDown = 10;
  m_Timer.initialize();
  
}

CSound::~CSound(){
  for(int i = 0; i < m_maxSounds; i++){
	  FSOUND_Stream_Close(m_pStream[i]);
  }
	FSOUND_Close();
}

void CSound::stopAll(){
  for(int i = 0; i < m_maxSounds; i++)
    stop(i);
}

void CSound::resetCountDown(){
  m_countDown = 0;
}

void CSound::update(){
  if(m_Timer.secondTimer() == true){
    m_countDown--;

    if(m_countDown < 1){
      stop(m_activeTrack);
      m_activeTrack++;
      if(m_activeTrack > 9)
        m_activeTrack = 4;

      play(m_activeTrack);
      m_countDown = 60;
      /*
      switch(m_activeTrack){
        case 4:
          m_countDown = 10;//188;
          break;
        case 5:
          m_countDown = 10;//66;
          break;
        case 6:
          m_countDown = 10;//166;
          break;
        case 7:
          m_countDown = 10;//201;
          break;
        case 8:
          m_countDown = 10;//190;
          break;
        case 9:
          m_countDown = 10;//181;
          break;
      }
      */
    }
  }
}

int CSound::getPosition(int track){
  if(track < 0 || track > m_maxSounds)
    return 0;
 
  return FSOUND_Stream_GetPosition(m_pStream[track]);
}

void CSound::setPosition(int track, int pos){
  if(track < 0 || track > m_maxSounds)
    return;
  if(pos < getLength(track))
    FSOUND_Stream_SetPosition(m_pStream[track], pos);
}

int CSound::getTime(int track){
  if(track < 0 || track > m_maxSounds)
    return 0;
  return FSOUND_Stream_GetTime(m_pStream[track]);
}

void CSound::setTime(int track, int pos){
  if(track < 0 || track > m_maxSounds)
    return;

  FSOUND_Stream_SetTime(m_pStream[track], pos);
}

int CSound::getLength(int track){
  if(track < 0 || track > m_maxSounds)
    return 0;

  return FSOUND_Stream_GetLength(m_pStream[track]);
}

int CSound::getLengthMS(int track){
  if(track < 0 || track > m_maxSounds)
    return 0;

  return FSOUND_Stream_GetLengthMs(m_pStream[track]);
}

void CSound::play(int track){
  if(track < 0 || track > m_maxSounds)
    return;

  FSOUND_Stream_Play(track, m_pStream[track]);
}

void CSound::stop(int track){
  if(track < 0 || track > m_maxSounds)
    return;

	FSOUND_Stream_Stop(m_pStream[track]);
}

int CSound::getVolume(int track){
  if(track < 0 || track > m_maxSounds)
    return 0;
  return 0;//reinterpret_cast<int>FSOUND_GetVolume(m_pStream[track]);
}

void CSound::setVolume(int track, int volume){
  if(track < 0 || track > m_maxSounds)
    return;
  if(volume < 0)
    volume = 0;
  if(volume > 255)
    volume = 255;
  FSOUND_SetVolumeAbsolute(track, volume);
}


bool CSound::isValid(){
  return m_bValid;
}

std::string CSound::getCurrDir(){
  TCHAR Buffer[MAX_PATH];
  DWORD dwRet;
  dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
  std::ostringstream oss;
  oss << Buffer;
  std::string s = oss.str();
  return s;
}
