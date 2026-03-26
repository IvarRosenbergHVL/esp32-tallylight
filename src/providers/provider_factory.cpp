#include "providers/provider_factory.h"
#include "providers/vmix_provider.h"

ITallyProvider* ProviderFactory::create(const DeviceConfig& config) {
  switch (config.type) {
    case SwitcherType::VMIX:
      return new VmixProvider();

    default:
      Serial.println("[FACTORY] Unknown switcher type, defaulting to vMix");
      return new VmixProvider();
  }
}
