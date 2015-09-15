// these functions use the button library to handle debouncing, edge detection, and hold time procesing of the digital inputs.
#define DEBOUNCE_MS 20     //debounce time = 20 mS for all inputs

//HACK - setup a bogus button since the first one setup gets stuck on for some unknown reason
Button bogusHackButton(A6, true, true, DEBOUNCE_MS);

//define and declare the CB Mic Key input:
#define CB_MIC_PULLUP true           //use the Arduino's internal pullup resistors
#define CB_MIC_INVERT true           //high state = button not pressed
#define CB_MINIMUM_TALK_TIME        1500  //time in milliseconds
Button cbMICkey(9, CB_MIC_PULLUP, CB_MIC_INVERT, DEBOUNCE_MS);

//define and declare the R2D2 motion sensor input:
#define R2D2_SENSOR_MOTION_PULLUP false           //do not use the Arduino's internal pullup resistors
#define R2D2_SENSOR_MOTION_INVERT false           //high state = motion detected
Button sensorMotionR2D2(35, R2D2_SENSOR_MOTION_PULLUP, R2D2_SENSOR_MOTION_INVERT, DEBOUNCE_MS);

// define and declare the passenger button inputs:
#define PASSENGER_PULLUP true            //use the Arduino's internal pullup resistors
#define PASSENGER_INVERT true            //high state = button not pressed

Button passengerGoodGuy(A8,  PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerBadGuy( A9,  PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon1(A10, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon2(A11, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon3(A12, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon4(A13, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon5(A14, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);
Button passengerWeapon6(A15, PASSENGER_PULLUP, PASSENGER_INVERT, DEBOUNCE_MS);

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
  {
    return(GOOD_GUYS_BUTTON);
  }
  
  if(passengerBadGuy.wasPressed())
  {
    return(BAD_GUYS_BUTTON);
  }
  
  if(passengerWeapon1.wasPressed())
  {
    return(PASSENGER_BUTTON_1);
  }
  
  if(passengerWeapon2.wasPressed())
  {
    return(PASSENGER_BUTTON_2);
  }
  
  if(passengerWeapon3.wasPressed())
  {
    return(PASSENGER_BUTTON_3);
  }
  
  if(passengerWeapon4.wasPressed())
  {
    return(PASSENGER_BUTTON_4);
  }
  
  if(passengerWeapon5.wasPressed())
  {
    return(PASSENGER_BUTTON_5);
  }
  
  if(passengerWeapon6.wasPressed())
  {
    return(PASSENGER_BUTTON_6);
  }
  
  return(0);
}

// CB microphone input:
char scanCBmicKey(void)
{
  static char buttonHeld = 0;

  //Read the buttons (this needs to be called often)
  cbMICkey.read();
  
  if(cbMICkey.pressedFor(CB_MINIMUM_TALK_TIME))
  {
    buttonHeld = 1;
  }
  
  if(cbMICkey.wasReleased() && buttonHeld == 1)
  {
    buttonHeld = 0;
    return(CB_MIC_PUSHBUTTON);
  }
}

char scanR2D2motionSensor(void)
{
  //Read the buttons (this needs to be called often)
  sensorMotionR2D2.read();
  
  if(sensorMotionR2D2.wasPressed())
    return(R2D2);
}
  
