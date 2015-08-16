#include <WaveHC.h>
#include <WaveUtil.h>
#include <MemoryFree.h>
#include <Keypad.h>
#include <Button.h>
#include <M66004Vfd.h> // VFD library.

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
  userInput = scanForUserInput(); //readUserConsole();
  
  processActionSounds();
  runCBsequence();        // 'C' key
  processGoodGuys();      // 'G' key
  processBadGuys();       // 'B' key
  processR2D2();          // 'R' key
  processChewy();         // 'W' key
  processHyperDrive();    // Takes care of throttle, temperature, repair
  playRandomSounds();     // Random sounds play throughout
  if (waveFileJustFinishedPlaying())
  {
    SetVibratorMotorLeft(0);
    SetVibratorMotorRight(0);
    setVFDmessageInactive(0);
    setCockpitColorWhite();
  }
  processVFDmessages();   // display the highest priority message on the VFD screen
  processShiftBrite();    // update the shiftbrite LEDs
}

