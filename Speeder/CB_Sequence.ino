/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CB Message Sequence Sounds
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CB_MESSAGE_RESET_TIME             60000
#define CB_MIC_LED                        8
#define CB_LED_BLINK_PERIOD               250 // time in milliseconds

unsigned char CB_LED_State = 0;

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
      setCB_LED(2);
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

void processCB_LED(void)
{
  static long CB_LED_BlinkTimer;
  static bool CB_LED_BlinkState = 0;
  
  if (millis() > CB_LED_BlinkTimer)
  {
    CB_LED_BlinkTimer = millis() + CB_LED_BLINK_PERIOD;
    CB_LED_BlinkState = !CB_LED_BlinkState;
  }
  
  if(CB_LED_State == 2)
  {
    digitalWrite(CB_MIC_LED, CB_LED_BlinkState);
  }  

  if(CB_LED_State == 0)
  {
    digitalWrite(CB_MIC_LED, 0);
  }
  
  if(CB_LED_State == 1)
  {
    digitalWrite(CB_MIC_LED, 1);
  }
  
}  

void setCB_LED(char state)
{
  CB_LED_State = state;
}

void initializeCB(void)
{
  pinMode(CB_MIC_LED, OUTPUT);
  digitalWrite(CB_MIC_LED, LOW);
}
    

