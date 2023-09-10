#include <M5AtomS3.h>

#include "ssid.h"

#define USE_M5_ATOM_S3
#include <WiFiESP32.h>

/** WiFiのSSID（ssid.h（git管理対象外）にて定義） */
const char *SSID = WIFI_SSID;
/** WiFiのパスワード（ssid.h（git管理対象外）にて定義） */
const char *PASS = WIFI_PASSWORD;
WiFiESP32 wifi = WiFiESP32(SSID, PASS);

uint8_t brightness = 128;

CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 8) | (g << 16) | b);
}

void setup() {
  M5.begin(false, true, false, true);

  // 本体LED赤点灯
  M5.dis.drawpix(dispColor(127, 0, 0));
  M5.dis.show();

  delay(2000);
  logger.info("AtimS3 Lite setup finished.");
}

void loop() {
  if (wifi.healthCheck()) {
    M5.dis.drawpix(dispColor(0, 127, 0));
    M5.dis.show();
  } else {
    M5.dis.drawpix(dispColor(127, 0, 0));
    M5.dis.show();
  }
  delay(10);
}
