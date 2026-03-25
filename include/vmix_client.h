#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "app_config.h"

class VmixClient {
public:
  VmixClient();

  void begin(const DeviceConfig& config);
  void loop();
  void disconnect();

  bool isConnected() const;
  TallyState getTallyState() const;

private:
  WiFiClient client_;
  String host_;
  uint16_t port_;
  uint8_t channel_;
  String rxLine_;

  unsigned long lastConnectAttemptMs_;
  unsigned long lastDataMs_;
  unsigned long lastPollMs_;

  bool connected_;
  TallyState tallyState_;

  bool connectIfNeeded();
  void readIncoming();
  void handleLine(const String& line);
  void applyTallyString(const String& values);
};