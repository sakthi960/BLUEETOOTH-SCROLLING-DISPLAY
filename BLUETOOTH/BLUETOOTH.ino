#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_NeoMatrix.h>   // NeoPixel matrix library
#include <Adafruit_NeoPixel.h>    // NeoPixel library
#include <SoftwareSerial.h>       // Software serial library
#include <EEPROM.h>               // EEPROM library

#define PIN 6  // The pin connected to the data input of the NeoPixel matrix
#define MATRIX_WIDTH  32
#define MATRIX_HEIGHT 7
#define EEPROM_TEXT_START 0      // Starting address for text storage in EEPROM
#define EEPROM_COLOR_R 101       // EEPROM address for storing Red color value
#define EEPROM_COLOR_G 102       // EEPROM address for storing Green color value
#define EEPROM_COLOR_B 103       // EEPROM address for storing Blue color value
#define MAX_TEXT_LENGTH 45     // Maximum length for display text

SoftwareSerial BTSerial(0, 1); // RX, TX pins for Bluetooth module

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

uint16_t textColor;
String displayText = ""; // Initial display text
int brightness = 200; // Initial brightness
int scrollDelay = 100; // Initial delay for scrolling

// Function to read text from EEPROM
void readTextFromEEPROM() {
  char storedText[MAX_TEXT_LENGTH + 1]; // +1 for null terminator
  for (int i = 0; i < MAX_TEXT_LENGTH; i++) {
    storedText[i] = EEPROM.read(EEPROM_TEXT_START + i);
    if (storedText[i] == '\0') break; // Stop if end of string is reached
  }
  storedText[MAX_TEXT_LENGTH] = '\0'; // Ensure null termination
  displayText = String(storedText);
}

// Function to write text to EEPROM
void writeTextToEEPROM(const String& text) {
  for (int i = 0; i < MAX_TEXT_LENGTH; i++) {
    if (i < text.length()) {
      EEPROM.write(EEPROM_TEXT_START + i, text[i]);
    } else {
      EEPROM.write(EEPROM_TEXT_START + i, '\0'); // Null terminate the remaining space
    }
  }
}

// Function to read color from EEPROM
void readColorFromEEPROM(uint8_t &r, uint8_t &g, uint8_t &b) {
  r = EEPROM.read(EEPROM_COLOR_R);
  g = EEPROM.read(EEPROM_COLOR_G);
  b = EEPROM.read(EEPROM_COLOR_B);
}

// Function to write color to EEPROM
void writeColorToEEPROM(uint8_t r, uint8_t g, uint8_t b) {
  EEPROM.write(EEPROM_COLOR_R, r);
  EEPROM.write(EEPROM_COLOR_G, g);
  EEPROM.write(EEPROM_COLOR_B, b);
}

void setup() {
  Serial.begin(9600);   // Start Serial Monitor at 9600 baud rate
  BTSerial.begin(9600); // Start Bluetooth serial at 9600 baud rate

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness); // Set initial brightness

  readTextFromEEPROM(); // Load text from EEPROM
  if (displayText.length() == 0) {
    displayText = "WELCOME"; // Set default text if EEPROM is empty
  }

  uint8_t r, g, b;
  readColorFromEEPROM(r, g, b); // Load color from EEPROM
  textColor = matrix.Color(r, g, b); // Set initial color
  matrix.setTextColor(textColor); // Set text color

  Serial.println("Bluetooth Test Initialized");
  Serial.print("Initial display text: ");
  Serial.println(displayText);
}

void loop() {
  // Check if data is available from the Bluetooth module
  if (BTSerial.available()) {
    String receivedText = BTSerial.readStringUntil('\n'); // Read incoming Bluetooth data until newline
    if (receivedText.length() > 0) {
      if (receivedText.startsWith("BRIGHTNESS:")) {
        int newBrightness = receivedText.substring(11).toInt(); // Extract brightness value
        if (newBrightness >= 0 && newBrightness <= 255) {
          brightness = newBrightness;
          matrix.setBrightness(brightness); // Update brightness
          Serial.print("Brightness set to: ");
          Serial.println(brightness);
        }
      } else if (receivedText.startsWith("COLOR:")) {
        int comma1 = receivedText.indexOf(',');
        int comma2 = receivedText.indexOf(',', comma1 + 1);
        if (comma1 > 6 && comma2 > comma1) {
          int r = receivedText.substring(6, comma1).toInt();
          int g = receivedText.substring(comma1 + 1, comma2).toInt();
          int b = receivedText.substring(comma2 + 1).toInt();
          if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            textColor = matrix.Color(r, g, b);
            matrix.setTextColor(textColor); // Update text color
            writeColorToEEPROM(r, g, b); // Save color to EEPROM
            Serial.print("Color set to: R=");
            Serial.print(r);
            Serial.print(", G=");
            Serial.print(g);
            Serial.print(", B=");
            Serial.println(b);
          }
        }
      } else if (receivedText.startsWith("DELAY:")) {
        int newDelay = receivedText.substring(6).toInt(); // Extract delay value
        if (newDelay >= 0) {
          scrollDelay = newDelay;
          Serial.print("Scroll delay set to: ");
          Serial.println(scrollDelay);
        }
      } else {
        Serial.print("Received Text: ");
        Serial.println(receivedText); // Print received text to Serial Monitor
        displayText = receivedText; // Update display text
        writeTextToEEPROM(displayText); // Save the new text to EEPROM
      }
    }
  }

  // Calculate text width manually
  int16_t textWidth = displayText.length() * 6; // Approximate width (6 pixels per character)

  // Update the NeoPixel matrix display with scrolling text
  static int16_t x = matrix.width(); // Start position for text

  matrix.fillScreen(0); // Clear the screen
  matrix.setCursor(x, 0); // Set cursor position
  matrix.print(displayText); // Print text to matrix

  if (--x < -textWidth) { // Check if text has completely scrolled out
    x = matrix.width(); // Reset text position
  }

  matrix.show(); // Display the updated text on the matrix

  // Apply the user-defined delay
  delay(scrollDelay);
}
