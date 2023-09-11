/**
 * @file ApiClient.cpp
 * @brief WebAPIクライアント
 */

#include "ApiClient.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Log.h>

/**
 * @brief Construct a new Api Client:: Api Client object
 *
 * @param protocol プロトコル
 * @param host サーバーホスト
 * @param port サーバーポート
 */
ApiClient::ApiClient(String protocol, String host, uint16_t port)
    : baseUri(protocol + String("://") + host + String(":") + String(port)) {}

/**
 * @brief Destroy the Api Client:: Api Client object
 *
 */
ApiClient::~ApiClient() {}

/**
 * @brief HTTP POSTのテスト
 *
 */
void ApiClient::postTest() {
  HTTPClient http;
  http.begin("http://httpbin.org/post");
  http.addHeader("Content-Type", "application/json");
  http.POST("{\"title\": \"test\"}");
  logger.info(http.getString());
  http.end();
}

/**
 * @brief 通過検知情報をPOSTする
 *
 * @param deviceId デバイスID
 * @param passing 通過検知情報
 * @return true 成功
 * @return false 失敗
 */
bool ApiClient::postPassing(String deviceId, bool passing) {
  logger.info("postPassing(): start");

  // https://arduinojson.org/v6/assistant
  StaticJsonDocument<64> doc;
  doc["deviceId"] = deviceId;
  doc["passing"] = passing;
  String reqBody = "";
  serializeJson(doc, reqBody);

  HTTPClient http;
  String url = baseUri + String("/api/v0/sensors");
  logger.info("postSensors(): start " + url + ", " + reqBody);
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int statusCode = http.POST(reqBody);
  http.end();

  if (statusCode == 200) {
    logger.info("postSensors(): response = " + http.getString());
    return true;
  }
  logger.error("postSensors(): statusCode = " + String(statusCode));
  return false;
}
