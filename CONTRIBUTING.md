# Contributing to ESP32 Tally Light

Thank you for considering contributing! This project aims to support multiple video switcher systems (vMix, OBS, ATEM, and more in the future). Below are guidelines for adding support for new systems and general contribution practices.

## Adding Support for New Switcher Systems
1. **Provider Interface**: Implement a new provider class that inherits from `ITallyProvider` (see `include/tally_provider.h`).
2. **Provider Implementation**: Place your provider in `include/providers/` and `src/providers/`.
3. **Factory Update**: Extend `ProviderFactory` to instantiate your provider based on config.
4. **Config/Webportal**: Update `DeviceConfig` and the WiFi portal to allow users to select and configure your new system.
5. **Testing**: Ensure your provider works with the main state machine and LED logic.
6. **Documentation**: Update the user guide and this file with setup instructions for your system.

## General Contribution Guidelines
- Fork the repo and create a feature branch for your changes.
- Write clear, descriptive commit messages.
- Keep code modular and document public APIs.
- Test your changes on real hardware if possible.
- Open a pull request and describe your changes and testing.

## Code Style
- Use consistent formatting (see existing code for style).
- Prefer English for code/comments, Norwegian is OK for user-facing docs.

## Issues & Feature Requests
- Use GitHub Issues for bugs, feature requests, and questions.

## Contact
For questions, open an issue or contact the maintainer.

---

**Supported systems:** vMix, OBS, ATEM. Contributions for other systems are welcome!
