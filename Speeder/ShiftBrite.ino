#define SBclockpin  32  // CI
#define SBenablepin 34  // EI
#define SBlatchpin  33  // LI
#define SBdatapin   31  // DI

#define NumLEDs 4

unsigned long SB_CommandPacket;
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;

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

//put function here




void WriteLEDArray()
{
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


