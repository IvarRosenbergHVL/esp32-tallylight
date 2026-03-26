#pragma once

#include "app_config.h"

class ITallyProvider {
public:
  virtual ~ITallyProvider() = default;

  virtual void begin(const DeviceConfig& config) = 0;
  virtual void loop() = 0;
  virtual bool isConnected() const = 0;
  virtual TallyState getState() const = 0;
};
