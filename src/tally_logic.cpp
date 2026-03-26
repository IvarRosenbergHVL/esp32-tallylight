#include "tally_logic.h"

LedMode TallyLogic::map(bool wifiConnected, bool providerConnected, TallyState tallyState) {
  if (!wifiConnected || !providerConnected) {
    return LedMode::Connecting;
  }

  switch (tallyState) {
    case TallyState::Program:
      return LedMode::Program;
    case TallyState::Preview:
      return LedMode::Preview;
    case TallyState::Off:
    default:
      return LedMode::Off;
  }
}