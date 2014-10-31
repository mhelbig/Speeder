void processJetThrusters(void)
{
  if (userInput == 'T')
  {  
    playWaveFile("Take_Off.wav",4);
  }
}

void processLaser(void)
{
  if (userInput == 'L')
  {  
    playWaveFile("Laser.wav",4);
  }
}

void processBlaster(void)
{
  if (userInput == 'R')
  {  
    playWaveFile("Blaster.wav",4);
  }
}

void processExplosion(void)
{
  if (userInput == 'E')
  {  
    playWaveFile("EXPRL.wav",4);
  }
}

void processGun(void)
{
  if (userInput == 'N')
  {  
    playWaveFile("Gun9mm.wav",4);
  }
}

void processStartSpeeder(void)
{
  if (userInput == 'S')
  {  
    playWaveFile("PRSTO.wav",4);
  }
}
