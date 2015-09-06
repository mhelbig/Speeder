// Define the laser cannon operating parameters
#define LASER_LIGHT_CYCLE_TIME           30  // How often the shiftbrites are updated (in Milliseconds)
#define LASER_FADE_FACTOR                .04 // Rate that laser cannon dims (from 100 down to 0)
#define LASER_THERMAL_RISE               10  // amount of thermal rise each time the laser is fired (laser cools off one unit per second)
#define LASER_GETTING_HOT                20  // temperature where color of laser cannon changes to show that it's getting hot
#define LASER_OVERHEATED                 30  // laser cannon temperature when overheated
#define PRIORITY_LASER_CANNON            5
#define VFD_BUFFER_LASER_CANNON          

// define and declare the electrical compartment repair inputs:
#define LASERCANNON_OK   26
#define LASERCANNON_TEST 27
Button laserCannonOK(LASERCANNON_OK, PULLUP, INVERT, DEBOUNCE_MS);
Button laserCannonTest(LASERCANNON_TEST, PULLUP, INVERT, DEBOUNCE_MS);

unsigned long waitTimer;
unsigned long oneSecondTimer = millis();
float laserCannonBrightness;
int laserCannonTemperature = 0;
bool electricalCompartmentHasBeenDisassembled = 0;
bool laserCannonIsOverheated = 0;
  
void processLaserCannon(void)
{
  if(myWaveFileJustFinishedPlaying(LASER)) // shut off the vibrator motors and laser cannon light when the sound is done playing
  {
    setVFDmessageInactive(0);
    SetVibratorMotorLeft(0);
    setLaserCannonBrightness(0,laserCannonTemperature>LASER_GETTING_HOT);
  }
  
  else if(myWaveFileIsPlaying(LASER))  // modulate the light inside the laser cannon when the sound is playing
  {
    if (millis() < waitTimer) return;
    
    setLaserCannonBrightness(laserCannonBrightness, laserCannonTemperature>LASER_GETTING_HOT);
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    if (laserCannonBrightness >0)
    {
      laserCannonBrightness -= LASER_FADE_FACTOR;
    }
  }
  
  else if (laserCannonIsOverheated == 1)
  {
    //Read all the electrical compartment inputs (this needs to be called often)
    laserCannonOK.read();
    laserCannonTest.read();
      
    if (laserCannonOK.releasedFor(MINIMUM_REPAIR_TIME))
    {
      electricalCompartmentHasBeenDisassembled = 1;
    }
    
    if ( (electricalCompartmentHasBeenDisassembled && laserCannonOK.isPressed() && laserCannonTest.wasPressed()) || userInput == FIX_LC)
    {
      laserCannonIsOverheated = 0;
      laserCannonTemperature = 0;
      electricalCompartmentHasBeenDisassembled = 0;
      setLaserCannonBrightness(0, 0);
      setVFDmessageInactive(0);
    }
    return;  // stop processing the rest of the laser cannon features until it gets repaired
  } 

  else if(laserCannonTemperature > LASER_OVERHEATED) // check if the laser cannon temperature is too high
  {
    playWaveFile("ENRSF1.wav",4,userInput);
    setVFDmessageActive(0, " Laser Overheat");
    laserCannonIsOverheated = 1;
  }

  if(millis() - oneSecondTimer > 1000)    //Process the laser cannon temperature
  {  
    oneSecondTimer = millis();
    if(laserCannonTemperature >0)
    {
      laserCannonTemperature--;
    }
    
    if(laserCannonTemperature == LASER_GETTING_HOT)
    {
      setLaserCannonBrightness(laserCannonBrightness,0);  //turn off the laser glow once we cool down enough
    }      
  }
  
  else if (userInput == LASER)  // check for user input and start the laser cannon fire sequence
  {  
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    laserCannonBrightness = 1;
    playWaveFile("Laser.wav",4,userInput);
    setVFDmessageActive(0, "  Laser Cannon");
    SetVibratorMotorLeft(127);
    setLaserCannonBrightness(laserCannonBrightness,laserCannonTemperature>LASER_GETTING_HOT);
    laserCannonTemperature += LASER_THERMAL_RISE;
    Serial.print("laserCannonTemperature = "); Serial.println(laserCannonTemperature);
  }
}



