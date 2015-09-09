const byte MOTOR_LF = 44;
const byte MOTOR_RT = 45;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = 
{
  {WHEEL_B,  WHEEL_L2,    WHEEL_SELECT,  WHEEL_CTR_FW}, //  | B      | L2     | SELECT | CTR FWD
  {WHEEL_A,  WHEEL_R2,    WHEEL_LF_PD,   WHEEL_CTR_RT}, //  | A      | R2     | LF PD  | CTR RT
  {WHEEL_I,  WHEEL_LF_PU, WHEEL_RT_PD,   WHEEL_CTR_DN}, //  | I      | LF PU  | RT PD  | CTR DN
  {WHEEL_II, WHEEL_RT_PU, WHEEL_START,   WHEEL_CTR_LF}  //  | II     | RT PU  | START  | CTR LF
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

