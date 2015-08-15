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

wavePlaylist r2d2Sound[]=
{
  {"R2D2001.wav",3},
  {"R2D2002.wav",3},
  {"R2D2003.wav",3},
  {"R2D2004.wav",3},
  {"R2D2005.wav",3}
};

wavePlaylist chewySound[]=
{
  {"Chew001.wav",3},
  {"Chew002.wav",3},
  {"Chew003.wav",3},
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
      setVFDmessageActive(0, "    Good Guy");
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
      setVFDmessageActive(0, "     Bad Guy");
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

void processR2D2(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(r2d2Sound)/sizeof(r2d2Sound[0]);

  if (userInput == 'R')
  {  
    if(playWaveFile(r2d2Sound[index].fileName,r2d2Sound[index].playPriority))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

void processChewy(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(chewySound)/sizeof(chewySound[0]);

  if (userInput == 'W')
  {  
    if(playWaveFile(chewySound[index].fileName,chewySound[index].playPriority))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

