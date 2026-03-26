# ESP32 Tally Light

Enkel, trådløs tally-lampe for vMix, OBS og ATEM. Åpen kildekode, bygget for ESP32.

## Hva er dette?
- Gir visuell tally-indikasjon (Program/Preview) med én RGB LED
- Støtter flere produksjonssystemer (vMix nå, OBS/ATEM kommer)
- Konfigureres via WiFi-portal, ingen PC nødvendig
- Designet for små produksjonsteam og streaming

## Kom i gang
1. Bygg firmware med PlatformIO
2. Koble til ESP32, LED, knapp og strøm (se hardware)
3. Start enheten, koble til WiFi-portalen, legg inn config
4. Klar til bruk!

## Bygg
Krav: PlatformIO, ESP32

```bash
platformio run
platformio run --target upload
```

## Hardware
- ESP32
- LiPo-batteri (valgfritt)
- TP4056 lader (valgfritt)
- RGB LED (NeoPixel)
- Reset-knapp (GPIO 7)

## Funksjoner
- LED viser:
  - Rødt: Program
  - Grønt: Preview
  - Gult blink: Tilkobler
  - Blinker: Feil
- WiFi fallback/AP-modus for enkel oppsett
- Robust mot strømbrudd og nettverksfeil

## Arkitektur
- Modulær firmware: config, nettverk, provider, LED
- Flyt: WiFi → Provider → TallyState → LED

## Roadmap
- vMix-støtte (MVP)
- OBS/ATEM-støtte (kommer)
- OTA-oppdatering, strømoptimalisering (senere)

## Lisens
MIT — se LICENSE
