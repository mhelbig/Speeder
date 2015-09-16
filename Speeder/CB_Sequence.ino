/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CB Message Sequence Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CB_MESSAGE_RESET_TIME             60000
#define CB_MIC_LED                        8

wavePlaylist CBsound[]=
{
  {"CB1_LS.wav",PRIORITY_CB_SOUNDS},
  {"CB2_HS.wav",PRIORITY_CB_SOUNDS},
  {"CB3_HS.wav",PRIORITY_CB_SOUNDS},
  {"CB4_HC.wav",PRIORITY_CB_SOUNDS},
  {"RedLDR.wav",PRIORITY_CB_SOUNDS},
  {"CB5_DV.wav",PRIORITY_CB_SOUNDS},
  {"WEML001.wav",PRIORITY_CB_SOUNDS},
  {"Yoda001.wav",PRIORITY_CB_SOUNDS}
};

void runCBsequence(void)
{
  static long waitTimer;
  static char index = 0;
  
  int numberOfSounds = sizeof(CBsound)/sizeof(CBsound[0]);

  if(myWaveFileJustFinishedPlaying(CB_MIC_PUSHBUTTON))
  {
    setVFDmessageInactive(PRIORITY_CB_SOUNDS);
    setCB_LED(0);
    
    index ++;      // increment the message index and wrap it around when we reach the end
    if (index >= numberOfSounds)
    {
      index = 0;
    }
  }
    
  if (userInput == CB_MIC_PUSHBUTTON)
  {

    if(playWaveFile(CBsound[index].fileName,CBsound[index].playPriority,userInput))
    {
      setVFDmessageActive(PRIORITY_CB_SOUNDS, "Incoming Message");
      setCB_LED(1);
      waitTimer = millis();
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

void setCB_LED(bool state)
{
  digitalWrite(CB_MIC_LED, state);
}

void initializeCB(void)
{
  pinMode(CB_MIC_LED, OUTPUT);
  digitalWrite(CB_MIC_LED, LOW);
}
    

