
/* Reader - Calibrate and Read with Core Spartan Controller
  Michael Bujard, Charles Ramsay, Carlos Gomez Team
  
  STEP 1: Calibration - Color Sensor for use with the Core Spartan Controller
  
        
  The Color Sensor uses various channels of color to determine the approximate color of an 
  object. The colors we are searching for are Red (1), Green (2), Blue (4), and White (7).
  Best results will be attained in a well-lit area and the object no more than 7cm away from the sensor. 
  
  This calibrates the White (7) and black (OFF/0) balance values used when calculating normalized 
  color readings. Upload the sketch to your Spartan, connect your touch and color sensors, open 
  the serial monitor using CTRL+SHIFT+M, and follow the prompts on your screen. Refer to the 
  documentation for more information on calibrating and descriptions of the returned values.

  STEP 2: Read - 
  
  Connections:
  Color Sensor(default addr 0x3C *0x1E*)  = Port I2C
  Touch Sensor(TOUCH)                     = Port D7
  *Arduino Address*
  
  NOTE* When using any Core sensor and its accompanying library, the pinMode() function
  should never be used as some sensors change or hold the mode as input/output at 
  certain times to function correctly. All mode initializations are automatically done 
  when the sensor constructor is called. i.e. CORE_TOUCH(). 
    
  Core Spartan language reference can be found at http://modernroboticsinc.com/spartan  
  Arduino language reference can be found at http://arduino.cc/en/Reference/HomePage  
  Windows: USE SERIAL MONITOR -> Ctrl+Shift+M
  Mac:     USE SERIAL MONITOR -> Cmd+Shift+M
-------------------------------------------------------------------------------------------*/

#include "CORE.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

// To regulate when the reader starts to read, create an instance of the Program Control Button
CORE_PCB PCB;

// for testing/debugging
// Create an instance of class CORE_TRICOLOR named beacon on port S3
CORE_COLOR_BEACON beacon;

// Create an instance of the class CORE_COLOR_SENSOR named color using the default I2C address
CORE_COLOR_SENSOR color;

// Create an instance of the class CORE_DIGITAL named push on port D7
CORE_DIGITAL push(7);

// message to fill and then print
//String line = " ";
//int lLength = 0; // when lLength gets to 30, clear line and increment count again

//chars_to_colors.insert
char alpha[] = "abcdefghijklmnopqrstuvwkyz ";

void setup() {
  
  // Initialize serial communications at 9600 baud
  Serial.begin(9600);

  // Set the mode of the color Sensor to PASSIVE at 60Hz 
  color.colorSetup(color.PASSIVE, color.SIXTY);

  // Don't start the sensor until the PCB button is pressed.
  PCB.delayedStart();
  
}

String letters[3][3][3] = {
    {
        { "a", "b", "c" },
        { "d", "e", "f" },
        { "g", "h", "i" }
    },
    {
        { "j", "k", "l" },
        { "m", "n", "o" },
        { "p", "q", "r" }
    },
    {
        { "s", "t", "u" },
        { "v", "w", "x" },
        { "y", "z", " " }
    }
};

/*
User function decides what color a particular beacon light is
*/

int getRGBWColor() {

  // 10 or 9 appears to be consistently RED (1)
  // 15 or 6 appears to be consistently GREEN (2)
  // 1 or 2 appears to be consistently BLUE (4)
  // 11 appears to be consistently WHITE (7)
  int colorNumber = color.getColorNumber();

  if (colorNumber == 10 || colorNumber == 9) {
    //Serial.println("coler value is 10 or 9");
    return 1;  
  }
  if (colorNumber == 15 || colorNumber == 6) {
    //Serial.println("color value is 15 or 6");
    return 2;  
  }
  if (colorNumber == 1 || colorNumber == 2) {
    //Serial.println("color value is 1 or 2");
    return 4;  
  }
  if (colorNumber == 11) {
    //Serial.println("color value is 11");
    return 7;  
  }
  
}

/*
User-defined method takes two parameters:
parameter 1: an int, color value,
1,2, or 4, that is, Red, Green, or Blue.
Parameter 2: int index, which is the index of the array
into which the return value is inserted.

This function returns a bit of information into the array that
describes, as a 3-bit base-3 number, the character encoded by
a 3-color sequence

i.e. return an index into the letters[][][] 3-dimensional array
*/
int fromColorGetBit(int colVal) {

  if (colVal == 4) {  // if blue, then index is 2
      return colVal - 2;
  }
  // if red, then corresponding index is 1 - 1 = 0 or 
  //if green, then corresponding index is 2 - 1 = 1
  return colVal - 1; 
  
}

/*
 utility method to print the letter that is in letters[][][]
 from the elements of characteer[] that correspond to the 
 indices of the letters[][][] 3-dimensional array.

 i.e. get the letter from a 3-bit bitstring, base 3, where
 all bits are subtracted by 1 because they are mapped to indices
 to a 3-d array.

 this function makes sure that the index outside the while loop is 
 currently 2, because this indicates that the reader robot has finished
 reading a full character.
*/

void handleCharacterPrint(int charArr[3], int idx) {

  //line += (String)letters[charArr[0]][charArr[1]][charArr[2]];
  //lLength++;

  if (idx == 2) {  // && lLength >= 30
    Serial.print(letters[charArr[0]][charArr[1]][charArr[2]]);
  }
  
}


int index = 0;
int character[3]; // initialize the characteer to be read, then printed
int colorValue = 7;  // initialize color value to 7

void loop() {
  
  /*
  In the beginning of this program, we assume that the color is 
  WHITE, because we programmed the reader to begin its message
  with a WHITE light, delayed for 1000 ms.
  
  We wait for the light to not be white...
  */
  while (colorValue == 7) {  // while it's white, loop and wait, doing nothing but waiting for a value
    colorValue = getRGBWColor();
  }

  // outside the while loop, we know that we have transitioned to another color,
  // that is, another bit of info corresponding to some 3-bit, base-3 bitstring
  // that denotes a character
  
  // just to be sure, wait 20 ms before taking the color value.
  // This ensures that there is no possibility of erroneous color reading due to 
  // the transition from white to another color.
  delay(20);  

  // we have waited, so now get the color value again (should be 1, 2, or 4)
  colorValue = getRGBWColor();
  // exit the while loop when the colorValue is not white.
  
  // the color is no longer white, so we have a new color.
  // first, save the color. Next, check if the index is 2.
  // if it's 2, print the character.
  // at the end, increase the index to get ready to add the next bit of 
  // information to the character[] array.

  // save the color as a bit of information that goes to the character[] array of 3 bits
  int colorBit = fromColorGetBit(colorValue);
  character[index] = colorBit;
    
  handleCharacterPrint(character, index);

  // increment index. If it had been 2, reset to 0 to begin extracting, from the color readings,
  // information about the next character.
  index = (index + 1) % 3;

  while (colorValue != 7) {  // loop and wait for white again, after reading RGB color from beacon
    colorValue = getRGBWColor();
  };

  // now the color is definitely white, because  getRGBWColor() returns a value that is not either
  // 1, 2, 4, or errno
  // errno is any number that is not 1, 2, 4, or 7, which was most likely returned because the
  // sensor was not in the optimal reading window (about a hand's width apart from the beacon)
  
}
