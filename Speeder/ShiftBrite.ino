#define SBclockpin  32  // CI
#define SBenablepin 34  // EI
#define SBlatchpin  33  // LI
#define SBdatapin   31  // DI

#define NUM_SHIFTBRITES 3

// define the shiftbrites in the serial chain, last device first
#define SB_LASERCANNON   0
#define SB_THRUSTERS     1
#define SB_COCKPIT       3
#define SB_R2D2          2

// define some common RGB color values
#define SB_OFF     0,    0,    0
#define SB_WHT     1023, 1023, 1023
#define SB_DIM     100,  100,  100
#define SB_RED     1023, 0,    0
#define SB_GRN     0,    1023, 0
#define SB_BLU     0,    0,    1023
#define SB_YEL     1023, 1023, 0
#define SB_ORN     1023, 255,  0

// define the specific laser color:
#define LASER_CANNON_COLOR_R 1023
#define LASER_CANNON_COLOR_G 255
#define LASER_CANNON_COLOR_B 0

// define the hyperdrive modes
#define HD_OFF         0
#define HD_LIGHTSPEED  1
#define HD_RIDICULOUS  2
#define HD_LUDICROUS   3
#define HD_HOT         4
#define HD_OVERHEATED  5

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
    case HD_OFF:
      setThrustersColor(SB_OFF);
      break;
    
    case HD_LIGHTSPEED:
      setThrustersColor(SB_WHT);
      break;
  
    case HD_RIDICULOUS:
      setThrustersColor(SB_BLU);
      break;
    
    case HD_LUDICROUS:
      setThrustersColor(SB_YEL);
      break;
    
    case HD_HOT:
      setThrustersColor(SB_ORN);
      break;
    
    case HD_OVERHEATED:
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
void setLaserCannonBrightness(float brightness)
{
  LEDChannels[SB_LASERCANNON][0] = LASER_CANNON_COLOR_R * brightness;
  LEDChannels[SB_LASERCANNON][1] = LASER_CANNON_COLOR_G * brightness;
  LEDChannels[SB_LASERCANNON][2] = LASER_CANNON_COLOR_B * brightness;
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
//    Serial.println("Updating ShiftBrites");

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

  // Initialize the various shiftbrite color states before we turn them on
  setCockpitColor(SB_DIM);
  setThrusterColor(HD_OFF);
  setLaserCannonBrightness(0);
  setR2D2Color(SB_OFF);
 }
