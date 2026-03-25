#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "app_config.h"
#include "led_controller.h"

class WifiPortal {
public:
  WifiPortal();

  bool run(DeviceConfig& config, LedController& led);

private:
  WebServer server_;
  bool saved_;
  DeviceConfig workingConfig_;

  String buildApName(const DeviceConfig& config) const;
  String htmlEscape(const String& value) const;
  void setupRoutes(LedController& led);
  void handleRoot();
  void handleSave();
  bool validate(DeviceConfig& config) const;
};