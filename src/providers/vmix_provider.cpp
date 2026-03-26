#include "providers/vmix_provider.h"

void VmixProvider::begin(const DeviceConfig& config) {
  client_.begin(config);
}

void VmixProvider::loop() {
  client_.loop();
}

bool VmixProvider::isConnected() const {
  return client_.isConnected();
}

TallyState VmixProvider::getState() const {
  return client_.getTallyState();
}
