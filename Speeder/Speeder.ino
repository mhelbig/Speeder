#include <WaveHC.h>
#include <WaveUtil.h>
#include <MemoryFree.h>
#include <Keypad.h>

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
  initializeSDcard();
  initializeRandomGenerator();
  playStarWarsThemeSong();
  Serial.print("Free Memory = ");  Serial.println(freeMemory());
}

//////////////////////////////////// LOOP
void loop()
{
  userInput = readKeypad(); //readUserInputs();
  
  processActionSounds();
  runCBsequence();        // 'C' key
  processGoodGuys();      // 'G' key
  processBadGuys();       // 'B' key
  processR2D2();          // 'R' key
  processChewy();         // 'W' key
  playRandomSounds();
}

