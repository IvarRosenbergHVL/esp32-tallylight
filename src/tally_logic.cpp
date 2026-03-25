#include "tally_logic.h"

LedMode TallyLogic::map(bool wifiConnected, bool vmixConnected, TallyState tallyState) {
  if (!wifiConnected || !vmixConnected) {
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