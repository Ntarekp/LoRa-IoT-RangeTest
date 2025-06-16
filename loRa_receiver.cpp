#include <SPI.h>
#include <RH_RF95.h>

// Pin and frequency definitions
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define RF95_FREQ 915.0
#define LED 13

// Radio driver instance
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Initialize Serial with timeout
  Serial.begin(115200);
  Serial.setTimeout(100);
  unsigned long startTime = millis();
  while (!Serial && millis() - startTime < 1000) delay(1);

  delay(100);
  Serial.println("LoRa RX Test!");

  // Reset LoRa module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Initialize LoRa with retries
  int retries = 3;
  while (retries > 0 && !rf95.init()) {
    Serial.println("LoRa init failed");
    retries--;
    delay(500);
  }
  if (!rf95.init()) {
    Serial.println("LoRa init failed");
    while (1);
  }
  Serial.println("LoRa init OK!");

  // Set frequency
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ, 1);

  // Configure LoRa settings
  rf95.setTxPower(23, false);
  rf95.setSignalBandwidth(125000);
  rf95.setCodingRate4(5);
  rf95.setSpreadingFactor(7);
}

void loop() {
  if (rf95.available()) {
    // Receive message
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      buf[len] = '\0';
      Serial.print("Got: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.print(rf95.lastRssi(), DEC);
      Serial.println(" dBm");

      // Send reply
      uint8_t data[] = "And hello back to you";
      if (rf95.send(data, sizeof(data))) {
        rf95.waitPacketSent();
        Serial.println("Sent reply");
      } else {
        Serial.println("Send failed");
      }
      digitalWrite(LED, LOW);
    } else {
      Serial.println("Receive failed");
    }
  }
  delay(10);
}
