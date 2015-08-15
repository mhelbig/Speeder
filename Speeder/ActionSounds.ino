void processActionSounds(void)
{
  switch (userInput)
  {  
  case 'T':
    playWaveFile("Take_Off.wav",3);
    setVFDmessageActive(0, "    Take off");
    break;
  case 'L':    
    playWaveFile("Laser.wav",4);
    setVFDmessageActive(0, "  Laser Cannon");
    SetVibratorMotorLeft(127);
    break;
  case 'P':
    playWaveFile("Blaster.wav",4);
    setVFDmessageActive(0, "    Blaster");
    SetVibratorMotorRight(127);
    break;
  case 'E':
    playWaveFile("EXPRL.wav",4);
    setVFDmessageActive(0, "   Explosion");
    break;
  case 'N':
    playWaveFile("Gun9mm.wav",4);
    setVFDmessageActive(0, "      9mm");
    break;
  case 'S':
    playWaveFile("PRSTO.wav",4);
    setVFDmessageActive(0, "    Start up");
  case 'M':
    SetVibratorMotorLeft(0);
    SetVibratorMotorRight(0);
  }
}

