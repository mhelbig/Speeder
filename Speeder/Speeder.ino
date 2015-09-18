#include <WaveHC.h>    // Plays .wav files from SD card
#include <WaveUtil.h>  // Plays .wav files from SD card
#include <MemoryFree.h>
#include <Keypad.h>    // Wheel interface
#include <Button.h>    // Discrete IO interface
#include <M66004Vfd.h> // VFD library.

// map the various actions to characters
#define SIMPLE_SOUND_1        '1'
#define SIMPLE_SOUND_2        '2'
#define SIMPLE_SOUND_3        '3'
#define SIMPLE_SOUND_4        '4'
#define SIMPLE_SOUND_5        '5'
#define SIMPLE_SOUND_6        '6'
#define SIMPLE_SOUND_7        '7'
#define SIMPLE_SOUND_8        '8'
#define SIMPLE_SOUND_9        '9'
#define SIMPLE_SOUND_10       '0'

#define LASER_CANNON          'L'
#define GOOD_GUYS             'G'
#define BAD_GUYS              'B'

#define NOT_ASSIGNED          ' '

// assorted character mappings used internally to signal things that are going on
#define CB_MIC_PUSHBUTTON 'C'
#define R2D2              'V'
#define FIX_HD            'F'
#define FIX_LC            'K'
#define SIMPLE_SND        'S'


// map wheel buttons to identifier characters:
#define WHEEL_A      SIMPLE_SOUND_7
#define WHEEL_B      SIMPLE_SOUND_4
#define WHEEL_I      SIMPLE_SOUND_1
#define WHEEL_II     SIMPLE_SOUND_6
#define WHEEL_START  SIMPLE_SOUND_5
#define WHEEL_SELECT NOT_ASSIGNED
#define WHEEL_L2     NOT_ASSIGNED
#define WHEEL_R2     NOT_ASSIGNED
#define WHEEL_LF_PU  LASER_CANNON
#define WHEEL_RT_PU  SIMPLE_SOUND_2
#define WHEEL_LF_PD  SIMPLE_SOUND_3
#define WHEEL_RT_PD  NOT_ASSIGNED
#define WHEEL_CTR_FW NOT_ASSIGNED
#define WHEEL_CTR_LF SIMPLE_SOUND_8
#define WHEEL_CTR_DN SIMPLE_SOUND_9 
#define WHEEL_CTR_RT SIMPLE_SOUND_10

//map the passenger buttons to the functions:
#define PASSENGER_BUTTON_1    SIMPLE_SOUND_1
#define PASSENGER_BUTTON_2    SIMPLE_SOUND_2
#define PASSENGER_BUTTON_3    SIMPLE_SOUND_3
#define PASSENGER_BUTTON_4    SIMPLE_SOUND_4
#define PASSENGER_BUTTON_5    SIMPLE_SOUND_5
#define PASSENGER_BUTTON_6    SIMPLE_SOUND_6
#define GOOD_GUYS_BUTTON      GOOD_GUYS
#define BAD_GUYS_BUTTON       BAD_GUYS

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
  processHyperDrive();                        // Takes care of throttle, temperature, repair

  userInput = scanForUserInput();             // returns identifier key to call associated functions below
  processSimpleSounds();                      // Functions that just play sound files
  runCBsequence();                            // Keeps track of CB sequence and plays recorded messages
  processGoodGuys();                          // Plays series of good guy sound files
  processBadGuys();                           // Plays series of bad guy sound files
  processR2D2();                              // WHEEL_R2 plays sounds and color sequences in response to motion sensor
  processLaserCannon();                       // Takes care of firing and eventually overheating and repair
  processLaserCannonLightAndVibratorMotor();  //Takes care of the laser Shiftbrite and Wheel vibrator motor
  playRandomSounds();                         // Random sounds play throughout
  
  processVFDmessages();                       // display the highest priority message on the VFD screen
  processShiftBrite();                        // update the shiftbrite LEDs
  processCB_LED();
}

