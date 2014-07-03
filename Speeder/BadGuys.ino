/*wavePlaylist badGuySound[]=
{
  {"Jaba001.wav",4},
  {"Alien01.wav",4},
  {"Alien02.wav",4},
  {"EN_TH01.wav",4},
  {"EN_TH02.wav",4}
};

void processBadGuys(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(badGuySound)/sizeof(badGuySound[0]);

  if (userInput == 'B')
  {  
    playWaveFile(badGuySound[index].fileName,badGuySound[index].playPriority);
    index ++;
    if (index >= numberOfSounds) index = 0;
  }
}
*/
