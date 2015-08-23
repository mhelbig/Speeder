// Declare the VFD interface (CS, CLK, DATA)
#define VFD_CS 28
#define VFD_CLK 29
#define VFD_DATA 30

#define SIZE_OF_MESSAGE_BUFFER 3

M66004Vfd vfd(VFD_CS, VFD_CLK, VFD_DATA);

unsigned int displayMessageSequenceTimer;

typedef struct messageBuffer
{
  int active;
  int refresh;                // Note: we only use the first instance of the array [0] for the refresh flag. I know it wastes some memory.  Maybe I'll fix it later.
  char displayMessage[17];
};

messageBuffer VFD[SIZE_OF_MESSAGE_BUFFER];

void initializeVaccuumFluorescentDisplay(void) 
{
  int i;
  
  vfd.begin(16, 1);    // Prepare the VFD library for use.
  vfd.display();       // Enables normal character display.
  displayMessageSequenceTimer = millis();  // sync up the message sequencing timer
  
  for (i=0; i<SIZE_OF_MESSAGE_BUFFER; i++)        //Initialize all messages in the buffer to the inactive state and null strings
  {
    setVFDmessageInactive(i);
  }
}


void processVFDmessages(void)
{
  unsigned int i;
  
  if (VFD[0].refresh == 0) return;  // nothing to do if nothing has changed
  
  for (i=0; i<SIZE_OF_MESSAGE_BUFFER; i++)
  {
    if(VFD[i].active == 1)
    {
      vfd.clear();
      vfd.print(VFD[i].displayMessage);
      VFD[0].refresh = 0;
      Serial.print("Updating VFD: ");
      Serial.println(VFD[i].displayMessage);
      return;  // bail out as soon as we hit a message to display
    }
  }
  vfd.clear();  //Clear the VFD if there are no active messages
}

void setVFDmessageActive(unsigned int priority, char *message)  //set a message into one of the priority buffers and make it active
{
  if(priority <= SIZE_OF_MESSAGE_BUFFER)
  {
    strcpy(VFD[priority].displayMessage, message);
    VFD[0].refresh = 1;
    VFD[priority].active = 1;
  }
}  

void setVFDmessageInactive(unsigned int priority)            // make one of the previously set message priority buffers inactive
{
  if(priority <= SIZE_OF_MESSAGE_BUFFER)
  {
    VFD[0].refresh = 1;
    VFD[priority].active = 0;
    VFD[priority].displayMessage[0]=0;
  }
}

void directDisplayVFDmessage(char *message)  // Just puts a message on the display without all the complex overhead stuff
{
  vfd.clear();
  vfd.println(message);
}

