#define LASER_LIGHT_CYCLE_TIME           30  // How often the shiftbrites are updated (in Milliseconds)
#define LASER_FADE_FACTOR                .04 // Rate that laser cannon dims (from 100 down to 0)
#define MAX_LASER_FIRES_PER_MINUTE       6   // How many times you can fire the laser cannon in a minute before it overheats

void processLaserCannon(void)
{
  static unsigned long waitTimer;
  static unsigned long oneSecondTimer = millis();
  static float laserCannonBrightness;
  static int laserCannonTemperature = 0;
  
  if(millis() - oneSecondTimer > 1000)    //Process the laser cannon temperature and check for overheating
  {  
    oneSecondTimer = millis();
    if(laserCannonTemperature >0)
    {
      laserCannonTemperature--;
    }
  }

  if(myWaveFileJustFinishedPlaying(LASER))
  {
    setVFDmessageInactive(0);
    SetVibratorMotorLeft(0);
    setLaserCannonBrightness(0);
  }
  else if(myWaveFileIsPlaying(LASER))
  {
    if (millis() < waitTimer) return;
    
    setLaserCannonBrightness(laserCannonBrightness);
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    if (laserCannonBrightness >0)
    {
      laserCannonBrightness -= LASER_FADE_FACTOR;
    }
  }
  else if (userInput == LASER)
  {  
    waitTimer = millis() + LASER_LIGHT_CYCLE_TIME;
    laserCannonBrightness = 1;
    playWaveFile("Laser.wav",4,userInput);
    setVFDmessageActive(0, "  Laser Cannon");
    SetVibratorMotorLeft(127);
    setLaserCannonBrightness(laserCannonBrightness);
    laserCannonTemperature += (60 / MAX_LASER_FIRES_PER_MINUTE);
    Serial.print("laserCannonTemperature = "); Serial.println(laserCannonTemperature);
  }
   
  if(laserCannonTemperature > 60)
 {
  Serial.println("Laser temperature too high");
 } 
}



