```markdown
# LoRa Receiver - Serial Chat with RFM95 and RadioHead

This Arduino sketch demonstrates a simple **LoRa receiver** with interactive serial communication using the **RFM95 LoRa module** and the **RadioHead RH_RF95** driver.

---

## 📡 Features

- Listens for messages on 915 MHz LoRa band
- Prints received messages and RSSI to the Serial Monitor
- Allows sending messages from the Serial Monitor over LoRa
- Uses `RadioHead` library for LoRa communication

---

## 🛠️ Hardware Requirements

- Arduino-compatible board (e.g., Feather32u4, ATmega32u4)
- RFM95 LoRa module
- USB cable and jumper wires

---

## ⚙️ Arduino IDE Setup

### ✅ 1. Add Board Manager URL
To install support for Feather and LoRa-compatible boards:

- Open Arduino IDE
- Go to `File > Preferences`
- In **Additional Board Manager URLs**, add:

[https://adafruit.github.io/arduino-board-index/package_adafruit_index.json](https://adafruit.github.io/arduino-board-index/package_adafruit_index.json)

### ✅ 2. Install Board Packages

- Go to `Tools > Board > Boards Manager`
- Search for and install:
  - `Adafruit AVR Boards`
  - Or select the appropriate board (e.g., **ATmega32u4**, **Feather32u4**, etc.)

### ✅ 3. Install RadioHead Library

- Download the latest RadioHead ZIP file from:
[https://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.125.zip](https://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.125.zip)
- In Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library...`
- Select the downloaded ZIP file

---

## 📦 Dependencies

- [RadioHead Library](https://www.airspayce.com/mikem/arduino/RadioHead/)
- Adafruit Feather / ATmega board support

---

## 📶 Pin Configuration

| Signal      | Arduino Pin |
|-------------|-------------|
| RFM95_CS    | 8           |
| RFM95_RST   | 4           |
| RFM95_INT   | 7           |
| Frequency   | 915 MHz     |

> Note: Frequency must match your transmitter’s frequency.

---

## 🚀 Usage

1. Connect your RFM95 module as shown above.
2. Upload the sketch to your board.
3. Open the Serial Monitor at **115200 baud**.
4. Observe messages from other LoRa nodes.
5. Type a message in Serial Monitor to send it back.

---

## 🧪 Sample Output

```
LoRa radio init OK!
Set Freq to: 915.0
Received: Hello from TX
Sent: Hello from RX
```

---

## 📜 License

This project is licensed under the MIT License.

---

Happy experimenting with LoRa! 📡🛰️🌱
```