#define MIN_RANDOM_WAIT_TIME              10000  // all times are in milliseconds
#define MAX_RANDOM_WAIT_TIME              20000
#define CB_MESSAGE_RESET_TIME             60000
#define DELAY_BETWEEN_THEME_SONG_MESSAGES 8000

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

void processActionSounds(void)
{
  switch (userInput)
  {  
  case 'T':
    playWaveFile("Take_Off.wav",3);
    setVFDmessageActive(0, "    Take off");
    break;
  case 'L':    
    playWaveFile("Laser.wav",4);
    setVFDmessageActive(0, "  Laser Cannon");
    SetVibratorMotorLeft(127);
    break;
  case 'P':
    playWaveFile("Blaster.wav",4);
    setVFDmessageActive(0, "    Blaster");
    SetVibratorMotorRight(127);
    break;
  case 'E':
    playWaveFile("EXPRL.wav",4);
    setVFDmessageActive(0, "   Explosion");
    break;
  case 'N':
    playWaveFile("Gun9mm.wav",4);
    setVFDmessageActive(0, "      9mm");
    break;
  case 'S':
    playWaveFile("PRSTO.wav",4);
    setVFDmessageActive(0, "    Start up");
  case 'M':
    SetVibratorMotorLeft(0);
    SetVibratorMotorRight(0);
  }
}

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

wavePlaylist CBsound[]=
{
  {"CB1_LS.wav",2},
  {"CB2_HS.wav",2},
  {"CB3_HS.wav",2},
  {"CB4_HC.wav",2},
  {"CB5_DV.wav",2}
};

void runCBsequence(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(CBsound)/sizeof(CBsound[0]);

  if (userInput == 'C')
  {

    if(playWaveFile(CBsound[index].fileName,CBsound[index].playPriority))
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

wavePlaylist randomSoundEffect[]=
{
  {"LSSPB01.wav",1},
  {"LSSPB02.wav",1},
  {"LSSPB03.wav",1},
  {"LSSRB01.wav",1},
  {"LSSRB02.wav",1},
  {"LSSRB03.wav",1},
  {"LSSRB04.wav",1},
  {"LSSRB05.wav",1},
  {"LSSRB06.wav",1},
  {"LSSRB07.wav",1},
  {"SATPB01.wav",1},
  {"SATPB02.wav",1},
  {"SATPB03.wav",1},
  {"SATPB04.wav",1},
  {"SFFB01.wav",1},
  {"SOFB.wav",1},
  {"SSPB01.wav",1},
  {"SSPB02.wav",1},
  {"SSPB03.wav",1},
  {"SSPB04.wav",1},
  {"SSPB05.wav",1},
  {"SSPB06.wav",1},
  {"SSPB07.wav",1},
  {"SSPB08.wav",1},
  {"SSTO01.wav",1},
  {"TACFA.wav",1},
  {"EXPRL.wav",1},
  {"PR01.wav",1},
  {"PR02.wav",1},
  {"PR03.wav",1}
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

  playWaveFile(randomSoundEffect[rndSound].fileName,randomSoundEffect[rndSound].playPriority);

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
  playWaveFile("SWTM001.wav",0);  //start things off with the Star Wars theme song
  directDisplayVFDmessage(" May the Force");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("  be with you");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
  
  directDisplayVFDmessage("Finn and Shamus");
  delay(DELAY_BETWEEN_THEME_SONG_MESSAGES);
}

