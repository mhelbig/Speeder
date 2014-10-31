void processActionSounds(void)
{
  switch (userInput)
  {  
  case 'T':
    playWaveFile("Take_Off.wav",3);
    break;
  case 'L':    
    playWaveFile("Laser.wav",4);
    break;
  case 'P':
    playWaveFile("Blaster.wav",4);
    break;
  case 'E':
    playWaveFile("EXPRL.wav",4);
    break;
  case 'N':
    playWaveFile("Gun9mm.wav",4);
    break;
  case 'S':
    playWaveFile("PRSTO.wav",4);
  }
}

