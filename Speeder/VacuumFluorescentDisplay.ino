// Declare the VFD interface (CS, CLK, DATA)
#define VFD_CS 28
#define VFD_CLK 29
#define VFD_DATA 30

M66004Vfd vfd(VFD_CS, VFD_CLK, VFD_DATA);

void initializeVaccuumFluorescentDisplay(void) 
{
  // Prepare the VFD library for use.
  vfd.begin(16, 1);
 
  // Enables normal character display.
  vfd.display();
}

void displayMessageVFD(char *message)
{
  vfd.print(message);
}  