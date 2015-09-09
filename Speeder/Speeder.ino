#include <WaveHC.h>
#include <WaveUtil.h>
#include <MemoryFree.h>
#include <Keypad.h>
#include <Button.h>
#include <M66004Vfd.h> // VFD library.

// map wheel buttons to identifier characters:
#define WHEEL_A      'G'
#define WHEEL_B      'B'
#define WHEEL_I      'T'
#define WHEEL_II     '2'
#define WHEEL_START  'S'
#define WHEEL_SELECT 'M'
#define WHEEL_L2     'C'
#define WHEEL_R2     'R'
#define WHEEL_LF_PU  'L'
#define WHEEL_RT_PU  'P'
#define WHEEL_LF_PD  'E'
#define WHEEL_RT_PD  'N'
#define WHEEL_CTR_FW '7'
#define WHEEL_CTR_LF '8'
#define WHEEL_CTR_DN '9'  
#define WHEEL_CTR_RT '0'

// passenger buttons mimic certain buttons on the wheel.  Map them here:
#define PASSENGER_BUTTON_1    WHEEL_I
#define PASSENGER_BUTTON_2    WHEEL_LF_PU
#define PASSENGER_BUTTON_3    WHEEL_RT_PU
#define PASSENGER_BUTTON_4    WHEEL_LF_PD
#define PASSENGER_BUTTON_5    WHEEL_RT_PD
#define PASSENGER_BUTTON_6    WHEEL_START

// map the various functions to the associated button inputs that activate them
#define LASER_CANNON_BUTTON   WHEEL_RT_PU
#define GOOD_GUYS_BUTTON      WHEEL_A
#define BAD_GUYS_BUTTON       WHEEL_B

#define SIMPLE_SOUND_1        WHEEL_I
#define SIMPLE_SOUND_2        WHEEL_RT_PU
#define SIMPLE_SOUND_3        WHEEL_LF_PD
#define SIMPLE_SOUND_4        WHEEL_RT_PD
#define SIMPLE_SOUND_5        WHEEL_START
#define SIMPLE_SOUND_6        WHEEL_II

// assorted character mappings used internally to signal things that are going on
#define FIX_HD       'F'
#define FIX_LC       'K'
#define SIMPLE_SND   's'

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
  runCBsequence();                // Keeps track of CB sequence and plays recorded messages
  processGoodGuys();              // Plays series of good guy sound files
  processBadGuys();               // Plays series of bad guy sound files
  processR2D2();                  // WHEEL_R2 plays sounds and color sequences in response to motion sensor
  processLaserCannon();           // Takes care of firing and eventually overheating and repair
  playRandomSounds();             // Random sounds play throughout
  
  processVFDmessages();           // display the highest priority message on the VFD screen
  processShiftBrite();            // update the shiftbrite LEDs
}

