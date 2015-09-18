// Define the laser cannon operating parameters
#define LASER_LIGHT_CYCLE_TIME           30  // How often the shiftbrites are updated (in Milliseconds)
#define LASER_FADE_FACTOR                .04 // Rate that laser cannon dims (from 100 down to 0)
#define LASER_THERMAL_RISE               10  // amount of thermal rise each time the laser is fired (laser cools off one unit per second)
#define LASER_GETTING_HOT                30  // temperature where color of laser cannon changes to show that it's getting hot
#define LASER_OVERHEATED                 40  // laser cannon temperature when overheated
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
 //Read all the electrical compartment inputs (this needs to be called often)
  laserCannonOK.read();
  laserCannonTest.read();

  if(userInput == FIX_LC)
  {
    resetLaserCannon();
    Serial.println("Manual LaserCannon reset via console input");
    return;
  }    
  
  if (laserCannonIsOverheated)
  {
    if (laserCannonOK.releasedFor(MINIMUM_REPAIR_TIME))
    {
      electricalCompartmentHasBeenDisassembled = 1;
    }
    
    if ( laserCannonTest.wasPressed() )
    {
      Serial.println("laserCannonTest.wasPressed()");
      if ( electricalCompartmentHasBeenDisassembled && laserCannonOK.isPressed() )
      {
        resetLaserCannon();
      }
      else
      {
        // do something to tell them they failed
      }
    }
    return;
  }

  processLaserCannonTemperature();
  if(laserCannonTemperature > LASER_OVERHEATED) // check if the laser cannon temperature is too high
  {
    playWaveFile("ENRSF1.wav",LASER_SOUND,userInput);
    setVFDmessageActive(LASER_SOUND, " Laser Overheat");
    laserCannonIsOverheated = 1;
    return;
  }
  
  if (userInput == LASER_CANNON)  // check for user input and start the laser cannon fire sequence
  {  
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    fireLaserCannonLightAndVibratorMotor();
  }
}

void fireLaserCannonLightAndVibratorMotor(void)
{
  SetVibratorMotorLeft(127);
  setLaserCannonBrightness(laserCannonBrightness,laserCannonTemperature>LASER_GETTING_HOT);
  laserCannonBrightness = 1;
  playWaveFile("Laser.wav",LASER_SOUND,LASER_CANNON);
  setVFDmessageActive(LASER_SOUND, "  Laser Cannon");
  laserCannonTemperature += LASER_THERMAL_RISE;
  Serial.print("laserCannonTemperature = "); Serial.println(laserCannonTemperature);
}

void processLaserCannonLightAndVibratorMotor(void)
{
  if(myWaveFileJustFinishedPlaying(LASER_CANNON))
  {
    setVFDmessageInactive(LASER_SOUND);
  }

  if(laserCannonBrightness <= LASER_FADE_FACTOR) // shut off the vibrator motors when the laser cannon light is out
  {
    SetVibratorMotorLeft(0);
    setLaserCannonBrightness(0,laserCannonTemperature>LASER_GETTING_HOT);
  }
  
  else // modulate the light inside the laser cannon
  {
    if (millis() < waitTimer) return;
    
    setLaserCannonBrightness(laserCannonBrightness, laserCannonTemperature>LASER_GETTING_HOT);
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    if (laserCannonBrightness >0)
    {
      laserCannonBrightness -= LASER_FADE_FACTOR;
    }
    return;
  }
}

void processLaserCannonTemperature(void)
{
  if(millis() - oneSecondTimer > 1000)    //Process the laser cannon temperature
  {  
    oneSecondTimer = millis();
//    Serial.print("laserCannonTemperature = "); Serial.println(laserCannonTemperature);
    if(laserCannonTemperature >0)
    {
      laserCannonTemperature--;
    }
    
    if(laserCannonTemperature == LASER_GETTING_HOT)
    {
      setLaserCannonBrightness(laserCannonBrightness,0);  //turn off the laser glow once we cool down enough
    }      
  }
}

void resetLaserCannon(void)
{
  laserCannonIsOverheated = 0;
  laserCannonTemperature = 0;
  electricalCompartmentHasBeenDisassembled = 0;
  setLaserCannonBrightness(0, 0);
  setVFDmessageInactive(LASER_SOUND);
  // PLAY SOUND HERE !!!!!!!!!!
}


