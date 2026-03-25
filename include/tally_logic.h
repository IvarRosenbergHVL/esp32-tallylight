#pragma once

#include "app_config.h"

class TallyLogic {
public:
  static LedMode map(bool wifiConnected, bool vmixConnected, TallyState tallyState);
};