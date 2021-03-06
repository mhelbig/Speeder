/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

char currentWaveFileIdentifier = 0;
char supercededWaveFileIdentifier = 0;
boolean lastPlayState = 0;

boolean waveFileIsPlaying(void)
{
  return(wave.isplaying);
}

boolean myWaveFileIsPlaying(char identifier)
{
  if(identifier == currentWaveFileIdentifier)
  {
    return(wave.isplaying);
  }
  else
  {
  return(0);
  }
}

boolean myWaveFileJustFinishedPlaying(char identifier)
{
  boolean result = 0;
  
  if (identifier == currentWaveFileIdentifier)
  {
    if (wave.isplaying == 0 && lastPlayState == 1)
    {
      Serial.print("waveFileJustFinishedPlaying = "); Serial.println(currentWaveFileIdentifier);
      lastPlayState=0;
      currentWaveFileIdentifier = 0;
      result = 1;
    }
    else
    {
      lastPlayState = wave.isplaying;
    }
  }
  if (identifier == supercededWaveFileIdentifier)
  {
    Serial.print("supercededJustFinishedPlaying = "); Serial.println(supercededWaveFileIdentifier);
    supercededWaveFileIdentifier = 0;
    result = 1;
  }
  return(result);
}

boolean playWaveFile(char *waveFile, int priority, char identifier)
{
  static int currentPlayPriority = 0;
  
  if (wave.isplaying)
  {
    if(priority <= currentPlayPriority && priority != HYPERDRIVE_SOUNDS && priority != LASER_SOUND)
    {
      Serial.print(waveFile);
      Serial.println(" has lower or equal priority.  Ignoring.");
      return(false);
    }
    else
    {
      wave.stop();
      lastPlayState=0;
      Serial.print("Superceded by higher priority. Saving '"); Serial.print(currentWaveFileIdentifier); Serial.println("' ");
      supercededWaveFileIdentifier = currentWaveFileIdentifier;  // save the previous identifier so that functions can see that it's no longer playing their file
    }
  }
  
  Serial.print("Playing: ");
  Serial.println(waveFile);
  currentPlayPriority = priority;
  currentWaveFileIdentifier = identifier;    // record the wave file identifier so that we can see when they finish playing
  
  if (!file.open(root, waveFile)) error("open by name");  // create wave and start play
  if (!wave.create(file))error("wave.create");
  wave.play();
  sdErrorCheck();  // check for play errors
  return(true);    // true means we played the file
}

void initializeSDcard(void)
{
  if (!card.init()) error("card.init");

  // enable optimized read - some cards may timeout
  card.partialBlockRead(true);

  if (!vol.init(card)) error("vol.init");

  if (!root.openRoot(vol)) error("openRoot");
  Serial.println("SD card initialized");
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) 
{
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
}

/*
 * print error message and halt if SD I/O error, great for debugging
 */
void sdErrorCheck(void) 
{
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  directDisplayVFDmessage(" SD Card Error");
  while(1);
}
