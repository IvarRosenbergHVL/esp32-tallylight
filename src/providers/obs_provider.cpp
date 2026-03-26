#include "obs_provider.h"

OBSProvider::OBSProvider(const char* host, uint16_t port)
    : _host(host), _port(port), _connected(false) {}

bool OBSProvider::connect() {
    // TODO: Implementer tilkobling til OBS WebSocket
    _connected = true;
    return _connected;
}

void OBSProvider::disconnect() {
    // TODO: Implementer frakobling
    _connected = false;
}

bool OBSProvider::update() {
    // TODO: Hent tally-status fra OBS
    return _connected;
}

TallyState OBSProvider::getTallyState(uint8_t input) {
    // TODO: Returner riktig tally-state for input
    return TALLY_OFF;
}
