#include "arduino_secrets.h"
#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "LCT"
  https://create.arduino.cc/cloud/things/2d9915c2-3202-4f3c-a313-ea8518deba17 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float distance;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

// Array to store incoming serial data
unsigned char data_buffer[4] = {0};

// Variable to hold checksum
unsigned char CS;

void setup() {
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Set up serial monitor
  Serial.begin(115200);
  // Set up software serial port
  Serial1.begin(9600);

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Your code here 
    // Run if data available
  if (Serial1.available() > 0) {

    // Check for packet header character 0xff
    if (Serial1.read() == 0xff) {
      // Insert header into array
      data_buffer[0] = 0xff;
      // Read remaining 3 characters of data and insert into array
      for (int i = 1; i < 4; i++) {
        data_buffer[i] = Serial1.read();
      }

      //Compute checksum
      CS = data_buffer[0] + data_buffer[1] + data_buffer[2];
      // If checksum is valid compose distance from data
      if (data_buffer[3] == CS) {
        distance = (data_buffer[1] << 8) + data_buffer[2];
        // Print to serial monitor
        Serial.print("distance: ");
        Serial.print(distance);
        Serial.println(" mm");
      }
    }
  }
  
  
}
