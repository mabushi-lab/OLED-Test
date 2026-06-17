/*
 * SBC-OLED01 Connection Test
 * Hardware: Arduino Nano + Joy-IT SBC-OLED01 (SSD1306, 128x64)
 *
 * Wiring:
 *   VCC  ->  3.3V or 5V
 *   GND  ->  GND
 *   SCL  ->  D12
 *   SDA  ->  D11
 *
 * Library required: U8g2 by olikraus (install via Library Manager)
 */

#include <U8g2lib.h>

// Software I2C: clock=D12, data=D11
U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(
  U8G2_R0,
  /* clock (SCL) */ 12,
  /* data  (SDA) */ 11,
  /* reset       */ U8X8_PIN_NONE
);

void drawCenteredStr(const char* str, uint8_t y) {
  uint8_t w = display.getStrWidth(str);
  display.drawStr((128 - w) / 2, y, str);
}

void setup() {
  display.begin();

  // --- Page 1: Connection confirmed ---
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB10_tr);
  drawCenteredStr("OLED Test", 14);
  display.drawHLine(0, 17, 128);
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(4, 30, "Screen:  SBC-OLED01");
  display.drawStr(4, 42, "Driver:  SSD1306");
  display.drawStr(4, 54, "SCL:D12  SDA:D11");
  display.drawStr(4, 64, "Status:  OK");
  display.sendBuffer();
  delay(3000);
}

void loop() {
  // --- Page 2: Shape / graphics test ---
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tr);
  drawCenteredStr("Graphics Test", 10);
  display.drawFrame(0, 0, 128, 64);         // border
  display.drawCircle(24, 38, 18);           // circle
  display.drawDisc(64, 42, 14);             // filled circle
  display.drawBox(88, 24, 32, 32);          // filled rect
  display.drawTriangle(4, 62, 44, 62, 24, 24); // triangle outline
  display.sendBuffer();
  delay(2500);

  // --- Page 3: Pixel / contrast test (checkerboard) ---
  display.clearBuffer();
  for (uint8_t x = 0; x < 128; x++) {
    for (uint8_t y = 0; y < 64; y++) {
      if ((x + y) % 2 == 0) display.drawPixel(x, y);
    }
  }
  display.sendBuffer();
  delay(2500);

  // --- Page 4: Scrolling text ---
  const char* msg = "  Connection verified! All pixels working.  ";
  int16_t offset = 128;
  display.setFont(u8g2_font_ncenB10_tr);
  uint32_t start = millis();
  while (millis() - start < 4000) {
    display.clearBuffer();
    display.drawStr(offset, 38, msg);
    display.sendBuffer();
    offset -= 2;
    if (offset < -(int16_t)display.getStrWidth(msg)) offset = 128;
  }
}
