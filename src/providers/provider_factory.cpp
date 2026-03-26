#include "providers/provider_factory.h"

#include "providers/vmix_provider.h"
#include "providers/obs_provider.h"
#include "providers/atem_provider.h"

ITallyProvider* ProviderFactory::create(const DeviceConfig& config) {
  switch (config.type) {
    case SwitcherType::VMIX:
      return new VmixProvider();
    case SwitcherType::OBS:
      return new OBSProvider(config.host, config.port); // Forutsetter at config har host/port
    case SwitcherType::ATEM:
      return new ATEMProvider(config.host, config.port);
    default:
      Serial.println("[FACTORY] Unknown switcher type, defaulting to vMix");
      return new VmixProvider();
  }
}
