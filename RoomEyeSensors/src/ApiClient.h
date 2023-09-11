/**
 * @file ApiClient.h
 * @brief WebAPIクライアント
 */

#pragma once

#include <Arduino.h>

class ApiClient {
 public:
  ApiClient(String, String, uint16_t);
  ~ApiClient();
  void postTest();
  bool postPassing(String, bool);

 private:
  String baseUri;
};
