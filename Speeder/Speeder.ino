#include <WaveHC.h>    // Plays .wav files from SD card
#include <WaveUtil.h>  // Plays .wav files from SD card
#include <MemoryFree.h>
#include <Keypad.h>    // Wheel interface
#include <Button.h>    // Discrete IO interface
#include <M66004Vfd.h> // VFD library.

// map wheel buttons to identifier characters:
#define WHEEL_A      'G'
#define WHEEL_B      'B'
#define WHEEL_I      'T'
#define WHEEL_II     '2'
#define WHEEL_START  'S'
#define WHEEL_SELECT 'M'
#define WHEEL_L2     'L'
#define WHEEL_R2     'R'
#define WHEEL_LF_PU  'P'
#define WHEEL_RT_PU  'D'
#define WHEEL_LF_PD  'E'
#define WHEEL_RT_PD  'N'
#define WHEEL_CTR_FW '7'
#define WHEEL_CTR_LF '8'
#define WHEEL_CTR_DN '9'  
#define WHEEL_CTR_RT '0'

//map the passenger buttons here:  If the fucntion mimics a button on the wheel, use that #define name from above
#define PASSENGER_BUTTON_1    WHEEL_I
#define PASSENGER_BUTTON_2    WHEEL_RT_PU
#define PASSENGER_BUTTON_3    WHEEL_LF_PD
#define PASSENGER_BUTTON_4    WHEEL_B
#define PASSENGER_BUTTON_5    WHEEL_START
#define PASSENGER_BUTTON_6    WHEEL_II

#define GOOD_GUYS_BUTTON      'Q'
#define BAD_GUYS_BUTTON       'W'

// map the various functions to the associated button inputs that activate them
#define LASER_CANNON_BUTTON   WHEEL_LF_PU

#define SIMPLE_SOUND_1        WHEEL_I
#define SIMPLE_SOUND_2        WHEEL_RT_PU
#define SIMPLE_SOUND_3        WHEEL_LF_PD
#define SIMPLE_SOUND_4        WHEEL_B
#define SIMPLE_SOUND_5        WHEEL_START
#define SIMPLE_SOUND_6        WHEEL_II
#define SIMPLE_SOUND_7        WHEEL_A
#define SIMPLE_SOUND_8        WHEEL_CTR_LF
#define SIMPLE_SOUND_9        WHEEL_CTR_DN
#define SIMPLE_SOUND_10       WHEEL_CTR_RT

// assorted character mappings used internally to signal things that are going on
#define CB_MIC_PUSHBUTTON 'C'
#define R2D2              'V'
#define FIX_HD            'F'
#define FIX_LC            'K'
#define SIMPLE_SND        's'

// Set the sound priorities and corresponding VFD buffers, higher number = higher priority
// Remember to adjust the size of the message display buffer if you increase the highest priority number
#define HYPERDRIVE_SOUNDS        5
#define LASER_SOUND              4
#define PRIORITY_CB_SOUNDS       3
#define PRIORITY_GOOD_GUY_SOUNDS 3
#define PRIORITY_BAD_GUY_SOUNDS  3
#define PRIORITY_SIMPLE_SOUNDS   2
#define PRIORITY_R2D2_SOUNDS     1
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
  
  initializeWheelInterface();
  initializeVaccuumFluorescentDisplay();
  initializeSDcard();
  initializeRandomGenerator();
  
  initializeShiftBrite();
  initializeCB();
  
//  playStarWarsThemeSong();
  setVFDmessageActive(0, " All Systems Go");  // default message displayed if nothing else is going on
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

