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
  int refresh;
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
  
  for (i=0; i<SIZE_OF_MESSAGE_BUFFER; i++)
  {
    if(VFD[i].active == 1 && VFD[i].refresh == 1)
    {
      vfd.clear();
      vfd.print(VFD[i].displayMessage);
      VFD[i].refresh = 0;
      Serial.print("Updating VFD: ");
      Serial.println(VFD[i].displayMessage);
      i = SIZE_OF_MESSAGE_BUFFER;              // jump to the end of the list
    }
  }
  Serial.print("i = ");
  Serial.println(i);
 if (i == SIZE_OF_MESSAGE_BUFFER)
 {
   vfd.clear();  //Clear the VFD if there are no active messages
 }
}

void setVFDmessageActive(unsigned int priority, char *message)  //set a message into one of the priority buffers and make it active
{
  if(priority <= SIZE_OF_MESSAGE_BUFFER)
  {
    strcpy(VFD[priority].displayMessage, message);
    VFD[priority].refresh = 1;
    VFD[priority].active = 1;
  }
}  

void setVFDmessageInactive(unsigned int priority)            // make one of the previously set message priority buffers inactive
{
  if(priority <= SIZE_OF_MESSAGE_BUFFER)
  {
    VFD[priority].refresh = 1;
    VFD[priority].active = 0;
    VFD[priority].displayMessage[0]=0;
  }
}

