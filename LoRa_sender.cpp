#include <SPI.h>           // Include the SPI library for communication with LoRa module
#include <RH_RF95.h>       // Include the RadioHead library for the RF95 LoRa module

// Pin definitions for the LoRa module
#define RFM95_CS 8         // Chip select pin
#define RFM95_RST 4        // Reset pin
#define RFM95_INT 7        // Interrupt pin

// LoRa frequency in MHz (915 for North America, 868 for Europe, etc.)
#define RF95_FREQ 915.0    // Explicitly define as float for precision

// Create a singleton instance of the RH_RF95 driver with chip select and interrupt pins
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Maximum message length for safety
#define MAX_MESSAGE_LEN 100

void setup() {
  // Configure reset pin as output and set it high
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Start the Serial monitor at 115200 baud with timeout
  Serial.begin(115200);
  Serial.setTimeout(100); // Set timeout for Serial.readStringUntil

  // Wait for Serial monitor to open, with timeout
  unsigned long startTime = millis();
  while (!Serial && millis() - startTime < 1000) {
    delay(1); // Wait up to 1 second for Serial
  }

  delay(100);  // Short delay for stabilization
  Serial.println("LoRa Chat Initialized!");

  // Manually reset the LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Initialize the LoRa radio with retry mechanism
  int retries = 3;
  bool initSuccess = false;
  while (retries > 0 && !initSuccess) {
    if (rf95.init()) {
      initSuccess = true;
      Serial.println("LoRa radio init OK!");
    } else {
      Serial.println("LoRa radio init failed, retrying...");
      retries--;
      delay(500);
    }
  }

  if (!initSuccess) {
    Serial.println("LoRa radio init failed after retries");
    while (1); // Halt if initialization fails
  }

  // Set the desired frequency
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1); // Halt if frequency setting fails
  }
  Serial.print("Set Freq to: ");
  Serial.print(RF95_FREQ, 1); // Print with 1 decimal place
  Serial.println(" MHz");

  // Set transmit power (max 23 dBm for most modules)
  rf95.setTxPower(23, false);

  // Configure LoRa parameters for better performance
  rf95.setSignalBandwidth(125000);  // 125 kHz bandwidth
  rf95.setCodingRate4(5);          // Coding rate 4/5
  rf95.setSpreadingFactor(7);       // SF7 for better range vs. speed
}

void loop() {
  // Check if a LoRa message is available to read
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // Buffer for incoming message
    uint8_t len = sizeof(buf);            // Length of the message

    if (rf95.recv(buf, &len)) {
      if (len > MAX_MESSAGE_LEN) {
        len = MAX_MESSAGE_LEN; // Prevent buffer overflow
      }
      buf[len] = '\0'; // Null-terminate the message
      Serial.print("Received: ");
      Serial.println((char *)buf);
      Serial.print("RSSI: ");
      Serial.print(rf95.lastRssi(), DEC);
      Serial.println(" dBm");
      Serial.println("---");
    } else {
      Serial.println("Receive failed");
    }
  }

  // Check for user input with buffer overflow protection
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0 && input.length() <= MAX_MESSAGE_LEN) {
      Serial.print("Sent: ");
      Serial.println(input);

      // Send the input message via LoRa
      if (rf95.send((uint8_t *)input.c_str(), input.length())) {
        rf95.waitPacketSent();
        Serial.println("Message sent successfully!");
      } else {
        Serial.println("Failed to send message");
      }
      Serial.println("---");
    } else if (input.length() > MAX_MESSAGE_LEN) {
      Serial.println("Error: Input exceeds maximum length");
    }
  }

  // Small delay to prevent excessive CPU usage
  delay(10);
}
