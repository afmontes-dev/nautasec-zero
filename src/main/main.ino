#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BTN_UP    18
#define BTN_DOWN  19
#define BTN_OK    23

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

void drawSelected() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("== NautaSec Zero ==");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setCursor(4, 25);
  display.print("> ");
  display.println(menuItems[currentItem]);
  display.setCursor(4, 45);
  display.println("OK to go back");
  display.display();
}

bool inSubmenu = false;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found");
    while (true);
  }

  display.clearDisplay();
  drawMenu();
}

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
      inSubmenu = true;
      drawSelected();
      delay(200);
    }
  } else {
    if (digitalRead(BTN_OK) == LOW) {
      inSubmenu = false;
      drawMenu();
      delay(200);
    }
  }
  delay(10);
}
