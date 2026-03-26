# Brukerguide: ESP32 Tally Light

Denne guiden hjelper deg fra start til ferdig oppsett av ESP32 Tally Light.

## Innhold
- [Hva er ESP32 Tally Light?](#hva-er-esp32-tally-light)
- [Krav og utstyr](#krav-og-utstyr)
- [Bygg og last opp firmware](#bygg-og-last-opp-firmware)
- [Førstegangsoppsett](#førstegangsoppsett)
- [Bruk](#bruk)
- [Feilsøking](#feilsøking)
- [FAQ](#faq)
- [Eksempler](#eksempler)

---

## Hva er ESP32 Tally Light?
Enkel, trådløs tally-lampe for vMix, OBS og ATEM. Gir visuell indikasjon (Program/Preview) med én RGB LED. Konfigureres via WiFi-portal.

## Krav og utstyr
- ESP32 (f.eks. ESP32-S3)
- RGB LED (NeoPixel)
- Reset-knapp (GPIO 7)
- (Valgfritt) LiPo-batteri og lader
- USB-kabel
- PC med PlatformIO

## Bygg og last opp firmware
1. Klon repoet og åpne i VS Code med PlatformIO.
2. Koble til ESP32 via USB.
3. Kjør:
   ```bash
   platformio run
   platformio run --target upload
   ```

## Førstegangsoppsett
1. Start enheten. Den oppretter et WiFi-nettverk (AP-modus).
2. Koble til nettverket fra PC/mobil.
3. Åpne konfigurasjonssiden i nettleser (vanligvis http://192.168.4.1).
4. Fyll inn WiFi, velg produksjonssystem (vMix/OBS/ATEM), og lagre.
5. Enheten kobler til og er klar til bruk.

## Bruk
- LED viser:
  - Rødt: Program (live)
  - Grønt: Preview
  - Gult blink: Tilkobler
  - Blinker: Feil
- Bytt produksjonssystem via webportal ved behov.
- Reset-knapp kan brukes for å nullstille config.

## Feilsøking
- LED blinker raskt: Sjekk WiFi eller config.
- Kommer ikke inn på portal: Hold inne reset og start på nytt.
- Ingen tally: Sjekk at riktig provider er valgt og at produksjonssystemet kjører.

## FAQ
**Kan jeg bruke andre LED-typer?**
- Kun NeoPixel (WS2812) støttes per nå.

**Kan jeg bruke batteri?**
- Ja, men sørg for sikker lading.

**Hvordan oppdaterer jeg firmware?**
- Last opp ny firmware via PlatformIO (OTA kommer snart).

## Eksempler
- Typisk oppsett: ESP32 + NeoPixel + knapp, montert på kamera.
- Flere tally-enheter kan brukes samtidig.

---

Se også [docs/index.md] for mer informasjon og roadmap.
