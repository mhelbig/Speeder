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

boolean waveFileIsPlaying(void)
{
  return(wave.isplaying);
}

boolean waveFileJustFinishedPlaying(char identifier)
{
  static bool lastPlayState = 0;
  
  if (identifier == currentWaveFileIdentifier)
  {
    if (wave.isplaying == 0 && lastPlayState == 1)
    {
      Serial.print("waveFileJustFinishedPlaying = "); Serial.println(currentWaveFileIdentifier);
      lastPlayState=0;
      currentWaveFileIdentifier = 0;
      return(1);
    }
    else
    {
      lastPlayState = wave.isplaying;
    }
  }
  return(0);
}

boolean playWaveFile(char *waveFile, int priority, char identifier)
{
  static int currentPlayPriority = 0;
  
  if (wave.isplaying)
  {
    if(priority <= currentPlayPriority)
    {
      Serial.print(waveFile);
      Serial.println(" has equal or lower priority.  Ignoring.");
      return(false);
    }
    wave.stop();
    Serial.print("Superceded by higher priority. ");
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
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
//  while(1);
}

/*
 * print error message and halt if SD I/O error, great for debugging
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
