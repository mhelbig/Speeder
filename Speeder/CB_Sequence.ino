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
      index ++;
      if (index >= numberOfSounds) index = 0;
    }
  }
}
