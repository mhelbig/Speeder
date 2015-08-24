#include <WaveHC.h>
#include <WaveUtil.h>
#include <MemoryFree.h>
#include <Keypad.h>
#include <Button.h>
#include <M66004Vfd.h> // VFD library.

// map functions to identifier characters:
#define CB_SEQ     'C'
#define BAD_GUYS   'B'
#define GOOD_GUYS  'G'
#define R2D2       'R'
#define CHEWY      'W'
#define LASER      'L'
#define TAKE_OFF   'T'
#define BLASTER    'P'
#define EXPLODE    'E'
#define NINE_MM    'N'
#define STARTUP    'S'
#define MOTORS_OFF 'M'

char userInput;

struct wavePlaylist
{
  char fileName[13];
  int playPriority;
};

//////////////////////////////////// SETUP
void setup() {
  Serial.begin(9600);
  Serial.println("Speeder Main System Control Module");
  Serial.print("Free Memory = ");  Serial.println(freeMemory());
  
  initializeVaccuumFluorescentDisplay();
  initializeSDcard();
  initializeRandomGenerator();
  
  initializeShiftBrite();
  
//  playStarWarsThemeSong();
  setVFDmessageActive(2, " All Systems Go");  // default message displayed if nothing else is going on
}

//////////////////////////////////// LOOP
void loop()
{
  processHyperDrive();            // Takes care of throttle, temperature, repair

  userInput = scanForUserInput(); // returns identifier key to call associated functions below
  processSimpleSounds();          // Functions that just play sound files
  runCBsequence();                // Keeps track of CB sequence and plays recorded messages
  processGoodGuys();              // Plays series of good guy sound files
  processBadGuys();               // Plays series of bad guy sound files
  processR2D2();                  // R2D2 plays sounds and color sequences in response to motion sensor
  processLaserCannon();           // Takes care of firing and eventually overheating and repair
  processChewy();                 // Plays series of Chewbacca sound files
  playRandomSounds();             // Random sounds play throughout
  
  processVFDmessages();           // display the highest priority message on the VFD screen
  processShiftBrite();            // update the shiftbrite LEDs
}

