// Declare the VFD interface (CS, CLK, DATA)
#define VFD_CS 28
#define VFD_CLK 29
#define VFD_DATA 30

M66004Vfd vfd(VFD_CS, VFD_CLK, VFD_DATA);

unsigned int displayMessageSequenceTimer;

void initializeVaccuumFluorescentDisplay(void) 
{
  vfd.begin(16, 1);    // Prepare the VFD library for use.
  vfd.display();       // Enables normal character display.
  displayMessageSequenceTimer = millis();  // sync up the message sequencing timer
}


void processVFDmessages(void)
{
  if(millis() - displayMessageSequenceTimer > 1000)    //sequence through messages on a timed interval
  {  
    displayMessageSequenceTimer = millis();
    // parse out the portion of the message that will be displayed at this point, if multiple lines
    vfd.clear();
    vfd.print(message);
  
  
}

void displayMessageVFD(char *message)
{
  // need to decide if we need to parse the message here, or not
  //also, we need to initialize the message parser to the first item
  //and, we might need to decide if we need to hold the queue of message here, or somewhere else
}  
