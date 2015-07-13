// define the hyperdrive operating parameters 
#define PULLUP true        //use the Arduino's internal pullup resistors for all the inputs.
#define INVERT true        //high state = button not pressed for all inputs
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs
#define MINIMUM_REPAIR_TIME 300 // Amount of time (in seconds) that the hyperdrive needs to be dissasembled before it can be tested and reactivated

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
  {  0," System Ready   ","lt-spd.wav"},
  {300,"  LightSpeed    ","lt-spd.wav"},
  {600,"Ridiculous Speed","rd-spd.wav"},
  {900,"Ludicrous Speed ","ld-spd.wav"}
};

// define and declare the hyperdrive repair compartment inputs:
#define HYPERDRIVE_OK   24
#define HYPERDRIVE_TEST 25
Button hyperDriveOK(HYPERDRIVE_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button hyperDriveTest(HYPERDRIVE_TEST, PULLUP, INVERT, DEBOUNCE_MS);

#define HYPERDRIVE_WARNING_TEMPERATURE 5    // Time in seconds before hyperdrive overheats when in Ludicrous speed mode
#define HYPERDRIVE_OVERHEAT_TEMPERATURE 10  // Time in seconds before hyperdrive overheats when in Ludicrous speed mode

int throttlePositionWithHysteresis;
int hyperDriveSpeed = 0;
int hyperDriveTemperature = 0;
unsigned long oneSecondTimer = millis();

void processHyperDrive(void)
{
  processHyperDriveSpeed();             //Convert analog readings into one of three hyperdrive speeds
  processHyperDriveRepair();            //Too much ludicrous speed overheats hyperdrive, requiring repair


// TODO:  Build a function that calls updates to display and sound file only when there is a change in the hyperdrive speed changes from one mode to another

  if(millis() - oneSecondTimer > 1000)    //Only process these things once per second
  {  
    oneSecondTimer = millis();
    displayMessageVFD(hyperDrive[hyperDriveSpeed].displayMessage);   //Show the current speed on the display
    processHyperDriveTemperature();                                  //speed determines how hot the hyperdrive runs
  }
}

void processHyperDriveSpeed(void)
{
  int i;
  int throttlePosition;
  
  if (hyperDriveTemperature >= HYPERDRIVE_OVERHEAT_TEMPERATURE ) return;  // stop processing the throttle until the hyperdrive is fixed
  throttlePosition = analogRead(THROTTLE_INPUT);
  
  if ( (throttlePosition > throttlePositionWithHysteresis + THROTTLE_HYSTERESIS) || 
       (throttlePosition < throttlePositionWithHysteresis - THROTTLE_HYSTERESIS) )
  {
    throttlePositionWithHysteresis = throttlePosition;
  }

  for(i=3; i>=0; i--)  //(sizeof(modes)/sizeof(modes[0]))
  {
    if(throttlePositionWithHysteresis>hyperDrive[i].throttleThreshold)
    {
      hyperDriveSpeed = i;
      break;
    }
  }
//  Serial.print(throttlePosition); Serial.print(",");  Serial.print(throttlePositionWithHysteresis);  Serial.print(",");  Serial.println(hyperDriveSpeed);
}

void processHyperDriveTemperature(void)
{
  // Process the hyperdrive speed, temperature, and check for overheating
  
//  Serial.print("hyperDriveTemperature = ");
//  Serial.println(hyperDriveTemperature);

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
