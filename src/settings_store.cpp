#include "settings_store.h"

bool SettingsStore::load(DeviceConfig& config) {
  if (!prefs_.begin(AppConfig::SETTINGS_NAMESPACE, true)) {
    return false;
  }

  config.deviceName = prefs_.getString("deviceName", "");
  config.wifiSsid = prefs_.getString("wifiSsid", "");
  config.wifiPassword = prefs_.getString("wifiPass", "");
  config.vmixIp = prefs_.getString("vmixIp", "");
  config.vmixPort = prefs_.getUShort("vmixPort", AppConfig::DEFAULT_VMIX_PORT);
  config.channel = prefs_.getUChar("channel", AppConfig::DEFAULT_CHANNEL);

  prefs_.end();
  return isValidConfig(config);
}

bool SettingsStore::save(const DeviceConfig& config) {
  if (!prefs_.begin(AppConfig::SETTINGS_NAMESPACE, false)) {
    return false;
  }

  prefs_.putString("deviceName", config.deviceName);
  prefs_.putString("wifiSsid", config.wifiSsid);
  prefs_.putString("wifiPass", config.wifiPassword);
  prefs_.putString("vmixIp", config.vmixIp);
  prefs_.putUShort("vmixPort", config.vmixPort);
  prefs_.putUChar("channel", config.channel);

  prefs_.end();
  return true;
}

void SettingsStore::clear() {
  if (!prefs_.begin(AppConfig::SETTINGS_NAMESPACE, false)) {
    return;
  }

  prefs_.clear();
  prefs_.end();
}