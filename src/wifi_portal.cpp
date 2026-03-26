// ESP32 Tally Light - WiFi Config Portal
// Open source, MIT License
// Web-basert konfigurasjonsportal for førstegangsoppsett og fallback
#include "wifi_portal.h"

WifiPortal::WifiPortal() : server_(80), saved_(false) {}

String WifiPortal::buildApName(const DeviceConfig& config) const {
  String suffix = String((uint32_t)ESP.getEfuseMac(), HEX);
  suffix.toUpperCase();
  if (suffix.length() > 4) {
    suffix = suffix.substring(suffix.length() - 4);
  }

  String base = config.deviceName.isEmpty() ? "TALLY" : config.deviceName;
  base.replace(" ", "-");
  return base + "-" + suffix;
}

String WifiPortal::htmlEscape(const String& value) const {
  String out = value;
  out.replace("&", "&amp;");
  out.replace("\"", "&quot;");
  out.replace("<", "&lt;");
  out.replace(">", "&gt;");
  return out;
}

bool WifiPortal::validate(DeviceConfig& config) const {
  if (config.deviceName.isEmpty()) return false;
  if (config.wifiSsid.isEmpty()) return false;
  if (config.wifiPassword.isEmpty()) return false;
  if (config.vmixPort == 0) return false;
  if (config.channel < 1 || config.channel > 99) return false;
  if (config.type == SwitcherType::VMIX && config.vmixIp.isEmpty()) return false;
  return true;
}

void WifiPortal::handleRoot() {
  String html;
  html.reserve(3000);

  html += "<!doctype html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>Tally Setup</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#111;color:#eee;padding:20px;}";
  html += "form{max-width:520px;margin:auto;background:#1b1b1b;padding:20px;border-radius:12px;}";
  html += "label{display:block;margin-top:14px;margin-bottom:6px;}";
  html += "input{width:100%;padding:10px;border-radius:8px;border:1px solid #444;background:#222;color:#fff;}";
  html += "select{width:100%;padding:10px;border-radius:8px;border:1px solid #444;background:#222;color:#fff;}";
  html += "button{margin-top:20px;padding:12px 16px;border:none;border-radius:8px;background:#2d6cdf;color:#fff;font-weight:bold;width:100%;}";
  html += "small{color:#bbb;display:block;margin-top:6px;}";
  html += "</style></head><body>";
  html += "<form method='POST' action='/save'>";
  html += "<h2>ESP32 Tally Setup</h2>";

  html += "<label>Switcher type</label>";
  html += "<select name='switcherType'>";
  html += String("<option value='0'") + (workingConfig_.type == SwitcherType::VMIX ? " selected" : "") + ">vMix</option>";
  html += String("<option value='1'") + (workingConfig_.type == SwitcherType::OBS  ? " selected" : "") + ">OBS (coming soon)</option>";
  html += String("<option value='2'") + (workingConfig_.type == SwitcherType::ATEM ? " selected" : "") + ">ATEM (coming soon)</option>";
  html += "</select>";

  html += "<label>Device name</label>";
  html += "<input name='deviceName' value='" + htmlEscape(workingConfig_.deviceName) + "' required>";

  html += "<label>WiFi SSID</label>";
  html += "<input name='wifiSsid' value='" + htmlEscape(workingConfig_.wifiSsid) + "' required>";

  html += "<label>WiFi password</label>";
  html += "<input type='password' name='wifiPassword' value='" + htmlEscape(workingConfig_.wifiPassword) + "' required>";

  html += "<label>vMix IP</label>";
  html += "<input name='vmixIp' value='" + htmlEscape(workingConfig_.vmixIp) + "' required>";

  html += "<label>vMix port</label>";
  html += "<input type='number' name='vmixPort' min='1' max='65535' value='" + String(workingConfig_.vmixPort) + "' required>";

  html += "<label>Channel / input number</label>";
  html += "<input type='number' name='channel' min='1' max='99' value='" + String(workingConfig_.channel) + "' required>";

  html += "<button type='submit'>Save configuration</button>";
  html += "<small>After save, the device will leave setup mode and use the saved values after restart.</small>";
  html += "</form></body></html>";

  server_.send(200, "text/html", html);
}

void WifiPortal::handleSave() {
  DeviceConfig newConfig;
  newConfig.deviceName = server_.arg("deviceName");
  newConfig.wifiSsid = server_.arg("wifiSsid");
  newConfig.wifiPassword = server_.arg("wifiPassword");
  newConfig.vmixIp = server_.arg("vmixIp");
  newConfig.vmixPort = static_cast<uint16_t>(server_.arg("vmixPort").toInt());
  newConfig.channel = static_cast<uint8_t>(server_.arg("channel").toInt());
  newConfig.type = static_cast<SwitcherType>(server_.arg("switcherType").toInt());

  if (!validate(newConfig)) {
    server_.send(400, "text/html", "<h2>Invalid config</h2><p>Go back and fill all fields correctly.</p>");
    return;
  }

  workingConfig_ = newConfig;
  saved_ = true;

  server_.send(200, "text/html",
    "<!doctype html><html><head><meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>Saved</title></head><body style='font-family:Arial;background:#111;color:#eee;padding:20px;'>"
    "<h2>Saved</h2><p>You can now return to the device. It will continue with the new configuration.</p>"
    "</body></html>");
}

void WifiPortal::setupRoutes(LedController& led) {
  server_.on("/", HTTP_GET, [this]() {
    handleRoot();
  });

  server_.on("/save", HTTP_POST, [this]() {
    handleSave();
  });

  server_.onNotFound([this]() {
    handleRoot();
  });

  led.showConfigPortal();
}

bool WifiPortal::run(DeviceConfig& config, LedController& led) {
  saved_ = false;
  workingConfig_ = config;

  WiFi.mode(WIFI_AP);
  WiFi.disconnect(true, true);
  delay(300);

  const String apName = buildApName(config);
  WiFi.softAP(apName.c_str(), AppConfig::AP_PASSWORD);

  Serial.println("[PORTAL] Setup AP started");
  Serial.print("[PORTAL] SSID: ");
  Serial.println(apName);
  Serial.print("[PORTAL] Password: ");
  Serial.println(AppConfig::AP_PASSWORD);
  Serial.print("[PORTAL] Open: http://");
  Serial.println(WiFi.softAPIP());

  setupRoutes(led);
  server_.begin();

  while (!saved_) {
    server_.handleClient();
    led.loop();
    delay(5);
  }

  server_.stop();
  WiFi.softAPdisconnect(true);

  config = workingConfig_;
  return true;
}