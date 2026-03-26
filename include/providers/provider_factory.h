#pragma once

#include "app_config.h"
#include "tally_provider.h"

class ProviderFactory {
public:
  static ITallyProvider* create(const DeviceConfig& config);
};
