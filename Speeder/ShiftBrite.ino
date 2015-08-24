#define SBclockpin  32  // CI
#define SBenablepin 34  // EI
#define SBlatchpin  33  // LI
#define SBdatapin   31  // DI

#define NUM_SHIFTBRITES 3

// define the shiftbrites in the serial chain, last device first
#define SB_LASERCANNON   3
#define SB_THRUSTERS     1
#define SB_COCKPIT       2
#define SB_R2D2          0

unsigned long SB_CommandPacket;
int LEDChannels[NUM_SHIFTBRITES][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;
boolean SB_changed = 1;     //flag to indicate when we need to update the shiftbrites

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Cockpit color functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setCockpitColor(int r, int g, int b)
{
  LEDChannels[SB_COCKPIT][0]=r;
  LEDChannels[SB_COCKPIT][1]=g;
  LEDChannels[SB_COCKPIT][2]=b;
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
      setThrustersColor(SB_OFF);
      break;
    
    case 1:  // WHITE - lightspeed
      setThrustersColor(SB_WHT);
      break;
  
    case 2:  // BLUE - ridiculous speed
      setThrustersColor(SB_BLU);
      break;
    
    case 3:  // YELLOW - ludicrous speed
      setThrustersColor(SB_YEL);
      break;
    
    case 4:  // ORANGE - hyperdrive hot
      setThrustersColor(SB_ORN);
      break;
    
    case 5:  // RED - hyperdrive overheated, needing repair
      setThrustersColor(SB_RED);
      break;
  }
}

void setThrustersColor(int r, int g, int b)
{
  LEDChannels[SB_THRUSTERS][0]=r;
  LEDChannels[SB_THRUSTERS][1]=g;
  LEDChannels[SB_THRUSTERS][2]=b;
  SB_changed = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//R2D2 color functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setR2D2Color(int r, int g, int b)
{
  LEDChannels[SB_R2D2][0]=r;
  LEDChannels[SB_R2D2][1]=g;
  LEDChannels[SB_R2D2][2]=b;
  SB_changed = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Laser cannon functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setLaserCannonColor(int r, int g, int b)
{
  LEDChannels[SB_LASERCANNON][0]=r;
  LEDChannels[SB_LASERCANNON][1]=g;
  LEDChannels[SB_LASERCANNON][2]=b;
  SB_changed = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Low level shiftbrite functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void processShiftBrite()
{
  int i;
  
  if(SB_changed == 1)
  {
    SB_changed = 0;
    Serial.println("Updating ShiftBrites");

// Set the RGB values in each shiftbrite:
    SB_CommandMode = B00; // Write to PWM control registers
    for (i = 0; i < NUM_SHIFTBRITES; i++)
    {
      SB_RedCommand   = LEDChannels[i][0];
      SB_GreenCommand = LEDChannels[i][1];
      SB_BlueCommand  = LEDChannels[i][2];
      SB_SendPacket();
    }
    
// latch data into registers    
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,HIGH);
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,LOW);
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
}

void initializeShiftBrite(void)
{
  pinMode(SBdatapin, OUTPUT);
  pinMode(SBlatchpin, OUTPUT);
  pinMode(SBenablepin, OUTPUT);
  pinMode(SBclockpin, OUTPUT);
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
  digitalWrite(SBlatchpin, LOW);
  digitalWrite(SBenablepin, LOW);
  
  // Set the LED current control registers 0 = min, 127=max
  SB_RedCommand   = 127;
  SB_GreenCommand = 127;
  SB_BlueCommand  = 127;
  SB_CommandMode = B01;
  for (int i = 0; i < NUM_SHIFTBRITES; i++)
  {
    SB_SendPacket();
  }
}
