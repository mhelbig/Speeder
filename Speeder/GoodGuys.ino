wavePlaylist goodGuySound[]=
{
  {"Yoda001.wav",4},
  {"Yoda002.wav",4},
  {"Yoda003.wav",4},
  {"Yoda004.wav",4},
  {"Yoda005.wav",4}
};

void processGoodGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(goodGuySound)/sizeof(goodGuySound[0]);

  if(millis() - waitTimer < 25000) return;
  waitTimer = millis();
  
  playWaveFile(goodGuySound[index].fileName,goodGuySound[index].playPriority);

  index ++;
  if (index > numberOfSounds) index = 0;
}
