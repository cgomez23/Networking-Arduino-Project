/* ColorDisplay - Color Beacon for use with Core Spartan system
  (c) Modern Robotics Inc. 2016  http://www.modernroboticsinc.com
      
  The Color Beacon uses an RGB LED to produce over 16 million colors. A color can be
  set by the user via programming or it can be set to RED or BLUE team colors by swiping 
  a magnet over the side of the device.
  
  This example cycles through the 8 main colors(states) the Color Beacon can be set to.
    
  Connections:
  Color Beacon(default addr 0x4C *0x26*)    = Port I2C
  *Arduino Address*
  
  NOTE* When using any Core sensor and its accompanying library, the pinMode() function
  should never be used as some sensors change or hold the mode as input/output at 
  certain times to function correctly. All mode initializations are automatically done 
  when the sensor constructor is calbeacon. i.e. CORE_TOUCH().
  
  Core Spartan language reference can be found at http://modernroboticsinc.com/spartan  
  Arduino language reference can be found at http://arduino.cc/en/Reference/HomePage  
  Windows: USE SERIAL MONITOR -> Ctrl+Shift+M
  Mac:     USE SERIAL MONITOR -> Cmd+Shift+M
-------------------------------------------------------------------------------------------*/
#include "CORE.h"
#include <string.h>
#include <stdlib.h>

char m[] = "i study you my friend slowly i study you slowly this difficult task its gain brings joy to my heart and pain by jerzy liebert";
// To regulate when the reader starts to read, create an instance of the Program Control Button
CORE_PCB PCB;

// Create an instance of class CORE_TRICOLOR named beacon on port S3
CORE_COLOR_BEACON beacon;

//chars_to_colors.insert
char alpha[] = "abcdefghijklmnopqrstuvwkyz ";

struct colorsSequences{
  char c;
  int color[3];
};

//character color sequence map
struct colorsSequences cols[27];

void setup(){

  // Initialize serial communications at 9600 baud
  Serial.begin(9600);
  
  // Set the mode and sample rate of the color Sensor to ACTIVE at 60Hz
  //color.colorSetup(color.ACTIVE, color.SIXTY);
  
  // put your setup code here, to run once:
int idx = 0;
  //int mod = 0;
  int zerothColorBit;
  int firstColorBit;
  int secondColorBit;
  for (int i = 1; i < 4; i++) {

    if (i == 3) { zerothColorBit = i + 1; }
    else { zerothColorBit = i; }

    for (int j = 1; j < 4; j++) {

      if (j == 3) { firstColorBit = j + 1; }
      else { firstColorBit = j; }

      for (int k = 1; k < 4; k++) {

        if (k == 3) { secondColorBit = k + 1; }
        else { secondColorBit = k; }

        cols[idx].c = alpha[idx];
        cols[idx].color[0] = zerothColorBit;
        cols[idx].color[1] = firstColorBit;
        cols[idx].color[2] = secondColorBit;

        idx++;

      }

    }
  }

  
  // Don't start the sensor until the PCB button is pressed.
  PCB.delayedStart();
  
}

// Function that take a character and returns the associated 
void getColorSequenceAndPost(int idx){
  int rv[3];
  //Serial.print(idx);
  for(int j = 0; j<3; j++){
    rv[j] = cols[idx].color[j];
    Serial.print(rv[j]);
    beacon.setColor(rv[j]);
    delay(80);
    beacon.setColor(7);
    delay(80);
  }
  Serial.print(" ");
}

int getIndex(char c){
  const char *ptr = strchr(alpha, c);
  int index;
  index = ptr - alpha;
  //Serial.print(index);
  return index;
}

void loop(){
  beacon.setColor(7);
  delay(1000);
  for(int i = 0; i < strlen(m); i++ ) {
    char c = m[i];
    //Serial.print(c);
    int idx = getIndex(c);
    getColorSequenceAndPost(idx);
    
  }
  beacon.setColor(7);
  delay(1000);
  Serial.print("\n");
}
