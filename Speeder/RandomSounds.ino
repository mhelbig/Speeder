#define MIN_RANDOM_WAIT_TIME 2000  // time is in milliseconds
#define MAX_RANDOM_WAIT_TIME 5000

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

#define DELAY_BETWEEN_THEME_SONG_MESSAGES 8000

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

