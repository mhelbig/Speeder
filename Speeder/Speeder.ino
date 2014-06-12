#include <WaveHC.h>
#include <WaveUtil.h>

struct wavePlaylist
{
  char fileName[13];
  int playPriority;
};

//////////////////////////////////// SETUP
void setup() {
  Serial.begin(9600);
  Serial.println("Speeder Project Main System Control Module");
  initializeSDcard();
  initializeRandomGenerator();
  Serial.println("System ready");
  
//  playWaveFile("SWTM001.wav",10);  //start things off with the Star Wars theme song
}

//////////////////////////////////// LOOP
void loop()
{
 playRandomSounds();
 runCBsequence();
 processGoodGuys();
}

