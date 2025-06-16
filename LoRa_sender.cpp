#include <SPI.h>           // SPI library used for communication with the LoRa module
#include <RH_RF95.h>       // RadioHead library specific to the RF95 LoRa transceiver

// Define wiring pins connected to the LoRa module
#define RFM95_CS 8         // Chip Select pin (NSS)
#define RFM95_RST 4        // Reset pin (RST)
#define RFM95_INT 7        // Interrupt pin (DIO0)

// Set the LoRa operating frequency in MHz
#define RF95_FREQ 915.0    // 915 MHz for North America (make sure both sender and receiver use the same frequency)

// Create an instance of the RF95 driver with CS and interrupt pins
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Define maximum allowed message length for transmission
#define MAX_MESSAGE_LEN 100

void setup() {
  // Set the reset pin as output and initialize it to HIGH (not in reset state)
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Begin Serial communication at 115200 baud
  Serial.begin(115200);
  Serial.setTimeout(100); // Set a timeout for reading input from Serial

  // Wait briefly for the Serial Monitor to open (max 1 second)
  unsigned long startTime = millis();
  while (!Serial && millis() - startTime < 1000) {
    delay(1);
  }

  delay(100);  // Short delay to ensure Serial is stable
  Serial.println("LoRa Chat Initialized!");

  // Perform a manual hardware reset on the LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Try to initialize the LoRa module with up to 3 retries
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

  // Stop execution if initialization failed
  if (!initSuccess) {
    Serial.println("LoRa radio init failed after retries");
    while (1);
  }

  // Set the desired frequency for LoRa communication
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1); // Halt if frequency cannot be set
  }
  Serial.print("Set Freq to: ");
  Serial.print(RF95_FREQ, 1); // Display frequency with 1 decimal
  Serial.println(" MHz");

  // Set transmission power to 23 dBm (maximum for most modules)
  rf95.setTxPower(23, false);

  // Configure LoRa radio parameters for performance tuning
  rf95.setSignalBandwidth(125000);  // 125 kHz bandwidth
  rf95.setCodingRate4(5);           // Forward error correction coding rate 4/5
  rf95.setSpreadingFactor(7);       // Spreading Factor 7 balances speed and range
}

void loop() {
  // Check if a LoRa message has been received
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // Buffer to store incoming message
    uint8_t len = sizeof(buf);            // Length of received message

    if (rf95.recv(buf, &len)) {
      if (len > MAX_MESSAGE_LEN) {
        len = MAX_MESSAGE_LEN; // Clamp message to max length to prevent overflow
      }
      buf[len] = '\0'; // Null-terminate the message for printing
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

  // Check if user entered a message via Serial
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove leading/trailing whitespace

    if (input.length() > 0 && input.length() <= MAX_MESSAGE_LEN) {
      Serial.print("Sent: ");
      Serial.println(input);

      // Send the input string as a LoRa packet
      if (rf95.send((uint8_t *)input.c_str(), input.length())) {
        rf95.waitPacketSent(); // Wait until the packet is fully transmitted
        Serial.println("Message sent successfully!");
      } else {
        Serial.println("Failed to send message");
      }
      Serial.println("---");
    } else if (input.length() > MAX_MESSAGE_LEN) {
      Serial.println("Error: Input exceeds maximum length");
    }
  }

  // Brief delay to avoid maxing out the processor
  delay(10);
}
