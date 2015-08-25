#define CB_MESSAGE_RESET_TIME             60000
#define DELAY_BETWEEN_THEME_SONG_MESSAGES 8000

// Set the sound priorities, higher number = higher priority
#define PRIORITY_SIMPLE_SOUNDS   4
#define PRIORITY_CB_SOUNDS       2
#define PRIORITY_GOOD_GUY_SOUNDS 3
#define PRIORITY_BAD_GUY_SOUNDS  3
#define PRIORITY_R2D2_SOUNDS     3
#define PRIORITY_CHEWY_SOUNDS    3
#define PRIORITY_RANDOM_SOUNDS   1
#define PRIORITY_THEME_SONG      0

void processSimpleSounds(void)
{
  switch (userInput)
  {  
  case TAKE_OFF:
    playWaveFile("Take_Off.wav",PRIORITY_SIMPLE_SOUNDS,userInput);
    setVFDmessageActive(0, "    Take off");
    break;
  case BLASTER:
    playWaveFile("Blaster.wav",PRIORITY_SIMPLE_SOUNDS,userInput);
    setVFDmessageActive(0, "    Blaster");
    break;
  case EXPLODE:
    playWaveFile("EXPRL.wav",PRIORITY_SIMPLE_SOUNDS,userInput);
    setVFDmessageActive(0, "   Explosion");
    break;
  case NINE_MM:
    playWaveFile("Gun9mm.wav",PRIORITY_SIMPLE_SOUNDS,userInput);
    setVFDmessageActive(0, "      9mm");
    break;
  case STARTUP:
    playWaveFile("PRSTO.wav",PRIORITY_SIMPLE_SOUNDS,userInput);
    setVFDmessageActive(0, "    Start up");
  case MOTORS_OFF:
    SetVibratorMotorLeft(0);
    SetVibratorMotorRight(0);
  }
}

wavePlaylist CBsound[]=
{
  {"CB1_LS.wav",PRIORITY_CB_SOUNDS},
  {"CB2_HS.wav",PRIORITY_CB_SOUNDS},
  {"CB3_HS.wav",PRIORITY_CB_SOUNDS},
  {"CB4_HC.wav",PRIORITY_CB_SOUNDS},
  {"CB5_DV.wav",PRIORITY_CB_SOUNDS}
};

void runCBsequence(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(CBsound)/sizeof(CBsound[0]);

  if (userInput == CB_SEQ)
  {

    if(playWaveFile(CBsound[index].fileName,CBsound[index].playPriority,userInput))
    {
      setVFDmessageActive(0, "Incoming Message");
      waitTimer = millis();
      index ++;
      if (index >= numberOfSounds)
      {
        index = 0;
      }
    }
  }
  else
  {
    if (index !=0 && millis() - waitTimer > CB_MESSAGE_RESET_TIME)
    {
      index = 0;
      waitTimer = millis();
      Serial.println("Resetting CB sequence index to inactivity");
    }
  }
}


wavePlaylist goodGuySound[]=
{
  {"Yoda001.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda002.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda003.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda004.wav",PRIORITY_GOOD_GUY_SOUNDS},
  {"Yoda005.wav",PRIORITY_GOOD_GUY_SOUNDS}
};

void processGoodGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(goodGuySound)/sizeof(goodGuySound[0]);

  if (userInput == GOOD_GUYS)
  {  
    if(playWaveFile(goodGuySound[index].fileName,goodGuySound[index].playPriority,userInput))
    {
      setVFDmessageActive(0, "    Good Guy");
      setCockpitColor(SB_GRN);
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
  
  if(myWaveFileJustFinishedPlaying(GOOD_GUYS))
  {
    setCockpitColor(SB_DIM);
    setVFDmessageInactive(0);
  }
}


wavePlaylist badGuySound[]=
{
  {"Jaba001.wav",PRIORITY_BAD_GUY_SOUNDS},
  {"Alien01.wav",PRIORITY_BAD_GUY_SOUNDS},
  {"Alien02.wav",PRIORITY_BAD_GUY_SOUNDS},
  {"EN_TH01.wav",PRIORITY_BAD_GUY_SOUNDS},
  {"EN_TH02.wav",PRIORITY_BAD_GUY_SOUNDS}
};

void processBadGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(badGuySound)/sizeof(badGuySound[0]);

  if (userInput == BAD_GUYS)
  {  
    if(playWaveFile(badGuySound[index].fileName,badGuySound[index].playPriority,userInput))
    {
      setVFDmessageActive(0, "     Bad Guy");
      setCockpitColor(SB_RED);
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
  
  if(myWaveFileJustFinishedPlaying(BAD_GUYS))
  {
    setCockpitColor(SB_DIM);
    setVFDmessageInactive(0);
  }
}


wavePlaylist r2d2Sound[]=
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
  
  int numberOfSounds = sizeof(r2d2Sound)/sizeof(r2d2Sound[0]);

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
    if(playWaveFile(r2d2Sound[index].fileName,r2d2Sound[index].playPriority,userInput))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
      waitTimer = millis() + R2D2_LIGHT_CYCLE_TIME;
      color = 0;
    }
  }
}

wavePlaylist chewySound[]=
{
  {"Chew001.wav",PRIORITY_CHEWY_SOUNDS},
  {"Chew002.wav",PRIORITY_CHEWY_SOUNDS},
  {"Chew003.wav",PRIORITY_CHEWY_SOUNDS},
};

void processChewy(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(chewySound)/sizeof(chewySound[0]);

  if (userInput == CHEWY)
  {  
    if(playWaveFile(chewySound[index].fileName,chewySound[index].playPriority,userInput))
    {
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}

#define MIN_RANDOM_WAIT_TIME              20000  // all times are in milliseconds
#define MAX_RANDOM_WAIT_TIME              60000

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

void playStarWarsThemeSong(void)
{
  playWaveFile("SWTM001.wav",PRIORITY_THEME_SONG,0);  //start things off with the Star Wars theme song
  directDisplayVFDmessage(" May the Force");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("  be with you");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("Finn and Shamus");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
}

