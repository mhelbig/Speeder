// define the hyperdrive operating parameters 
#define PULLUP true        //use the Arduino's internal pullup resistors for all the inputs.
#define INVERT true        //high state = button not pressed for all inputs
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs
#define MINIMUM_REPAIR_TIME 2000 // Amount of time (in mSec) that the hyperdrive needs to be dissasembled before it can be tested and reactivated

// define the throttle parameters
#define THROTTLE_INPUT            A0
#define THROTTLE_HYSTERESIS       50

typedef struct modes
{
  int throttleThreshold;
  char displayMessage[17];
  char soundFile[16];
};

modes hyperDrive[]=
{
  {  0," Hyperdrive Off ","lt-spd.wav"},
  {300,"  LightSpeed    ","lt-spd.wav"},
  {600,"Ridiculous Speed","rd-spd.wav"},
  {900,"Ludicrous Speed ","ld-spd.wav"},
  {900," Hyperdrive Hot ","ld-spd.wav"},
  {900," Fix Hyperdrive ","ld-spd.wav"}
};

// define and declare the hyperdrive repair compartment inputs:
#define HYPERDRIVE_OK   24
#define HYPERDRIVE_TEST 25
Button hyperDriveOK(HYPERDRIVE_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button hyperDriveTest(HYPERDRIVE_TEST, PULLUP, INVERT, DEBOUNCE_MS);

#define HYPERDRIVE_WARNING_TEMPERATURE 5    // Time in seconds before hyperdrive overheats when in Ludicrous speed mode
#define HYPERDRIVE_OVERHEAT_TEMPERATURE 10  // Time in seconds before hyperdrive overheats when in Ludicrous speed mode

int throttlePosition;
int hyperDriveSpeed = 0;
int previousHyperDriveSpeed = 1;  // deliberately setting it different than hyperDrive speed so it updates the display on startup
int hyperDriveTemperature = 0;
unsigned long oneSecondTimer = millis();

void processHyperDrive(void)
{
  int i;
  int rawThrottlePosition;
  static bool hyperDriveHasBeenDisassembled = 0;
  
  if (hyperDriveTemperature < HYPERDRIVE_OVERHEAT_TEMPERATURE )      //Process the throttle if we're not overheating
  {
    rawThrottlePosition = analogRead(THROTTLE_INPUT);
    
    if ( (rawThrottlePosition > throttlePosition + THROTTLE_HYSTERESIS) || 
         (rawThrottlePosition < throttlePosition - THROTTLE_HYSTERESIS) )
    {
      throttlePosition = rawThrottlePosition;
    }
  
    for(i=3; i>=0; i--)  //(sizeof(modes)/sizeof(modes[0]))
    {
      if(throttlePosition > hyperDrive[i].throttleThreshold)
      {
        hyperDriveSpeed = i;
        break;
      }
    }
    
    if(previousHyperDriveSpeed != hyperDriveSpeed)
    {
      previousHyperDriveSpeed = hyperDriveSpeed;
      updateHyperDriveDisplayAndSound(hyperDriveSpeed);
    }
  }
  else  // the hyperdrive is overheated and needs to be repaired by the jedi
  {
    //Read all the hyperdrive compartment inputs (this needs to be called often)
    hyperDriveOK.read();
    hyperDriveTest.read();
      
    if (hyperDriveOK.releasedFor(MINIMUM_REPAIR_TIME))
    {
      hyperDriveHasBeenDisassembled = 1;
    }
    
    if (hyperDriveHasBeenDisassembled && hyperDriveOK.isPressed() && hyperDriveTest.wasPressed())
    {
      hyperDriveTemperature = 0;
      hyperDriveHasBeenDisassembled = 0;
      previousHyperDriveSpeed=10;  //force an update to the current throttle setting
    }
  }

  if(millis() - oneSecondTimer > 1000)    //Process the hyperdrive temperature and check for overheating
  {  
    oneSecondTimer = millis();

    Serial.println(hyperDriveTemperature);
    switch (hyperDriveSpeed)
    {  
    case 0:
      hyperDriveTemperature = 0;  // this particular spaceship's hyperdrive cools off instantly when shut down
      break;
    case 1:
      if (hyperDriveTemperature > 0) hyperDriveTemperature--;  // the hyperdrive cools when in LightSpeed mode
      break;
    case 2:
      break;
    case 3:
      if(hyperDriveTemperature < HYPERDRIVE_OVERHEAT_TEMPERATURE) hyperDriveTemperature++;  // the hyperdrive heats up when in Ludicrous Speed mode
      break;
    }
    
    if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE )
    {
      updateHyperDriveDisplayAndSound(5);
    }
    else if (hyperDriveTemperature > HYPERDRIVE_WARNING_TEMPERATURE )
    {
      updateHyperDriveDisplayAndSound(4);
    }
  }
}

void updateHyperDriveDisplayAndSound(int setting)
{
  static int previousSetting=10;
  
  if(previousSetting!=setting)
  {
    previousSetting=setting;
    displayMessageVFD(hyperDrive[setting].displayMessage);   //Show the current speed on the display
  // also play associated WAV file here
  }
}

void oldKeyboardHyperDriveFunction(void)
{
    switch (userInput)  // This will be replaced by a routine that reads the A/D input and determines the speed from that
  {
    case '0':
      hyperDriveSpeed = '0';
      displayMessageVFD("  System Ready");
      break;
    case '1':
      hyperDriveSpeed = '1';
      displayMessageVFD("  Light Speed");
//    playWaveFile("LtSpeed.wav",3);
      break;
    case '2':
      hyperDriveSpeed = '2';
      displayMessageVFD("Ridiculous Speed");
//    playWaveFile("RdSpeed.wav",3);
      break;
    case '3':
      hyperDriveSpeed = '3';
      displayMessageVFD("Ludicrous Speed");
//    playWaveFile("LdSpeed.wav",3);
      break;
  }
}
