const byte MOTOR_LF = 44;
const byte MOTOR_RT = 45;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = 
{
  {'B','C','M','1'}, //  | B      | L2     | SELECT | CTR FWD
  {'G','R','E','1'}, //  | A      | R2     | LF PD  | CTR RT
  {'T','L','N','1'}, //  | I      | LF PU  | RT PD  | CTR DN
  {'W','P','S','1'}  //  | II     | RT PU  | START  | CTR LF
};

// Mad Catz MC2 Steering wheel wire color matrix connections
// Columns: 2=BLK; 3=GRY; 4=PNK; 5=PUR
// Rows:    6=RED; 7=ORN; 8=YEL; 9=BRN
// Motors:  PWR=GRN; LEFT=WHT; RIGHT=BLU

byte colPins[COLS] = {36, 37, 38, 39}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {40, 41, 42, 43}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char scanSteeringWheel()
{
  char key = keypad.getKey();
  
  if (key)
  {
    return(key);
  }
}

void SetVibratorMotorLeft (char value)
{
  analogWrite(MOTOR_LF, value);
}

void SetVibratorMotorRight (char value)
{
  analogWrite(MOTOR_RT, value);
}

