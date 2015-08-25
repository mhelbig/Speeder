#define LASER_LIGHT_CYCLE_TIME 30    // How often the shiftbrites are updated (in Milliseconds)
#define LASER_FADE_FACTOR      .04   // Rate that laser cannon dims (from 100 down to 0)

void processLaserCannon(void)
{
  static long waitTimer;
  static float laserCannonBrightness;
  
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
  }
}



