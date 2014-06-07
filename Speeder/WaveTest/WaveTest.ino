#include <WaveHC.h>
#include <WaveUtil.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

// time to play each tone in milliseconds
#define PLAY_TIME 20000

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

//////////////////////////////////// SETUP
void setup() {
  Serial.begin(9600);

  if (!card.init()) error("card.init");

  // enable optimized read - some cards may timeout
  card.partialBlockRead(true);

  if (!vol.init(card)) error("vol.init");

  if (!root.openRoot(vol)) error("openRoot");

  PgmPrintln("Play files by name");
  playByName();
}

//////////////////////////////////// LOOP
void loop() { }

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
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

// Number of files.
#define FILE_COUNT 3

char* fileList[] =  {"Alien001.wav", "Chew001.wav", "R2D2004.wav"}; 
      
void playByName(void) {
  char name[10];
  
  // copy flash string to RAM
  strcpy_P(name, PSTR("DTMFx.WAV"));
  
  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    // start time
    uint32_t t = millis();
    
    // open file by name
    Serial.print("Opening: ");
    Serial.println(fileList[i]);
    if (!file.open(root, fileList[i])) error("open by name"); 
    
    // create wave and start play
    if (!wave.create(file))error("wave.create");
    wave.play();
    
    // print time
    Serial.println(millis() - t);
    
    // stop after PLAY_TIME ms
     while (wave.isplaying);
    wave.stop();
    
    // check for play errors
    sdErrorCheck();
  }
  PgmPrintln("Done");
}

