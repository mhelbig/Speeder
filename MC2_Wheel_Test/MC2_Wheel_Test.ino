/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'Y','Z','K','U'},
  {'X','W','2','R'},
  {'V','1','4','D'},
  {'U','3','J','L'}
};

// Mad Catz MC2 Steering wheel wire color matrix connections
// Columns: 2=BLK; 3=GRY; 4=PNK; 5=PUR
// Rows:    6=RED; 7=ORN; 8=YEL; 9=BRN
// Motors:  PWR=GRN; LEFT=WHT; RIGHT=BLU

byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
