wavePlaylist goodGuySound[]=
{
  {"Yoda001.wav",3},
  {"Yoda002.wav",3},
  {"Yoda003.wav",3},
  {"Yoda004.wav",3},
  {"Yoda005.wav",3}
};

wavePlaylist badGuySound[]=
{
  {"Jaba001.wav",3},
  {"Alien01.wav",3},
  {"Alien02.wav",3},
  {"EN_TH01.wav",3},
  {"EN_TH02.wav",3}
};


void processGoodGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(goodGuySound)/sizeof(goodGuySound[0]);

  if (userInput == 'G')
  {  
    if(playWaveFile(goodGuySound[index].fileName,goodGuySound[index].playPriority))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

void processBadGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(badGuySound)/sizeof(badGuySound[0]);

  if (userInput == 'B')
  {  
    if(playWaveFile(badGuySound[index].fileName,badGuySound[index].playPriority))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

