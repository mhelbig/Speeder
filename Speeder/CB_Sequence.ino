wavePlaylist CBsound[]=
{
  {"CB1_LS.wav",5},
  {"CB2_HS.wav",5},
  {"CB3_HS.wav",5},
  {"CB4_HC.wav",5},
  {"CB5_DV.wav",5}
};

void runCBsequence(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(CBsound)/sizeof(CBsound[0]);

  if (userInput == 'C')
  {
    playWaveFile(CBsound[index].fileName,CBsound[index].playPriority);
    index ++;
    if (index >= numberOfSounds) index = 0;
  }
}
