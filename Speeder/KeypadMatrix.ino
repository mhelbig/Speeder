const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = 
{
  {'G','G','1','L'}, //  | II     | R2     | SELECT | CTR FWD
  {'G','B','1','B'}, //  | I      | L2     | LF PD  | CTR RT
  {'B','1','1','T'}, //  | A      | LF PU  | RT PD  | CTR DN
  {'B','1','C','G'}  //  | B      | LF PD  | START  | CTR LF
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

