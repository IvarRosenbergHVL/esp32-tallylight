#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "app_config.h"

class LedController {
public:
  LedController();

  void begin();
  void setMode(LedMode mode);
  LedMode getMode() const;
  void loop();

  void showProgram();
  void showPreview();
  void showOff();
  void showConnecting();
  void showConfigPortal();
  void showError();
  void showBoot();

private:
  Adafruit_NeoPixel pixel_;
  LedMode currentMode_;
  unsigned long lastBlinkMs_;
  bool blinkOn_;

  void setColor(uint8_t r, uint8_t g, uint8_t b);
  void render();
};