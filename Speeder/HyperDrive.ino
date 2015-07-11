// define the hyperdrive operating parameters 
#define PULLUP true        //use the Arduino's internal pullup resistors for all the inputs.
#define INVERT true        //high state = button not pressed for all inputs
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs
#define MINIMUM_REPAIR_TIME 300 // Amount of time (in seconds) that the hyperdrive needs to be dissasembled before it can be tested and reactivated

// define and declare the hyperdrive repair compartment inputs:
#define HYPERDRIVE_OK   24
#define HYPERDRIVE_TEST 25
Button hyperDriveOK(HYPERDRIVE_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button hyperDriveTest(HYPERDRIVE_TEST, PULLUP, INVERT, DEBOUNCE_MS);

#define HYPERDRIVE_WARNING_TEMPERATURE 5    // Time in seconds before hyperdrive overheats when in Ludicrous speed mode
#define HYPERDRIVE_OVERHEAT_TEMPERATURE 10  // Time in seconds before hyperdrive overheats when in Ludicrous speed mode

static char hyperDriveSpeed = '0';
static int hyperDriveTemperature = 0;
static unsigned long oneSecondTimer = millis();

void processHyperDrive(void)
{
  processThrottleLever();
  processHyperDriveTemperature();    // Determined by throttle position
  processHyperDriveRepair();
}

void processThrottleLever(void)
{
  if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE ) return;  // stop processing the throttle until the hyperdrive is fixed
      
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

void processHyperDriveTemperature(void)
{
  // Process the hyperdrive temperature and check for overheating
  if(millis() - oneSecondTimer < 1000)  return;  // only process the hyperdrive once per second
  oneSecondTimer = millis();
  
  Serial.print("hyperDriveTemperature = ");
  Serial.println(hyperDriveTemperature);

  switch (hyperDriveSpeed)
  {  
  case '0':
    hyperDriveTemperature = 0;  // this particular spaceship's hyperdrive cools off instantly when shut down
    break;
  case '1':
    if (hyperDriveTemperature > 0) hyperDriveTemperature--;  // the hyperdrive cools when in LightSpeed mode
    break;
  case '2':
    break;
  case '3':
    if (hyperDriveTemperature < HYPERDRIVE_OVERHEAT_TEMPERATURE) hyperDriveTemperature++;  // the hyperdrive heats up when in Ludicrous Speed mode
   
    if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE )
    {
      //    playWaveFile("OVERHEAT.wav",3);
      displayMessageVFD(" Drive Overheat");
      break;
    }
    else if (hyperDriveTemperature > HYPERDRIVE_WARNING_TEMPERATURE )
    {
      //    playWaveFile("HYPERDRIVEWARNING.wav",3);
      displayMessageVFD("  Drive Warning");
    }
    break;
  }
}

void processHyperDriveRepair(void)
{
  static bool hyperDriveHasBeenDisassembled = 0;
 
  //Read all the hyperdrive compartment inputs (this needs to be called often)
  hyperDriveOK.read();
  hyperDriveTest.read();
  
  if (hyperDriveOK.releasedFor(MINIMUM_REPAIR_TIME))
    hyperDriveHasBeenDisassembled = 1;

  if (hyperDriveHasBeenDisassembled && hyperDriveOK.isPressed() && hyperDriveTest.wasPressed())
  {
    hyperDriveTemperature = 0;
    hyperDriveHasBeenDisassembled = 0;
  }
}

