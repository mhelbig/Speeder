// these functions use the button library to handle debouncing, edge detection, and hold time procesing of the digital inputs.
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs

// define and declare the passenger button inputs:
#define PASSENGER_PULLUP true            //use the Arduino's internal pullup resistors
#define PASSENGER_INVERT true            //high state = button not pressed
#define BUTTON_PASSENGER_GOOD_GUY   A8
#define BUTTON_PASSENGER_BAD_GUY    A9
#define BUTTON_PASSENGER_WEAPON_1   A10
#define BUTTON_PASSENGER_WEAPON_2   A11
#define BUTTON_PASSENGER_WEAPON_3   A12
#define BUTTON_PASSENGER_WEAPON_4   A13
#define BUTTON_PASSENGER_WEAPON_5   A14
#define BUTTON_PASSENGER_WEAPON_6   A15
Button passengerGoodGuy(BUTTON_PASSENGER_GOOD_GUY, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerBadGuy(BUTTON_PASSENGER_BAD_GUY,   PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon1(BUTTON_PASSENGER_WEAPON_1, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon2(BUTTON_PASSENGER_WEAPON_2, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon3(BUTTON_PASSENGER_WEAPON_3, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon4(BUTTON_PASSENGER_WEAPON_4, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon5(BUTTON_PASSENGER_WEAPON_5, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon6(BUTTON_PASSENGER_WEAPON_6, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);

//define and declare the CB Mic Key input:
#define CB_MIC_PULLUP true           //use the Arduino's internal pullup resistors
#define CB_MIC_INVERT true           //high state = button not pressed
#define BUTTON_CB_MIC_KEY           9
#define CB_MINIMUM_TALK_TIME        3000  //time in milliseconds
Button cbMICkey(BUTTON_CB_MIC_KEY, CB_MIC_PULLUP, CB_MIC_INVERT, DEBOUNCE_MS);

//define and declare the R2D2 motion sensor input:
#define R2D2_SENSOR_MOTION_PULLUP false           //do not use the Arduino's internal pullup resistors
#define R2D2_SENSOR_MOTION_INVERT false           //high state = motion detected
#define SENSOR_MOTION_R2D2          35
Button sensorMotionR2D2(SENSOR_MOTION_R2D2, R2D2_SENSOR_MOTION_PULLUP, R2D2_SENSOR_MOTION_INVERT, DEBOUNCE_MS);

char input;

char scanForUserInput(void)
{
// NOTE: The order of these functions determines thier priority, as the function returns as soon as a valid input is found
  
// Check the serial console for input.  This will only be used for testing and debugging.
  if (Serial.available() > 0) return(toupper(Serial.read()));

// Then look for input from the steering wheel
  input = scanSteeringWheel();
  if (input) return(input);
  
// Then look for input from the passenger buttons
  input = scanPassengerButtons();
  if (input) return (input);

// Then look for input from the CB mic:
  input = scanCBmicKey();
  if (input) return(input);
  
// Maybe the kids aren't in the cockpit, look for input from R2D2's motion sensor:
  input = scanR2D2motionSensor();
  if (input) return(input);

// Return a null if none of the user inputs had any action
  else return(0);
}

// Passenger panel inputs:
char scanPassengerButtons(void)
{
  //Read all the buttons (this needs to be called often)
  passengerGoodGuy.read();
  passengerBadGuy.read();
  passengerWeapon1.read();
  passengerWeapon2.read();
  passengerWeapon3.read();
  passengerWeapon4.read();
  passengerWeapon5.read();
  passengerWeapon6.read();
  
  if(passengerGoodGuy.wasPressed())
    return('G');
  
  if(passengerBadGuy.wasPressed())
    return('B');
  
  if(passengerWeapon1.wasPressed())
    return('T');
  
  if(passengerWeapon2.wasPressed())
    return('L');
  
  if(passengerWeapon3.wasPressed())
    return('P');
  
  if(passengerWeapon4.wasPressed())
    return('E');
  
  if(passengerWeapon5.wasPressed())
    return('N');
  
  if(passengerWeapon6.wasPressed())
    return('S');
    
  return(0);
}

// CB microphone input:
char scanCBmicKey(void)
{
  //Read the buttons (this needs to be called often)
  cbMICkey.read();
  
  if(cbMICkey.pressedFor(CB_MINIMUM_TALK_TIME))
    return('C');
}

char scanR2D2motionSensor(void)
{
  //Read the buttons (this needs to be called often)
  sensorMotionR2D2.read();
  
  if(sensorMotionR2D2.wasPressed())
    return('R');
}
  
