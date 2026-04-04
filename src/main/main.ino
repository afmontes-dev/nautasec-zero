#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.hpp>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BTN_UP    18
#define BTN_DOWN  19
#define BTN_OK    23

#define IR_RECEIVE_PIN 15
#define IR_SEND_PIN    4

#define RDM6300_RX    16
#define LED_RFID       2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* menuItems[] = {
  "Sub-GHz",
  "NFC",
  "RFID 125kHz",
  "Infrared",
  "WiFi"
};
const int menuSize = 5;
int currentItem = 0;
bool inSubmenu = false;
int currentModule = -1;

// IR storage
uint32_t lastIRCode = 0;
uint16_t lastIRProtocol = 0;

// RFID storage
String lastRFIDCard = "";

// NFC storage
String lastNFCCard = "";
String lastNFCType = "";

// Sub-GHz storage
String lastRFSignal = "";
int lastRFFreq = 433;

// ─── DISPLAY FUNCTIONS ────────────────────────────────────

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== NautaSec Zero ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  for (int i = 0; i < menuSize; i++) {
    if (i == currentItem) {
      display.fillRect(0, 14 + i * 10, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(4, 15 + i * 10);
    display.print(menuItems[i]);
  }
  display.display();
}

void drawIRMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Infrared ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.println("UP:   Capture");
  display.setCursor(0, 24);
  display.println("DOWN: Replay");
  display.setCursor(0, 34);
  if (lastIRCode != 0) {
    display.print("Code: 0x");
    display.println(lastIRCode, HEX);
  } else {
    display.println("No code captured");
  }
  display.setCursor(0, 54);
  display.println("OK: Back");
  display.display();
}

void drawRFIDMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== RFID 125kHz ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.println("Scanning...");
  display.setCursor(0, 24);
  display.println("Place card near");
  display.setCursor(0, 34);
  display.println("reader.");
  if (lastRFIDCard != "") {
    display.setCursor(0, 44);
    display.print("Last: ");
    display.println(lastRFIDCard);
  }
  display.setCursor(0, 54);
  display.println("OK: Back");
  display.display();
}

void drawNFCMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== NFC 13.56MHz ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.println("UP: Scan card");
  display.setCursor(0, 24);
  display.println("DOWN: Show last");
  if (lastNFCCard != "") {
    display.setCursor(0, 34);
    display.print("UID: ");
    display.println(lastNFCCard);
    display.setCursor(0, 44);
    display.print("Type: ");
    display.println(lastNFCType);
  } else {
    display.setCursor(0, 34);
    display.println("No card scanned");
  }
  display.setCursor(0, 54);
  display.println("OK: Back");
  display.display();
}

void simulateNFCCard() {
  lastNFCCard = "04 A3 2B 1C";
  lastNFCType = "MIFARE Classic";

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== NFC Detected ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.print("UID: ");
  display.println(lastNFCCard);
  display.setCursor(0, 24);
  display.print("Type: ");
  display.println(lastNFCType);
  display.setCursor(0, 34);
  display.println("Sector 0 key:");
  display.setCursor(0, 44);
  display.println("FF FF FF FF FF FF");
  display.setCursor(0, 54);
  display.println("Saved.");
  display.display();
  delay(2000);
  drawNFCMenu();
}

void drawSubGHzMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Sub-GHz ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.println("UP:   Capture");
  display.setCursor(0, 24);
  display.println("DOWN: Replay");
  display.setCursor(0, 34);
  display.print("Freq: ");
  display.print(lastRFFreq);
  display.println(" MHz");
  if (lastRFSignal != "") {
    display.setCursor(0, 44);
    display.print("Sig: ");
    display.println(lastRFSignal);
  } else {
    display.setCursor(0, 44);
    display.println("No signal captured");
  }
  display.setCursor(0, 54);
  display.println("OK: Back");
  display.display();
}

void simulateRFCapture() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Capturing RF ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.print("Freq: ");
  display.print(lastRFFreq);
  display.println(" MHz");
  display.setCursor(0, 24);
  display.println("Scanning...");
  display.display();
  delay(2000);

  lastRFSignal = "A1B2C3D4";

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("== Captured! ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.print("Freq: ");
  display.print(lastRFFreq);
  display.println(" MHz");
  display.setCursor(0, 24);
  display.print("Signal: ");
  display.println(lastRFSignal);
  display.setCursor(0, 34);
  display.println("Modulation: OOK");
  display.setCursor(0, 44);
  display.println("Saved.");
  display.display();
  delay(2000);
  drawSubGHzMenu();
}

void simulateRFReplay() {
  if (lastRFSignal == "") {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("No signal to replay.");
    display.println("Capture first.");
    display.display();
    delay(2000);
    drawSubGHzMenu();
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Replaying RF ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 14);
  display.print("Freq: ");
  display.print(lastRFFreq);
  display.println(" MHz");
  display.setCursor(0, 24);
  display.print("Signal: ");
  display.println(lastRFSignal);
  display.setCursor(0, 34);
  display.println("Transmitting...");
  display.display();
  delay(2000);

  display.setCursor(0, 44);
  display.println("Done.");
  display.display();
  delay(1000);
  drawSubGHzMenu();
}

// ─── IR FUNCTIONS ─────────────────────────────────────────

