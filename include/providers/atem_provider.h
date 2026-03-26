# ATEMProvider
// Grensesnitt og implementasjon for ATEM tally

#ifndef ATEM_PROVIDER_H
#define ATEM_PROVIDER_H

#include "tally_provider.h"
#include <WiFiClient.h>

class ATEMProvider : public ITallyProvider {
public:
    ATEMProvider(const char* host, uint16_t port);
    bool connect() override;
    void disconnect() override;
    bool update() override;
    TallyState getTallyState(uint8_t input) override;
private:
    const char* _host;
    uint16_t _port;
    WiFiClient _client;
    bool _connected;
    // TODO: Legg til ATEM-protokollhåndtering
};

#endif // ATEM_PROVIDER_H
