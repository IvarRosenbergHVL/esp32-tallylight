#include "atem_provider.h"

ATEMProvider::ATEMProvider(const char* host, uint16_t port)
    : _host(host), _port(port), _connected(false) {}

bool ATEMProvider::connect() {
    // TODO: Implementer tilkobling til ATEM
    _connected = true;
    return _connected;
}

void ATEMProvider::disconnect() {
    // TODO: Implementer frakobling
    _connected = false;
}

bool ATEMProvider::update() {
    // TODO: Hent tally-status fra ATEM
    return _connected;
}

TallyState ATEMProvider::getTallyState(uint8_t input) {
    // TODO: Returner riktig tally-state for input
    return TALLY_OFF;
}
