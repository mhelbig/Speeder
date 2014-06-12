/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

boolean waveFileIsPlaying(void)
{
  return(wave.isplaying);
}

boolean playWaveFile(char *waveFile, int priority)
{
  static int currentPlayPriority = 0;
  
//  Serial.println("Entering playWaveFile()");
//  if (!wave.isplaying) currentPlayPriority = 0;  // clear any previous priorities
  
  if (wave.isplaying)
  {
    if(priority <= currentPlayPriority)
    {
      return(false);
    }
    wave.stop();
    Serial.println("Superceded by higher priority sound");
  }
  
  Serial.print("Playing: ");
  Serial.println(waveFile);
  currentPlayPriority = priority;
  
  if (!file.open(root, waveFile)) error("open by name");  // create wave and start play
  if (!wave.create(file))error("wave.create");
  wave.play();
  sdErrorCheck();  // check for play errors
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
