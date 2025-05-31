# 📡 LoRa Serial Chat Receiver with RFM95 & RadioHead

Welcome to your **LoRa Receiver** project! This Arduino sketch enables interactive serial chat via LoRa using the **RFM95 module** and the **RadioHead RH_RF95** driver library.



## 🌟 Features

- **Real-Time LoRa Reception:** Listens for LoRa messages on the 915 MHz ISM band.
- **Serial Monitor Chat:** Display incoming messages & RSSI; send replies from your computer.
- **Plug & Play:** Designed for easy setup with Adafruit Feather32u4, ATmega32u4, and other Arduino-compatible boards.
- **Reliable Communication:** Leveraging the proven [RadioHead Library](https://www.airspayce.com/mikem/arduino/RadioHead/).



## 🛠️ Hardware Required

- **Arduino-compatible board:** (e.g., Adafruit Feather32u4, ATmega32u4)
- **RFM95 LoRa module**
- **USB cable** and **jumper wires**

##  Arduino IDE Setup

### 1️⃣ Add Board Manager URL

1. Open **Arduino IDE**.
2. Go to **File → Preferences**.
3. In **Additional Board Manager URLs**, add:  
   ```
   https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
   ```

### 2️⃣ Install Board Packages

- Navigate to **Tools → Board → Boards Manager**.
- Search for and install:
  - `Adafruit AVR Boards` (or your specific board, e.g., ATmega32u4, Feather32u4, etc.)

### 3️⃣ Install RadioHead Library

- Download the latest RadioHead ZIP:  
  [RadioHead-1.125.zip](https://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.125.zip)
- In Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library...**
- Select the downloaded ZIP file.


## 📦 Dependencies

- [RadioHead Library](https://www.airspayce.com/mikem/arduino/RadioHead/)
- Adafruit Feather / ATmega board support



## 🔌 Pin Configuration

| **Signal**   | **Arduino Pin** |
|--------------|-----------------|
| `RFM95_CS`   | 8               |
| `RFM95_RST`  | 4               |
| `RFM95_INT`  | 7               |
| **Frequency**| 915 MHz         |

> **Note:** Ensure the frequency matches your LoRa transmitter.



## 🚀 Getting Started

1. **Connect** your RFM95 module to your Arduino as shown in the pin table.
2. **Upload** the sketch to your board.
3. **Open** the Serial Monitor (**115200 baud**).
4. **Watch** for incoming LoRa messages.
5. **Type** into the serial monitor and press Enter to send a message via LoRa.



## 🧪 Example Output

```
LoRa radio init OK!
Set Freq to: 915.0
Received: Hello from TX [RSSI: -45]
Sent: Hello from RX
```



## 📝 License

MIT License



##  Tips

- Make sure your antenna is connected before powering up!
- Use identical frequency settings on all communicating LoRa nodes.
- For troubleshooting, check wiring and ensure driver libraries are properly installed.

