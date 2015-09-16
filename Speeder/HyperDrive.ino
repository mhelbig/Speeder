// define the hyperdrive operating parameters 
#define PULLUP true        //use the Arduino's internal pullup resistors for all the inputs.
#define INVERT true        //high state = button not pressed for all inputs
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs
#define MINIMUM_REPAIR_TIME 2000 // Amount of time (in mSec) that the hyperdrive needs to be dissasembled before it can be tested and reactivated

// define the throttle parameters
#define THROTTLE_INPUT            A0
#define THROTTLE_HYSTERESIS       15
#define THROTTLE_DEBOUNCE_COUNTS  5 // used for "debouncing" the throttle input, we wait for the input to change less than this amount before we consider it stable

typedef struct modes
{
  int throttleThreshold;
  char displayMessage[17];
  char soundFile[16];
};

modes hyperDrive[]=
{
  {0,  "null"             , "null.wav"},    //Nothing gets displayed or played when the throttle is all the way down
  {45,  "  LightSpeed    ", "PR01.wav"},    // 1
  {100, "Ridiculous Speed", "PR02.wav"},    // 2
  {175, "Ludicrous  Speed", "PRSTO.wav"},   // 3
  {200, " Hyperdrive Hot ", "EXPRL.wav"},   // 4
  {200, " Fix Hyperdrive ", "ENRSF1.wav"}   // 5
};

// define and declare the hyperdrive repair compartment inputs:
#define HYPERDRIVE_OK   24
#define HYPERDRIVE_TEST 25
Button hyperDriveOK(HYPERDRIVE_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button hyperDriveTest(HYPERDRIVE_TEST, PULLUP, INVERT, DEBOUNCE_MS);

int throttlePosition;            // conditioned analog reading from the throttle potentiometer
int hyperDriveSpeed = 0;                // discrete hyperdrive speeds converted from analog values
int hyperDriveMode = 0;          // processed hyperdrive mode (speeds + overheating)
int hyperDriveTemperature = 0;   // effective temperature of the hyperdrive, caused by running it in ludicrous speed
bool hyperDriveHasBeenDisassembled = 0;

#define HYPERDRIVE_WARNING_TEMPERATURE 20   // Time in seconds before hyperdrive gets hot when in Ludicrous speed mode
#define HYPERDRIVE_OVERHEAT_TEMPERATURE 30  // Time in seconds before hyperdrive is damaged when in Ludicrous speed mode

void processHyperDrive(void)
{
  //Read all the hyperdrive compartment inputs (this needs to be called often)
  hyperDriveOK.read();
  hyperDriveTest.read();
  
  updateHyperDriveDisplayAndSound(hyperDriveMode);

  if(hyperDriveOK.isReleased())    //check to see if engine is apart
  {
    hyperDriveMode = 5;            //set it to fix hyperdrive mode if it's apart
  }
  
  if(hyperDriveMode <=4)          // run the hyperdrive at the throttle setting if it's OK
  {
    runHyperDrive();
    processHyperDriveTemperature();
    return;
  }

  if(hyperDriveMode == 5)          // it needs to be repaired
  {
  if(userInput == FIX_HD)
  {
    resetHyperDrive();
    Serial.println("Manual HyperDrive reset via console input");
    return;
  }
    if (hyperDriveOK.releasedFor(MINIMUM_REPAIR_TIME))
    {
      hyperDriveHasBeenDisassembled = 1;
    }
    
    if ( hyperDriveTest.wasPressed() )
    {
      Serial.println("hyperDriveTest.wasPressed()");
      if ( hyperDriveHasBeenDisassembled && hyperDriveOK.isPressed() )
      {
        resetHyperDrive();
      }
      else
      {
        updateHyperDriveDisplayAndSound(0);  //Temporarily reset the mode to trigger the system failure sound and message
        updateHyperDriveDisplayAndSound(5);
      }
    }
  }
}

void runHyperDrive(void)
{
  static int previousRawThrottlePosition; // used for tracking position change and "debouncing" the analog input
  int rawThrottlePosition;                // raw analog reading
  int i;

  rawThrottlePosition = analogRead(THROTTLE_INPUT);
  
  if ( (rawThrottlePosition < previousRawThrottlePosition + THROTTLE_DEBOUNCE_COUNTS) ||   // wait until it stops changing   !!!! this is going too fast to be of value, throttle this with another timer !!!!
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

void processHyperDriveTemperature(void)
{
  static unsigned long oneSecondTimer = millis();
  
  if(millis() - oneSecondTimer > 1000)    //Process the hyperdrive temperature and check for overheating
  {  
    oneSecondTimer = millis();

//    Serial.print("hyperDriveMode = "); Serial.println(hyperDriveMode);
    
    if(hyperDriveSpeed < 3)
    {
      hyperDriveTemperature = 0;  // this particular spaceship's hyperdrive cools off instantly when running below ludicrous speed
    }
    else
    {
      if(hyperDriveTemperature < HYPERDRIVE_OVERHEAT_TEMPERATURE) hyperDriveTemperature++;  // the hyperdrive heats up when in Ludicrous Speed mode
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
}

void resetHyperDrive(void)
{
  hyperDriveTemperature = 0;
  hyperDriveHasBeenDisassembled = 0;
  hyperDriveMode = 0;
  Serial.println("HyperDrive Reset");
}

void updateHyperDriveDisplayAndSound(int setting)
{
  static int previousSetting=10;
  
  if(previousSetting!=setting)
  {
    previousSetting=setting;
    setThrusterColor(setting);

    if(setting > 0)
    {
      setVFDmessageActive(HYPERDRIVE_SOUNDS, hyperDrive[setting].displayMessage);   //Show the current speed on the display
    }
    else
    {
      setVFDmessageInactive(HYPERDRIVE_SOUNDS);
    }
    if (setting !=0)
    {
      playWaveFile(hyperDrive[setting].soundFile,HYPERDRIVE_SOUNDS,'.');
    }
  }
}
