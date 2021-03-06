/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Simple Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void processSimpleSounds(void)
{
  if(myWaveFileJustFinishedPlaying(SIMPLE_SND))
  {
    setVFDmessageInactive(PRIORITY_SIMPLE_SOUNDS);
  }

  switch (userInput)
  {  
  case SIMPLE_SOUND_1:
    if(playWaveFile("Blaster.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Blaster");
    break;
  case SIMPLE_SOUND_2:
    if(playWaveFile("LASER2.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "   Laser Gun");
    break;
  case SIMPLE_SOUND_3:
    if(playWaveFile("Gun9mm.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "      9mm");
    break;
  case SIMPLE_SOUND_4:
    if(playWaveFile("EXPRL.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Explosion");
    break;
  case SIMPLE_SOUND_5:
    if(playWaveFile("MGUNFST.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "  Machine Gun");
    break;
  case SIMPLE_SOUND_6:
    if(playWaveFile("GATLSR.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "  Gatlin Laser");
    break;
  case SIMPLE_SOUND_7:
    if(playWaveFile("SSPB01.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Thruster");
    break;
  case SIMPLE_SOUND_8:
    if(playWaveFile("SSPB02.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Thruster");
    break;
  case SIMPLE_SOUND_9:
    if(playWaveFile("SSPB04.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Thruster");
    break;
  case SIMPLE_SOUND_10:
    if(playWaveFile("LSSRB02.wav",PRIORITY_SIMPLE_SOUNDS,SIMPLE_SND)) setVFDmessageActive(PRIORITY_SIMPLE_SOUNDS, "    Thruster");
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Good Guy Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
wavePlaylist goodGuySound[]=
{
  {"CB1_LS.wav" ,PRIORITY_GOOD_GUY_SOUNDS},
  {"Chew001.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Chew002.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"OBIWAN1.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"OBIWAN2.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"RedLDR.wav" ,PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda001.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda002.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda003.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda004.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda005.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda006.wav",PRIORITY_GOOD_GUY_SOUNDS}
};

void processGoodGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(goodGuySound)/sizeof(goodGuySound[0]);

  if(myWaveFileJustFinishedPlaying(GOOD_GUYS_BUTTON))
  {
    setCockpitColor(SB_DIM);
    setVFDmessageInactive(PRIORITY_GOOD_GUY_SOUNDS);
  }
  if (userInput == GOOD_GUYS_BUTTON)
  {  
    if(playWaveFile(goodGuySound[index].fileName,goodGuySound[index].playPriority,userInput))
    {
      setVFDmessageActive(PRIORITY_GOOD_GUY_SOUNDS, "    Good Guy");
      setCockpitColor(SB_GRN);
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Bad Guy Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
wavePlaylist badGuySound[]=
{
  {"Alien01.wav" ,PRIORITY_BAD_GUY_SOUNDS},
  {"Alien02.wav" ,PRIORITY_BAD_GUY_SOUNDS},
  {"CB5_DV.wav"  ,PRIORITY_BAD_GUY_SOUNDS},
  {"COUNTD.wav"  ,PRIORITY_BAD_GUY_SOUNDS},
  {"EMPEROR.wav" ,PRIORITY_BAD_GUY_SOUNDS},
  {"Jaba001.wav" ,PRIORITY_BAD_GUY_SOUNDS},
  {"Vader001.wav",PRIORITY_BAD_GUY_SOUNDS}
};

void processBadGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(badGuySound)/sizeof(badGuySound[0]);

  if (userInput == BAD_GUYS_BUTTON)
  {  
    if(playWaveFile(badGuySound[index].fileName,badGuySound[index].playPriority,userInput))
    {
      setVFDmessageActive(PRIORITY_BAD_GUY_SOUNDS, "     Bad Guy");
      setCockpitColor(SB_RED);
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
  
  if(myWaveFileJustFinishedPlaying(BAD_GUYS_BUTTON))
  {
    setCockpitColor(SB_DIM);
    setVFDmessageInactive(PRIORITY_BAD_GUY_SOUNDS);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//R2D2 Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
wavePlaylist R2D2Sound[]=
{
  {"R2D2001.wav",PRIORITY_R2D2_SOUNDS},
  {"R2D2002.wav",PRIORITY_R2D2_SOUNDS},
  {"R2D2003.wav",PRIORITY_R2D2_SOUNDS},
  {"R2D2004.wav",PRIORITY_R2D2_SOUNDS},
  {"R2D2005.wav",PRIORITY_R2D2_SOUNDS}
};

#define R2D2_LIGHT_CYCLE_TIME 250

void processR2D2(void)
{
  static long waitTimer;
  static char index = 0;
  static char color = 0;
  
  int numberOfSounds = sizeof(R2D2Sound)/sizeof(R2D2Sound[0]);

  if(myWaveFileJustFinishedPlaying(R2D2))
  {
    setR2D2Color(SB_OFF);
    return;
  }
  else if(myWaveFileIsPlaying(R2D2))
  {
    if (millis() < waitTimer) return;
    
    waitTimer = millis() + R2D2_LIGHT_CYCLE_TIME;
    if (color == 0)
    {
      setR2D2Color(SB_RED);
      color = 1;
    }
    else if(color == 1)
    {
      setR2D2Color(SB_GRN);
      color = 2;
    }
    else if(color == 2)
    {
      setR2D2Color(SB_BLU);
      color = 0;
    }
  }
  else if (userInput == R2D2)
  {  
    if(playWaveFile(R2D2Sound[index].fileName,R2D2Sound[index].playPriority,userInput))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
      waitTimer = millis() + R2D2_LIGHT_CYCLE_TIME;
      color = 0;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Random Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIN_RANDOM_WAIT_TIME              60000  // all times are in milliseconds
#define MAX_RANDOM_WAIT_TIME              180000

wavePlaylist randomSoundEffect[]=
{
  {"LSSPB01.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSPB02.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSPB03.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB01.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB02.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB03.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB04.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB05.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB06.wav",PRIORITY_RANDOM_SOUNDS},
  {"LSSRB07.wav",PRIORITY_RANDOM_SOUNDS},
  {"SATPB01.wav",PRIORITY_RANDOM_SOUNDS},
  {"SATPB02.wav",PRIORITY_RANDOM_SOUNDS},
  {"SATPB03.wav",PRIORITY_RANDOM_SOUNDS},
  {"SATPB04.wav",PRIORITY_RANDOM_SOUNDS},
  {"SFFB01.wav",PRIORITY_RANDOM_SOUNDS},
  {"SOFB.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB01.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB02.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB03.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB04.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB05.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB06.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB07.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSPB08.wav",PRIORITY_RANDOM_SOUNDS},
  {"SSTO01.wav",PRIORITY_RANDOM_SOUNDS},
  {"TACFA.wav",PRIORITY_RANDOM_SOUNDS},
  {"EXPRL.wav",PRIORITY_RANDOM_SOUNDS},
  {"PR01.wav",PRIORITY_RANDOM_SOUNDS},
  {"PR02.wav",PRIORITY_RANDOM_SOUNDS},
  {"PR03.wav",PRIORITY_RANDOM_SOUNDS}
};

void playRandomSounds(void)
{
  static long holdoffTime;
  static long rndTime;
  static long rndSound;
  
  int numberOfRandomSounds = sizeof(randomSoundEffect)/sizeof(randomSoundEffect[0]);
  
  if (waveFileIsPlaying())
  {   
    holdoffTime = millis();  // keep advancing the timer so it doesn't play a random sound as soon as it's done playing the higher priority sound
    return;
  }
 
  if (millis() - holdoffTime < rndTime) return;

  playWaveFile(randomSoundEffect[rndSound].fileName,randomSoundEffect[rndSound].playPriority,0);

  rndSound = random(numberOfRandomSounds);       //choose a new random sound effect
  rndTime = random(MIN_RANDOM_WAIT_TIME, MAX_RANDOM_WAIT_TIME);  //choose a new random time between plays (in mSec)
  Serial.print("Next random in "); Serial.print(rndTime); Serial.println("mS");
}

void initializeRandomGenerator(void)
{
  randomSeed(analogRead(0));
  Serial.println("RND gen init.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Theme song and welcome messages
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DELAY_BETWEEN_THEME_SONG_MESSAGES 8000

void playStarWarsThemeSong(void)
{
  playWaveFile("SWTM001.wav",PRIORITY_THEME_SONG,0);  //start things off with the Star Wars theme song
  directDisplayVFDmessage(" May the Force");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("  be with you");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("Finn and Seamus");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
}

