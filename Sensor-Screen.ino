#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Array to store incoming serial data
unsigned char data_buffer[4] = {0};

// Integer to store distance
int distance = 0;

// Variable to hold checksum
unsigned char CS;

void setup() {
  // Set up serial monitor
  Serial.begin(115200);
  // Set up software serial port
  Serial1.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  // Run if data available
  if (Serial1.available() > 0) {
    delay(4);

    // Check for packet header character 0xff
    if (Serial1.read() == 0xff) {
      // Insert header into array
      data_buffer[0] = 0xff;
      // Read remaining 3 characters of data and insert into array
      for (int i = 1; i < 4; i++) {
        data_buffer[i] = Serial1.read();
      }

      // Compute checksum
      CS = data_buffer[0] + data_buffer[1] + data_buffer[2];

      // If checksum is valid, compose distance from data
      if (data_buffer[3] == CS) {
        distance = (data_buffer[1] << 8) + data_buffer[2];

        // Print to serial monitor
        Serial.print("distance: ");
        Serial.print(distance);
        Serial.println(" mm");

        // Update OLED display
        display.clearDisplay();  // Clear the screen before drawing new text
        display.setCursor(0, 10); // Reset cursor position
        display.print("Distance: ");
        display.print(distance);
        display.print(" mm");
        display.display();  // Show the updated text
      }
    }
  }
}
