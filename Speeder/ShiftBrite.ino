#define SBclockpin  32  // CI
#define SBenablepin 34  // EI
#define SBlatchpin  33  // LI
#define SBdatapin   31  // DI

// define the shiftbrites as they are located in the serial chain:
#define SB_COCKPIT       0
#define SB_LASERCANNON   1
#define SB_R2D2          2
#define SB_THRUSTERS     3

#define NumLEDs 4

unsigned long SB_CommandPacket;
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;
boolean SB_changed = 1;     //flag to indicate when we need to update the shiftbrites

void initializeShiftBrite(void)
{
  pinMode(SBdatapin, OUTPUT);
  pinMode(SBlatchpin, OUTPUT);
  pinMode(SBenablepin, OUTPUT);
  pinMode(SBclockpin, OUTPUT);
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
  digitalWrite(SBlatchpin, LOW);
  digitalWrite(SBenablepin, LOW);
}

//put functions here

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Cockpit color functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setCockpitColorRed(void)
{
  LEDChannels[SB_COCKPIT][0]=1023;
  LEDChannels[SB_COCKPIT][1]=0;
  LEDChannels[SB_COCKPIT][2]=0;
  SB_changed;
}

void setCockpitColorGreen(void)
{
  LEDChannels[SB_COCKPIT][0]=0;
  LEDChannels[SB_COCKPIT][1]=1023;
  LEDChannels[SB_COCKPIT][2]=0;
  SB_changed = 1;
}

void setCockpitColorWhite(void)
{
  LEDChannels[SB_COCKPIT][0]=1023;
  LEDChannels[SB_COCKPIT][1]=1023;
  LEDChannels[SB_COCKPIT][2]=1023;
  SB_changed = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Thruster color functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setThrusterColor(int thrusterMode)
{
  switch (thrusterMode)
  {
    case 0:  // hyperdrive off
      LEDChannels[SB_COCKPIT][0]=0;
      LEDChannels[SB_COCKPIT][1]=0;
      LEDChannels[SB_COCKPIT][2]=0;
    
    case 1:  // WHITE - lightspeed
      LEDChannels[SB_COCKPIT][0]=1023;
      LEDChannels[SB_COCKPIT][1]=1023;
      LEDChannels[SB_COCKPIT][2]=1023;
  
    case 2:  // BLUE - ridiculous speed
      LEDChannels[SB_COCKPIT][0]=0;
      LEDChannels[SB_COCKPIT][1]=0;
      LEDChannels[SB_COCKPIT][2]=1023;
    
    case 3:  // YELLOW - ludicrous speed
      LEDChannels[SB_COCKPIT][0]=1023;
      LEDChannels[SB_COCKPIT][1]=1023;
      LEDChannels[SB_COCKPIT][2]=0;
    
    case 4:  // ORANGE - hyperdrive hot
      LEDChannels[SB_COCKPIT][0]=1023;
      LEDChannels[SB_COCKPIT][1]=255;
      LEDChannels[SB_COCKPIT][2]=0;
    
    case 5:  // RED - hyperdrive overheated, needing repair
      LEDChannels[SB_COCKPIT][0]=1023;
      LEDChannels[SB_COCKPIT][1]=0;
      LEDChannels[SB_COCKPIT][2]=0;
  }
  SB_changed = 1;
}

void processShiftBrite()
{
  if(SB_changed == 1)
  {
    SB_changed = 0;
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0;h<NumLEDs;h++)
    {
  	  SB_RedCommand = LEDChannels[h][0];
  	  SB_GreenCommand = LEDChannels[h][1];
  	  SB_BlueCommand = LEDChannels[h][2];
  	  SB_SendPacket();
    }
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,LOW);
   
    SB_CommandMode = B01; // Write to current control registers
    for (int z = 0; z < NumLEDs; z++) SB_SendPacket();
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,LOW);
    Serial.println("Updating ShiftBrites");
  }
}

void SB_SendPacket() 
{
   SB_CommandPacket = SB_CommandMode & B11;
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_BlueCommand & 1023);
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_RedCommand & 1023);
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_GreenCommand & 1023);

   shiftOut(SBdatapin, SBclockpin, MSBFIRST, SB_CommandPacket >> 24);
   shiftOut(SBdatapin, SBclockpin, MSBFIRST, SB_CommandPacket >> 16);
   shiftOut(SBdatapin, SBclockpin, MSBFIRST, SB_CommandPacket >> 8);
   shiftOut(SBdatapin, SBclockpin, MSBFIRST, SB_CommandPacket);

   delay(1); // adjustment may be necessary depending on chain length
   digitalWrite(SBlatchpin,HIGH); // latch data into registers
   delay(1); // adjustment may be necessary depending on chain length
   digitalWrite(SBlatchpin,LOW);
}


