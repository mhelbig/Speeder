/*
  M66004Vfd and HpLaserJetControl libraries - Demo
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/M66004Vfd
*/

#include <M66004Vfd.h> // VFD library.

//            CS, CLK, DATA
M66004Vfd vfd(9, 13, 11);

// Out custom characters.
byte openHeart[8] = {
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b01010,
  0b00100,
  0b00000, 
  0b00000
};

byte filledHeart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

void setup() 
{
  pinMode (8, OUTPUT);
  
  // Prepare the VFD library for use.
  vfd.begin(16, 1);
 
  // Create open  as custom character 0. Note that this display supports 16 custom characters.
  vfd.createChar(0, openHeart);
  
  // Create filled heart as custom character 1.
  vfd.createChar(1, filledHeart);
 
  // Sets all pixels on the display.
  vfd.allDisplay();
  delay(500);
  
  // Enables normal character display.
  vfd.display();
}

void loop()
{
  //          "                "  
  showMessage("  Laser Armed   ");
  showMessage("  Fire at will  ");
  showMessage("                ");
  showMessage("                ");
  showMessage("    Warning !   ");
  showMessage(" Enemy in Range ");
  showMessage("                ");
  showMessage("                ");
  showMessage("    Alert !     ");
  showMessage("Hyperdrive down ");
  showMessage("                ");
  showMessage("                ");
}

void showMessage(char *message)
{
  vfd.print(message);
  delay(750);
  vfd.clear();
}  
