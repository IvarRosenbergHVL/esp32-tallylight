#pragma once

#include <Arduino.h>

namespace AppConfig {
  static constexpr const char* SETTINGS_NAMESPACE = "tallycfg";

  static constexpr uint16_t DEFAULT_VMIX_PORT = 8099;
  static constexpr uint8_t DEFAULT_CHANNEL = 1;

  static constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 25000;
  static constexpr uint32_t VMIX_RETRY_MS = 3000;
  static constexpr uint32_t VMIX_POLL_MS = 10000;
  static constexpr uint32_t VMIX_DATA_TIMEOUT_MS = 30000;

  static constexpr int RGB_LED_PIN = 48;
  static constexpr int RGB_LED_COUNT = 1;

  // Pins
  namespace Pins {
    static constexpr int LED_RED = 4;
    static constexpr int LED_GREEN = 5;
    static constexpr int LED_YELLOW = 6;
    static constexpr int BUTTON_RESET = 7;
  }

// Timing
#define RESET_HOLD_TIME_MS 3000

  
  static constexpr uint32_t LED_BLINK_INTERVAL_MS = 400;

  static constexpr const char* AP_PASSWORD = "12345678";
}

enum class TallyState : uint8_t {
  Off = 0,
  Program = 1,
  Preview = 2
};

enum class LedMode : uint8_t {
  Off,
  Boot,
  ConfigPortal,
  Connecting,
  Program,
  Preview,
  Error
};

struct DeviceConfig {
  String deviceName;
  String wifiSsid;
  String wifiPassword;
  String vmixIp;
  uint16_t vmixPort = AppConfig::DEFAULT_VMIX_PORT;
  uint8_t channel = AppConfig::DEFAULT_CHANNEL;
};

inline bool isValidConfig(const DeviceConfig& config) {
  return !config.deviceName.isEmpty() &&
         !config.wifiSsid.isEmpty() &&
         !config.wifiPassword.isEmpty() &&
         !config.vmixIp.isEmpty() &&
         config.vmixPort > 0 &&
         config.channel >= 1;
}