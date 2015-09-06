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
#define LASER      'L'
#define TAKE_OFF   'T'
#define BLASTER    'P'
#define EXPLODE    'E'
#define NINE_MM    'N'
#define STARTUP    'S'
#define MOTORS_OFF 'M'
#define FIX_HD     'F'
#define FIX_LC     'K'
#define SIMPLE_SND 's'

// Set the sound priorities, higher number = higher priority
#define PRIORITY_SIMPLE_SOUNDS   4
#define PRIORITY_CB_SOUNDS       2
#define PRIORITY_GOOD_GUY_SOUNDS 3
#define PRIORITY_BAD_GUY_SOUNDS  3
#define PRIORITY_R2D2_SOUNDS     3
#define PRIORITY_CHEWY_SOUNDS    3
#define PRIORITY_RANDOM_SOUNDS   1
#define PRIORITY_THEME_SONG      0

char userInput;

struct wavePlaylist
{
  char fileName[13];
  int playPriority;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Initialization
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(56700);
  Serial.println("Speeder Main System Control Module");
  Serial.print("Free Memory = ");  Serial.println(freeMemory());
  
  initializeVaccuumFluorescentDisplay();
  initializeSDcard();
  initializeRandomGenerator();
  
  initializeShiftBrite();
  
//  playStarWarsThemeSong();
  setVFDmessageActive(2, " All Systems Go");  // default message displayed if nothing else is going on
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main Loop
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  processHyperDrive();            // Takes care of throttle, temperature, repair

  userInput = scanForUserInput(); // returns identifier key to call associated functions below
  processSimpleSounds();          // Functions that just play sound files
  processVFDmessages();           // display the highest priority message on the VFD screen
  runCBsequence();                // Keeps track of CB sequence and plays recorded messages
  processVFDmessages();           // display the highest priority message on the VFD screen
  processGoodGuys();              // Plays series of good guy sound files
  processVFDmessages();           // display the highest priority message on the VFD screen
  processBadGuys();               // Plays series of bad guy sound files
  processVFDmessages();           // display the highest priority message on the VFD screen
  processR2D2();                  // R2D2 plays sounds and color sequences in response to motion sensor
  processVFDmessages();           // display the highest priority message on the VFD screen
  processLaserCannon();           // Takes care of firing and eventually overheating and repair
  processVFDmessages();           // display the highest priority message on the VFD screen
  playRandomSounds();             // Random sounds play throughout
  
  processVFDmessages();           // display the highest priority message on the VFD screen
  processShiftBrite();            // update the shiftbrite LEDs
}