void captureIR() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Capturing IR ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Point remote and");
  display.setCursor(0, 30);
  display.println("press any button...");
  display.display();

  IrReceiver.resume();
  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (IrReceiver.decode()) {
      lastIRCode = IrReceiver.decodedIRData.decodedRawData;
      lastIRProtocol = (uint16_t)IrReceiver.decodedIRData.protocol;
      IrReceiver.resume();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("== Captured! ==");
      display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
      display.setCursor(0, 20);
      display.print("Code: 0x");
      display.println(lastIRCode, HEX);
      display.setCursor(0, 35);
      display.println("Saved.");
      display.display();
      delay(2000);
      return;
    }
  }
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Timeout.");
  display.println("No signal detected.");
  display.display();
  delay(1500);
}

void replayIR() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Replaying IR ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  if (lastIRCode == 0) {
    display.setCursor(0, 25);
    display.println("No code to replay.");
    display.println("Capture first.");
    display.display();
    delay(2000);
    return;
  }

  IrSender.sendNEC(lastIRCode, 32);
  display.setCursor(0, 20);
  display.print("Sent: 0x");
  display.println(lastIRCode, HEX);
  display.display();
  delay(1500);
}

// ─── RFID FUNCTIONS ───────────────────────────────────────

bool readRFID() {
  if (Serial2.available() >= 14) {
    byte buf[14];
    Serial2.readBytes(buf, 14);

    // Validar STX y ETX
    if (buf[0] == 0x02 && buf[13] == 0x03) {
      String cardID = "";
      for (int i = 1; i <= 10; i++) {
        cardID += (char)buf[i];
      }
      lastRFIDCard = cardID;

      // Flash LED
      digitalWrite(LED_RFID, HIGH);
      delay(100);
      digitalWrite(LED_RFID, LOW);

      // Mostrar en pantalla
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("== Card Detected ==");
      display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
      display.setCursor(0, 20);
      display.print("ID: ");
      display.println(cardID);
      display.setCursor(0, 35);
      display.println("Saved.");
      display.display();
      delay(2000);
      drawRFIDMenu();
      return true;
    }
  }
  return false;
}

void simulateRFIDCard() {
  lastRFIDCard = "0100 2A3B4C";

  digitalWrite(LED_RFID, HIGH);
  delay(100);
  digitalWrite(LED_RFID, LOW);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== Card Detected ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("ID: ");
  display.println(lastRFIDCard);
  display.setCursor(0, 35);
  display.println("Saved.");
  display.display();
  delay(2000);
  drawRFIDMenu();
}

// ─── SETUP ────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RDM6300_RX, -1);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(LED_RFID, OUTPUT);
  digitalWrite(LED_RFID, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrSender.begin(IR_SEND_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found");
    while (true);
  }

  display.clearDisplay();
  drawMenu();
}

// ─── LOOP ─────────────────────────────────────────────────

void loop() {
  if (!inSubmenu) {
    if (digitalRead(BTN_UP) == LOW) {
      currentItem = (currentItem - 1 + menuSize) % menuSize;
      drawMenu();
      delay(200);
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      currentItem = (currentItem + 1) % menuSize;
      drawMenu();
      delay(200);
    }
    if (digitalRead(BTN_OK) == LOW) {
      currentModule = currentItem;
      inSubmenu = true;
      if (currentModule == 2) drawRFIDMenu();
      if (currentModule == 3) drawIRMenu();
      if (currentModule == 1) drawNFCMenu();
      if (currentModule == 0) drawSubGHzMenu();

      delay(200);
    }
  } else {
    // ── Módulo RFID ──
    if (currentModule == 2) {
      readRFID();
      if (digitalRead(BTN_UP) == LOW) {
        simulateRFIDCard();
        delay(200);
      }
      if (digitalRead(BTN_OK) == LOW) {
        inSubmenu = false;
        currentModule = -1;
        drawMenu();
        delay(200);
      }
    }

    // ── Módulo IR ──
    if (currentModule == 3) {
      if (digitalRead(BTN_UP) == LOW) {
        captureIR();
        drawIRMenu();
        delay(200);
      }
      if (digitalRead(BTN_DOWN) == LOW) {
        replayIR();
        drawIRMenu();
        delay(200);
      }
      if (digitalRead(BTN_OK) == LOW) {
        inSubmenu = false;
        currentModule = -1;
        drawMenu();
        delay(200);
      }
    }

    // ── Módulo NFC ──
    if (currentModule == 1) {
      if (digitalRead(BTN_UP) == LOW) {
        simulateNFCCard();
        delay(200);
      }
      if (digitalRead(BTN_DOWN) == LOW) {
        drawNFCMenu();
        delay(200);
      }
      if (digitalRead(BTN_OK) == LOW) {
        inSubmenu = false;
        currentModule = -1;
        drawMenu();
        delay(200);
      }
    }

    // ── Módulo Sub-GHz ──
    if (currentModule == 0) {
      if (digitalRead(BTN_UP) == LOW) {
        simulateRFCapture();
        delay(200);
      }
      if (digitalRead(BTN_DOWN) == LOW) {
        simulateRFReplay();
        delay(200);
      }
      if (digitalRead(BTN_OK) == LOW) {
        inSubmenu = false;
        currentModule = -1;
        drawMenu();
        delay(200);
      }
    }
  }
  delay(10);
}
