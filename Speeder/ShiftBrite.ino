#define SBclockpin  32  // CI
#define SBenablepin 34  // EI
#define SBlatchpin  33  // LI
#define SBdatapin   31  // DI

#define NUM_SHIFTBRITES 3

// define the shiftbrites as they are located in the serial chain in reverse order:
#define SB_R2D2          3
#define SB_COCKPIT       2
#define SB_THRUSTERS     1
#define SB_LASERCANNON   0

unsigned long SB_CommandPacket;
int LEDChannels[NUM_SHIFTBRITES][3] = {0};
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
  
  setCockpitColorNormal();
  setThrusterColor(0);
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Cockpit color functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setCockpitColorBadGuy(void)
{
  LEDChannels[SB_COCKPIT][0]=1023;
  LEDChannels[SB_COCKPIT][1]=0;
  LEDChannels[SB_COCKPIT][2]=0;
  SB_changed = 1;
}

void setCockpitColorGoodGuy(void)
{
  LEDChannels[SB_COCKPIT][0]=0;
  LEDChannels[SB_COCKPIT][1]=1023;
  LEDChannels[SB_COCKPIT][2]=0;
  SB_changed = 1;
}

void setCockpitColorNormal(void)
{
  LEDChannels[SB_COCKPIT][0]=100;
  LEDChannels[SB_COCKPIT][1]=100;
  LEDChannels[SB_COCKPIT][2]=100;
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
      LEDChannels[SB_THRUSTERS][0]=0;
      LEDChannels[SB_THRUSTERS][1]=0;
      LEDChannels[SB_THRUSTERS][2]=0;
      break;
    
    case 1:  // WHITE - lightspeed
      LEDChannels[SB_THRUSTERS][0]=1023;
      LEDChannels[SB_THRUSTERS][1]=1023;
      LEDChannels[SB_THRUSTERS][2]=1023;
      break;
  
    case 2:  // BLUE - ridiculous speed
      LEDChannels[SB_THRUSTERS][0]=0;
      LEDChannels[SB_THRUSTERS][1]=0;
      LEDChannels[SB_THRUSTERS][2]=1023;
      break;
    
    case 3:  // YELLOW - ludicrous speed
      LEDChannels[SB_THRUSTERS][0]=1023;
      LEDChannels[SB_THRUSTERS][1]=1023;
      LEDChannels[SB_THRUSTERS][2]=0;
      break;
    
    case 4:  // ORANGE - hyperdrive hot
      LEDChannels[SB_THRUSTERS][0]=1023;
      LEDChannels[SB_THRUSTERS][1]=255;
      LEDChannels[SB_THRUSTERS][2]=0;
      break;
    
    case 5:  // RED - hyperdrive overheated, needing repair
      LEDChannels[SB_THRUSTERS][0]=1023;
      LEDChannels[SB_THRUSTERS][1]=0;
      LEDChannels[SB_THRUSTERS][2]=0;
      break;
  }
  SB_changed = 1;
}

void processShiftBrite()
{
  if(SB_changed == 1)
  {
    SB_changed = 0;
    Serial.print("Updating ShiftBrite: ");

// Write to current control registers 0 = min, 127=max
    SB_RedCommand   = 127;
    SB_GreenCommand = 127;
    SB_BlueCommand  = 127;
    SB_CommandMode = B01;
    for (int z = 0; z < NUM_SHIFTBRITES; z++)
    {
      SB_SendPacket();
    }
    
// Write to PWM control registers:
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0; h<NUM_SHIFTBRITES; h++)
    {
  	  SB_RedCommand = LEDChannels[h][0];
  	  SB_GreenCommand = LEDChannels[h][1];
  	  SB_BlueCommand = LEDChannels[h][2];
  	  SB_SendPacket();
          Serial.print(h);
          Serial.print("=");
          Serial.print(LEDChannels[h][0]);
          Serial.print(",");
          Serial.print(LEDChannels[h][1]);
          Serial.print(",");
          Serial.print(LEDChannels[h][2]);
          Serial.print(" ");
    }
    Serial.println();
    delayMicroseconds(15);
    digitalWrite(SBlatchpin,HIGH); // latch data into registers
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
