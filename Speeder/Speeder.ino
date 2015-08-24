#include <WaveHC.h>
#include <WaveUtil.h>
#include <MemoryFree.h>
#include <Keypad.h>
#include <Button.h>
#include <M66004Vfd.h> // VFD library.

// functions to identifier character mappings:
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

// define some common RGB color values
#define SB_OFF     0,    0,    0
#define SB_WHT     1023, 1023, 1023
#define SB_DIM     100,  100,  100
#define SB_RED     1023, 0,    0
#define SB_GRN     0,    1023, 0
#define SB_BLU     0,    0,    1023
#define SB_YEL     1023, 1023, 0
#define SB_ORN     1023, 255,  0

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
  
// Initialize the various shiftbrite color states before we turn them on
  setCockpitColor(SB_DIM);
  setThrusterColor(0);
  setLaserCannonColor(SB_OFF);
  // R2D2 goes here
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

