# OBSProvider
// Grensesnitt og implementasjon for OBS WebSocket tally

#ifndef OBS_PROVIDER_H
#define OBS_PROVIDER_H

#include "tally_provider.h"
#include <WiFiClient.h>

class OBSProvider : public ITallyProvider {
public:
    OBSProvider(const char* host, uint16_t port);
    bool connect() override;
    void disconnect() override;
    bool update() override;
    TallyState getTallyState(uint8_t input) override;
private:
    const char* _host;
    uint16_t _port;
    WiFiClient _client;
    bool _connected;
    // TODO: Legg til evt. autentisering og meldingshåndtering
};

#endif // OBS_PROVIDER_H
