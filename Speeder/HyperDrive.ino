// define the hyperdrive operating parameters 
#define PULLUP true        //use the Arduino's internal pullup resistors for all the inputs.
#define INVERT true        //high state = button not pressed for all inputs
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs
#define MINIMUM_REPAIR_TIME 2000 // Amount of time (in mSec) that the hyperdrive needs to be dissasembled before it can be tested and reactivated

// define the throttle parameters
#define THROTTLE_INPUT            A0
#define THROTTLE_HYSTERESIS       50
#define THROTTLE_DEBOUNCE_COUNTS  5 // used for "debouncing" the throttle input, we wait for the input to change less than this amount before we consider it stable

typedef struct modes
{
  int throttleThreshold;
  char displayMessage[17];
  char soundFile[16];
};

modes hyperDrive[]=
{
  {0,  "null"            ,"null.wav"},   //Nothing gets displayed or played when the throttle is all the way down
  {300,"  LightSpeed    ","PR01.wav"},
  {600,"Ridiculous Speed","PR02.wav"},
  {900,"Ludicrous  Speed","PRSTO.wav"},
  {900," Hyperdrive Hot ","EXPRL.wav"},
  {900," Fix Hyperdrive ","ENRSF1.wav"}
};

// define and declare the hyperdrive repair compartment inputs:
#define HYPERDRIVE_OK   24
#define HYPERDRIVE_TEST 25
Button hyperDriveOK(HYPERDRIVE_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button hyperDriveTest(HYPERDRIVE_TEST, PULLUP, INVERT, DEBOUNCE_MS);

#define HYPERDRIVE_WARNING_TEMPERATURE 10   // Time in seconds before hyperdrive gets hot when in Ludicrous speed mode
#define HYPERDRIVE_OVERHEAT_TEMPERATURE 20  // Time in seconds before hyperdrive is damaged when in Ludicrous speed mode

void processHyperDrive(void)
{
  static bool hyperDriveHasBeenDisassembled = 0;
  static unsigned long oneSecondTimer = millis();

  int rawThrottlePosition;                // raw analog reading
  static int previousRawThrottlePosition; // used for tracking position change and "debouncing" the analog input
  static int throttlePosition;            // conditioned analog reading from the throttle potentiometer

  static int previousHyperDriveMode = 1;  // deliberately setting it different than hyperDrive speed so it updates the display on startup
  int hyperDriveSpeed = 0;                // discrete hyperdrive speeds converted from analog values
  static int hyperDriveMode = 0;          // processed hyperdrive mode (speeds + overheating)
  static int hyperDriveTemperature = 0;   // effective temperature of the hyperdrive, caused by running it in ludicrous speed

  
  int i;
  
  if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE )      //the hyperdrive is overheated and needs to be repaired by the jedi before it works again
  {
    //Read all the hyperdrive compartment inputs (this needs to be called often)
    hyperDriveOK.read();
    hyperDriveTest.read();
      
    if (hyperDriveOK.releasedFor(MINIMUM_REPAIR_TIME))
    {
      hyperDriveHasBeenDisassembled = 1;
    }
    
    if ( (hyperDriveHasBeenDisassembled && hyperDriveOK.isPressed() && hyperDriveTest.wasPressed()) || userInput == FIX_HD)
    {
      hyperDriveTemperature = 0;
      hyperDriveHasBeenDisassembled = 0;
    }
    return;
  }
  
  else  // Process the throttle if we're not overheating
  {
    rawThrottlePosition = analogRead(THROTTLE_INPUT);
    
    if ( (rawThrottlePosition < previousRawThrottlePosition + THROTTLE_DEBOUNCE_COUNTS) ||   // wait until it stops changing
         (rawThrottlePosition > previousRawThrottlePosition - THROTTLE_DEBOUNCE_COUNTS) )
    {
      if ( (rawThrottlePosition > throttlePosition + THROTTLE_HYSTERESIS) ||                 // then see if it changed enough to convert it to a hyperdrive speed
           (rawThrottlePosition < throttlePosition - THROTTLE_HYSTERESIS) )
      {
        throttlePosition = (throttlePosition *.875) + (rawThrottlePosition *.125);
      }
    
      for(i=3; i>=0; i--)  //(sizeof(modes)/sizeof(modes[0]))
      {
        if(throttlePosition > hyperDrive[i].throttleThreshold)
        {
          hyperDriveSpeed = i;
          break;
        }
      }
    }
  }

  if(millis() - oneSecondTimer > 1000)    //Process the hyperdrive temperature and check for overheating
  {  
    oneSecondTimer = millis();

//    Serial.print("hyperDriveTemperature = ");  Serial.println(hyperDriveTemperature);
    switch (hyperDriveSpeed)
    {  
    case 0:
    case 1:
    case 2:
      hyperDriveTemperature = 0;  // this particular spaceship's hyperdrive cools off instantly when running below ludicrous speed
      break;
    case 3:
      if(hyperDriveTemperature < HYPERDRIVE_OVERHEAT_TEMPERATURE) hyperDriveTemperature++;  // the hyperdrive heats up when in Ludicrous Speed mode
      break;
    }
    
    if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE )
    {
      hyperDriveMode = 5;
    }
    else if (hyperDriveTemperature > HYPERDRIVE_WARNING_TEMPERATURE )
    {
      hyperDriveMode = 4;
    }
    else
    {
      hyperDriveMode = hyperDriveSpeed;
    }
  }

  if(previousHyperDriveMode != hyperDriveMode)                                        // update the display and sound if it changed from last time
  {
    previousHyperDriveMode = hyperDriveMode;
    updateHyperDriveDisplayAndSound(hyperDriveMode);
    setThrusterColor(hyperDriveMode);
  }
}

void updateHyperDriveDisplayAndSound(int setting)
{
  static int previousSetting=10;
  
  if(previousSetting!=setting)
  {
    previousSetting=setting;
    if(setting > 0)
    {
      setVFDmessageInactive(0);  // cancel any ongoing higher priority messages (cause they won't get pulled down)
      setVFDmessageActive(1, hyperDrive[setting].displayMessage);   //Show the current speed on the display
    }
    else
    {
      setVFDmessageInactive(1);
    }
    if (setting !=0)
    {
      playWaveFile(hyperDrive[setting].soundFile,4,0);
    }
  }
}
