#define MIN_RANDOM_WAIT_TIME 20  // time is in seconds
#define MAX_RANDOM_WAIT_TIME 60

wavePlaylist randomSoundEffect[]=
{
  {"R2D2001.wav",1},
  {"R2D2002.wav",1},
  {"R2D2003.wav",1},
  {"R2D2004.wav",1},
  {"R2D2005.wav",1},
  {"Chew001.wav",1},
  {"Chew002.wav",1},
  {"Chew003.wav",1}
};

void playRandomSounds(void)
{
  static long holdoffTime;
  static long rndTime = 2000;
  static long rndSound;
  
  int numberOfRandomSounds = sizeof(randomSoundEffect)/sizeof(randomSoundEffect[0]);
  
  if (waveFileIsPlaying())
  {   
    holdoffTime = millis();  // keep advancing the timer so it doesn't play a random sound as soon as it's done playing the higher priority sound
    return;
  }
 
  if (millis() - holdoffTime < rndTime) return;

  playWaveFile(randomSoundEffect[rndSound].fileName,randomSoundEffect[rndSound].playPriority);

  rndSound = random(0, numberOfRandomSounds);       //choose a new random sound effect
  rndTime = random(MIN_RANDOM_WAIT_TIME * 1000, MAX_RANDOM_WAIT_TIME * 1000);  //choose a new random time between plays (in mSec)
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
}

