// ESP32 Tally Light - LED Controller
// Open source, MIT License
// Modul for styring av RGB LED basert på systemstatus og tally-state
#include "led_controller.h"

LedController::LedController()
  : pixel_(AppConfig::RGB_LED_COUNT, AppConfig::RGB_LED_PIN, NEO_GRB + NEO_KHZ800),
    currentMode_(LedMode::Off),
    lastBlinkMs_(0),
    blinkOn_(true) {}

void LedController::begin() {
  pixel_.begin();
  pixel_.setBrightness(200);
  pixel_.clear();
  pixel_.show();
}

void LedController::setMode(LedMode mode) {
  if (mode != currentMode_) {
    currentMode_ = mode;
    lastBlinkMs_ = 0;
    blinkOn_ = true;
  }
}

LedMode LedController::getMode() const {
  return currentMode_;
}

void LedController::loop() {
  render();
}

void LedController::showProgram() {
  setMode(LedMode::Program);
  render();
}

void LedController::showPreview() {
  setMode(LedMode::Preview);
  render();
}

void LedController::showOff() {
  setMode(LedMode::Off);
  render();
}

void LedController::showConnecting() {
  setMode(LedMode::Connecting);
  render();
}

void LedController::showConfigPortal() {
  setMode(LedMode::ConfigPortal);
  render();
}

void LedController::showError() {
  setMode(LedMode::Error);
  render();
}

void LedController::showBoot() {
  setMode(LedMode::Boot);
  render();
}

void LedController::setColor(uint8_t r, uint8_t g, uint8_t b) {
  pixel_.setPixelColor(0, pixel_.Color(r, g, b));
  pixel_.show();
}

void LedController::render() {
  const unsigned long now = millis();

  switch (currentMode_) {
    case LedMode::Program:
      setColor(255, 0, 0);
      break;

    case LedMode::Preview:
      setColor(0, 200, 0);
      break;

    case LedMode::Boot:
      setColor(80, 80, 80);
      break;

    case LedMode::Off:
      setColor(0, 0, 0);
      break;

    case LedMode::Connecting:
      if (now - lastBlinkMs_ >= AppConfig::LED_BLINK_INTERVAL_MS) {
        lastBlinkMs_ = now;
        blinkOn_ = !blinkOn_;
      }
      setColor(blinkOn_ ? 200 : 0, blinkOn_ ? 130 : 0, 0);
      break;

    case LedMode::ConfigPortal:
      if (now - lastBlinkMs_ >= AppConfig::LED_BLINK_INTERVAL_MS) {
        lastBlinkMs_ = now;
        blinkOn_ = !blinkOn_;
      }
      setColor(0, 0, blinkOn_ ? 255 : 0);
      break;

    case LedMode::Error: {
      const uint32_t errorInterval = AppConfig::LED_BLINK_INTERVAL_MS / 2;
      if (now - lastBlinkMs_ >= errorInterval) {
        lastBlinkMs_ = now;
        blinkOn_ = !blinkOn_;
      }
      setColor(blinkOn_ ? 255 : 0, 0, 0);
      break;
    }
  }
}
