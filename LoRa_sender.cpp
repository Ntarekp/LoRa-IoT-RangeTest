#include <SPI.h>           // Include the SPI library for communication with LoRa module
#include <RH_RF95.h>       // Include the RadioHead library for the RF95 LoRa module

// Pin definitions for the LoRa module
#define RFM95_CS 8         // Chip Select pin for LoRa
#define RFM95_RST 4        // Reset pin for LoRa
#define RFM95_INT 7        // Interrupt pin for LoRa

// LoRa frequency in MHz (set based on region)
#define RF95_FREQ 915.0    // Frequency used for communication (e.g., 915 MHz for North America)

// Create driver instance using chip select and interrupt pins
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Maximum length for messages to avoid buffer overflow
#define MAX_MESSAGE_LEN 100

void setup() {
  // Initialize reset pin
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH); // Default HIGH for LoRa reset

  // Start Serial Monitor
  Serial.begin(115200);
  Serial.setTimeout(100); // Timeout for serial input

  // Wait for serial connection to initialize (max 1 second)
  unsigned long startTime = millis();
  while (!Serial && millis() - startTime < 1000) {
    delay(1);
  }

  delay(100);  // Brief pause before continuing
  Serial.println("LoRa Chat Initialized!");

  // Manually reset LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Attempt to initialize LoRa radio, retry if needed
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

  // Stop if initialization fails after retries
  if (!initSuccess) {
    Serial.println("LoRa radio init failed after retries");
    while (1); // Infinite loop = halt
  }

  // Set radio frequency
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1); // Halt if failed
  }
  Serial.print("Set Freq to: ");
  Serial.print(RF95_FREQ, 1);
  Serial.println(" MHz");

  // Set transmission power level
  rf95.setTxPower(23, false); // 23 dBm

  // Configure additional LoRa settings
  rf95.setSignalBandwidth(125000);  // 125 kHz for standard operation
  rf95.setCodingRate4(5);           // Forward error correction: 4/5
  rf95.setSpreadingFactor(7);       // Trade-off between range & speed
}

void loop() {
  // Check for incoming LoRa message
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // Buffer to hold received message
    uint8_t len = sizeof(buf);            // Message length

    if (rf95.recv(buf, &len)) {
      if (len > MAX_MESSAGE_LEN) {
        len = MAX_MESSAGE_LEN; // Limit size to prevent overflow
      }
      buf[len] = '\0'; // Null-terminate for safe printing
      Serial.print("Received: ");
      Serial.println((char *)buf);
      Serial.print("RSSI: ");
      Serial.print(rf95.lastRssi(), DEC); // Signal strength
      Serial.println(" dBm");
      Serial.println("---");
    } else {
      Serial.println("Receive failed");
    }
  }

  // Check for user input from Serial
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read until newline
    input.trim(); // Remove extra whitespace

    // Ensure message is within allowed size
    if (input.length() > 0 && input.length() <= MAX_MESSAGE_LEN) {
      Serial.print("Sent: ");
      Serial.println(input);

      // Send message using LoRa
      if (rf95.send((uint8_t *)input.c_str(), input.length())) {
        rf95.waitPacketSent(); // Ensure message is transmitted
        Serial.println("Message sent successfully!");
      } else {
        Serial.println("Failed to send message");
      }
      Serial.println("---");
    } else if (input.length() > MAX_MESSAGE_LEN) {
      Serial.println("Error: Input exceeds maximum length");
    }
  }

  delay(10); // Prevent excessive CPU usage
}
