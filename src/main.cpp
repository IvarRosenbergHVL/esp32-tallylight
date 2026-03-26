// ESP32 Tally Light - Main Application
// Open source, MIT License
// Provides main firmware logic for WiFi, config portal, provider-abstraksjon og LED-styring
#include <Arduino.h>
#include <WiFi.h>

#include "app_config.h"
#include "led_controller.h"
#include "settings_store.h"
#include "wifi_portal.h"
#include "tally_provider.h"
#include "providers/provider_factory.h"
#include "tally_logic.h"

LedController led;
SettingsStore settingsStore;
WifiPortal wifiPortal;
ITallyProvider* provider = nullptr;

DeviceConfig config;

bool connectToWifi(const DeviceConfig& cfg) {
  Serial.print("[WIFI] Connecting to SSID: ");
  Serial.println(cfg.wifiSsid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg.wifiSsid.c_str(), cfg.wifiPassword.c_str());

  const unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < AppConfig::WIFI_CONNECT_TIMEOUT_MS) {
    led.showConnecting();
    led.loop();
    delay(25);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[WIFI] Connected, IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("[WIFI] Connection failed");
  return false;
}

void startPortalAndSaveConfig() {
  WiFi.disconnect(true, true);
  delay(300);

  led.showConfigPortal();

  if (wifiPortal.run(config, led)) {
    if (settingsStore.save(config)) {
      Serial.println("[CFG] Saved successfully");
    } else {
      Serial.println("[CFG] Save failed");
      led.showError();
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(AppConfig::Pins::BUTTON_RESET, INPUT_PULLUP);

  led.begin();
  led.showBoot();
  delay(500);
  led.showOff();

  Serial.println();
  Serial.println("ESP32 Tally Light");

  const bool hasConfig = settingsStore.load(config);

  if (!hasConfig) {
    Serial.println("[CFG] No valid config found, starting portal");
    startPortalAndSaveConfig();
  }

  if (!connectToWifi(config)) {
    Serial.println("[CFG] Falling back to setup portal");
    startPortalAndSaveConfig();

    if (!connectToWifi(config)) {
      Serial.println("[WIFI] Failed again after portal");
      led.showError();
      delay(3000);
      ESP.restart();
    }
  }

  provider = ProviderFactory::create(config);
  provider->begin(config);

  Serial.print("[CFG] Device name: ");
  Serial.println(config.deviceName);
  Serial.print("[CFG] Switcher type: ");
  Serial.println(static_cast<int>(config.type));
  Serial.print("[CFG] Channel: ");
  Serial.println(config.channel);
}

void loop() {
  // Reset button: hold GPIO 7 for 3 seconds to factory reset
  static unsigned long resetPressMs = 0;
  if (digitalRead(AppConfig::Pins::BUTTON_RESET) == LOW) {
    if (resetPressMs == 0) {
      resetPressMs = millis();
    } else if (millis() - resetPressMs >= AppConfig::RESET_HOLD_TIME_MS) {
      Serial.println("[RESET] Factory reset triggered");
      settingsStore.clear();
      delay(500);
      ESP.restart();
    }
  } else {
    resetPressMs = 0;
  }

  if (WiFi.status() != WL_CONNECTED) {
    led.showConnecting();
    led.loop();
    delay(100);

    if (!connectToWifi(config)) {
      Serial.println("[WIFI] Lost connection, restarting setup portal");
      startPortalAndSaveConfig();

      if (!connectToWifi(config)) {
        led.showError();
        delay(3000);
        ESP.restart();
      }

      if (provider) { delete provider; provider = nullptr; }
      provider = ProviderFactory::create(config);
      provider->begin(config);
    }

    return;
  }

  provider->loop();

  const LedMode mode = TallyLogic::map(
    WiFi.status() == WL_CONNECTED,
    provider->isConnected(),
    provider->getState()
  );

  led.setMode(mode);
  led.loop();

  delay(10);
}