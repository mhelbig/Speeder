const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = 
{
  {'B','1','C','1'}, //  | B      | R2     | SELECT | CTR FWD
  {'G','1','E','1'}, //  | A      | L2     | LF PD  | CTR RT
  {'T','L','N','1'}, //  | I      | LF PU  | RT PD  | CTR DN
  {'1','R','S','1'}  //  | II     | RT PU  | START  | CTR LF
};

// Mad Catz MC2 Steering wheel wire color matrix connections
// Columns: 2=BLK; 3=GRY; 4=PNK; 5=PUR
// Rows:    6=RED; 7=ORN; 8=YEL; 9=BRN
// Motors:  PWR=GRN; LEFT=WHT; RIGHT=BLU

byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char readKeypad()
{
  char key = keypad.getKey();
  
  if (key)
  {
    return(key);
  }
}

