#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "app_config.h"

class SettingsStore {
public:
  bool load(DeviceConfig& config);
  bool save(const DeviceConfig& config);
  void clear();

private:
  Preferences prefs_;
};