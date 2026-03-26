
# ESP32 Tally Light – Development Plan

## Overview

This document outlines the remaining development work required to evolve the ESP32 Tally Light project from a working prototype (vMix only) into a modular, extensible and production-ready system.

---

## Current Status

### Implemented
- ESP32 firmware base
- WiFi connectivity
- Basic vMix integration (direct approach)
- LED indication
- Initial hardware setup (LiPo + charging concept)

### Missing / Needs Refactor
- No provider abstraction (currently vMix-specific logic)
- No unified configuration model
- No structured state handling
- No support for OBS or ATEM
- No persistent config system (or incomplete)
- No clear module separation

---

## Phase 1 – Core Refactor (CRITICAL)

### Goal
Introduce a clean architecture that supports multiple tally providers.

### Tasks

#### 1. Introduce TallyProvider Interface

Create:
- `include/tally_provider.h`

Responsibilities:
- Define contract for all tally providers
- Standardize output (TallyState)

Example:

```cpp
enum class TallyState {
  PROGRAM,
  PREVIEW,
  IDLE,
  UNKNOWN,
  ERROR
};

class ITallyProvider {
public:
  virtual void begin() = 0;
  virtual void loop() = 0;
  virtual bool isConnected() = 0;
  virtual TallyState getState() = 0;
};
```

---

#### 2. Refactor vMix into Provider

Create:
- `src/providers/vmix_provider.cpp`
- `src/providers/vmix_provider.h`

Tasks:
- Move existing TCP logic into provider
- Implement:
  - connect
  - subscribe tally
  - parse responses
- Output normalized TallyState

---

#### 3. Introduce Provider Factory

Create:
- `src/providers/provider_factory.cpp`

Responsibility:
- Instantiate correct provider based on config

---

#### 4. Update main.cpp

Replace:
- direct vMix logic

With:
- provider instance
- generic loop handling

---

## Phase 2 – Configuration System

### Goal
Persistent, flexible configuration

### Tasks

#### 1. Create DeviceConfig model

```cpp
enum class SwitcherType {
  VMIX,
  OBS,
  ATEM
};

struct DeviceConfig {
  String ssid;
  String password;
  SwitcherType type;
  String host;
  int port;
  int channel;
};
```

---

#### 2. Implement storage (Preferences / NVS)

Create:
- `src/config/config_store.cpp`

Features:
- save config
- load config
- reset config

---

#### 3. Web Config Portal

Features:
- AP mode fallback
- HTML form
- Save + reboot

---

## Phase 3 – LED Controller Refactor

### Goal
Decouple LED logic from providers

### Tasks

Create:
- `src/led/led_controller.cpp`

Responsibilities:
- Handle:
  - program (red)
  - preview (green)
  - connecting (yellow blink)
  - error (red blink)
- Input = TallyState

---

## Phase 4 – OBS Provider

### Goal
Add OBS support

### Requirements:
- WebSocket client
- Handle:
  - Hello
  - Identify
  - authentication (if enabled)

### Tasks:
- Implement connection
- Map OBS state → TallyState

---

## Phase 5 – ATEM Provider

### Goal
Add ATEM support

### Tasks:
- Implement ATEM network protocol
- Extract tally information
- Map to TallyState

---

## Phase 6 – Device State Machine

### Goal
Make system predictable and robust

States:
- BOOT
- SETUP_MODE
- CONNECTING_WIFI
- CONNECTING_PROVIDER
- RUNNING
- ERROR

---

## Phase 7 – Power Optimization (Later)

### Tasks:
- Reduce idle consumption
- Optional deep sleep
- Optimize LED usage

---

## Phase 8 – OTA Updates (Optional)

### Tasks:
- Add firmware update over WiFi
- Secure endpoint

---

## Phase 9 – Hardware Integration Improvements

### Tasks:
- Finalize enclosure layout
- Improve button handling
- Stabilize power delivery

---

## Recommended Development Order

1. Provider interface + vMix refactor
2. Config system (NVS)
3. LED controller abstraction
4. OBS provider
5. ATEM provider
6. State machine
7. Power optimization
8. OTA

---

## Definition of Done (MVP v2)

- Device configurable via web
- Supports vMix fully
- Clean provider architecture
- Stable LED indication
- Config persists after reboot

---

## Future Extensions

- Multiple device sync
- Web dashboard
- Signal strength indicator
- Battery monitoring

---

## Final Note

Do NOT implement OBS and ATEM before completing the provider refactor.

That refactor is the foundation for everything else.
