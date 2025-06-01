#include <SPI.h>           // Include the SPI library for communication with LoRa module
#include <RH_RF95.h>       // Include the RadioHead library for the RF95 LoRa module

// Pin definitions for the LoRa module
#define RFM95_CS 8         // Chip select pin
#define RFM95_RST 4        // Reset pin
#define RFM95_INT 7        // Interrupt pin

// LoRa frequency in MHz (915 for North America, 868 for Europe, etc.)
#define RF95_FREQ 915

// Create a singleton instance of the RH_RF95 driver with chip select and interrupt pins
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  // Configure reset pin as output and set it high
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Start the Serial monitor at 115200 baud
  Serial.begin(115200);
  while (!Serial) {
    delay(1); // Wait for the Serial monitor to open
  }

  delay(100);  // Short delay for stabilization
  Serial.println("LoRa Chat!");

  // Manually reset the LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Initialize the LoRa radio
  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1); // Stay here if initialization fails
  }
  Serial.println("LoRa radio init OK!");

  // Set the desired frequency
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1); // Stay here if frequency setting fails
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);

  // Set transmit power (max 23 dBm for most modules)
  // The second parameter enables the PA_BOOST pin if set to true (depends on module)
  rf95.setTxPower(23, false);
}

void loop() {
  // Check if a LoRa message is available to read
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // Buffer to store incoming message
    uint8_t len = sizeof(buf);            // Length of the message

    if (rf95.recv(buf, &len)) {           // Receive message into buffer
      buf[len] = '\0';                    // Null-terminate the message for safe printing
      Serial.print("Received: ");
      Serial.println((char *)buf);        // Print received message as a string

      // Optionally print RSSI (Received Signal Strength Indicator)
      // Serial.print("RSSI: ");
      // Serial.println(rf95.lastRssi(), DEC);

      Serial.println("***");              // Separator for readability
    } else {
      Serial.println("Receive failed");   // Print error if reception fails
    }
  }

  // Check if user input is available in the Serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read the input until newline
    input.trim();                                // Remove whitespace and newline characters

    if (input.length() > 0) {
      Serial.print("Sent: ");
      Serial.println(input);                     // Echo the message to Serial

      // Send the input message via LoRa
      rf95.send((uint8_t *)input.c_str(), input.length());
      rf95.waitPacketSent();                     // Wait until the message is fully transmitted

      // Optionally print confirmation
      // Serial.println("Message sent!");
      Serial.println("***");                     // Separator for readability
    }
  }
}
