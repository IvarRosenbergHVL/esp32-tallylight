#pragma once

#include "tally_provider.h"
#include "vmix_client.h"

class VmixProvider : public ITallyProvider {
public:
  void begin(const DeviceConfig& config) override;
  void loop() override;
  bool isConnected() const override;
  TallyState getState() const override;

private:
  VmixClient client_;
};
