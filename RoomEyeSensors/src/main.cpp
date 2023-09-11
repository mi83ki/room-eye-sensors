/**
 * @file main.cpp
 * @brief 通過検知センサモジュールのメイン処理部
 */

#include <M5Atom.h>
#include <Timer.h>
#include <WiFiESP32.h>

#include "ApiClient.h"
#include "ToFSensor.h"
#include "ssid.h"


/** WiFiのSSID（ssid.h（git管理対象外）にて定義） */
const char *SSID = WIFI_SSID;
/** WiFiのパスワード（ssid.h（git管理対象外）にて定義） */
const char *PASS = WIFI_PASSWORD;
WiFiESP32 wifi = WiFiESP32(SSID, PASS);

/** デバイスID */
const String DEVICE_ID = "sensor01";

/** LED輝度（0～255） */
uint8_t brightness = 60;

/** WebAPIクライアントインスタンス */
ApiClient apiClient = ApiClient("http", SERVER_HOST, SERVER_PORT);

/** ToFセンサー */
ToFSensor *tofSensor;

/**
 * @brief LED表示カラー値を取得する
 *
 * @param r 赤
 * @param g 緑
 * @param b 青
 * @return CRGB LED表示カラー値
 */
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 8) | (g << 16) | b);
}

/**
 * @brief setup関数
 *
 */
void setup() {
  M5.begin();
  tofSensor = new ToFSensor();

  // 本体LED赤点灯
  // M5.dis.drawpix(dispColor(brightness, 0, 0));
  // M5.dis.show();

  delay(2000);
  logger.info("Atom Lite setup finished.");
}

/**
 * @brief loop関数
 *
 */
void loop() {
  tofSensor->tofLoop();
  /*
  static bool isConnected = false;
  static Timer timer = Timer(10000);
  if (wifi.healthCheck()) {
    if (!isConnected) {
      M5.dis.drawpix(dispColor(0, brightness, 0));
      M5.dis.show();
      isConnected = true;
    }
    if (timer.isCycleTime()) {
      apiClient.postPassing(String(DEVICE_ID), true);
    }
  } else {
    if (isConnected) {
      M5.dis.drawpix(dispColor(brightness, 0, 0));
      M5.dis.show();
      isConnected = false;
    }
  }
  delay(10);
  */
}
