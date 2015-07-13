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
  initializeVaccuumFluorescentDisplay();
  initializeSDcard();
  initializeRandomGenerator();
  displayMessageVFD("  System Ready");
//  playStarWarsThemeSong();
  Serial.print("Free Memory = ");  Serial.println(freeMemory());
}

//////////////////////////////////// LOOP
void loop()
{
  userInput = scanForUserInput(); //readUserConsole();
  
  processActionSounds();
  if (!waveFileIsPlaying())
  {
    SetVibratorMotorLeft(0);
    SetVibratorMotorRight(0);
  }
  runCBsequence();        // 'C' key
  processGoodGuys();      // 'G' key
  processBadGuys();       // 'B' key
  processR2D2();          // 'R' key
  processChewy();         // 'W' key
  processHyperDrive();    // Takes care of throttle, temperature, repair
//  playRandomSounds();
}

