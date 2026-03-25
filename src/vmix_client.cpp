#include "vmix_client.h"

VmixClient::VmixClient()
  : port_(AppConfig::DEFAULT_VMIX_PORT),
    channel_(1),
    lastConnectAttemptMs_(0),
    lastDataMs_(0),
    lastPollMs_(0),
    connected_(false),
    tallyState_(TallyState::Off) {}

void VmixClient::begin(const DeviceConfig& config) {
  host_ = config.vmixIp;
  port_ = config.vmixPort;
  channel_ = config.channel;

  disconnect();
}

void VmixClient::disconnect() {
  if (client_.connected()) {
    client_.stop();
  }

  connected_ = false;
  tallyState_ = TallyState::Off;
  rxLine_ = "";
}

bool VmixClient::isConnected() const {
  return connected_;
}

TallyState VmixClient::getTallyState() const {
  return tallyState_;
}

bool VmixClient::connectIfNeeded() {
  if (client_.connected()) {
    connected_ = true;
    return true;
  }

  connected_ = false;

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  const unsigned long now = millis();
  if (now - lastConnectAttemptMs_ < AppConfig::VMIX_RETRY_MS) {
    return false;
  }

  lastConnectAttemptMs_ = now;

  Serial.print("[vMix] Connecting to ");
  Serial.print(host_);
  Serial.print(":");
  Serial.println(port_);

  if (!client_.connect(host_.c_str(), port_)) {
    Serial.println("[vMix] Connect failed");
    return false;
  }

  client_.setNoDelay(true);
  connected_ = true;
  lastDataMs_ = millis();
  lastPollMs_ = millis();

  client_.print("SUBSCRIBE TALLY\r\n");
  client_.print("TALLY\r\n");

  Serial.println("[vMix] Connected");
  return true;
}

void VmixClient::applyTallyString(const String& values) {
  const int index = static_cast<int>(channel_) - 1;

  if (index < 0 || index >= static_cast<int>(values.length())) {
    tallyState_ = TallyState::Off;
    return;
  }

  const char state = values.charAt(index);

  switch (state) {
    case '1':
      tallyState_ = TallyState::Program;
      break;
    case '2':
      tallyState_ = TallyState::Preview;
      break;
    default:
      tallyState_ = TallyState::Off;
      break;
  }
}

void VmixClient::handleLine(const String& line) {
  if (line.length() == 0) {
    return;
  }

  Serial.print("[vMix RX] ");
  Serial.println(line);

  if (line.startsWith("TALLY OK ")) {
    String values = line.substring(9);
    values.trim();
    applyTallyString(values);
  }
}

void VmixClient::readIncoming() {
  while (client_.connected() && client_.available()) {
    const char c = static_cast<char>(client_.read());
    lastDataMs_ = millis();

    if (c == '\n') {
      rxLine_.trim();
      handleLine(rxLine_);
      rxLine_ = "";
    } else if (c != '\r') {
      rxLine_ += c;
    }
  }
}

void VmixClient::loop() {
  if (!connectIfNeeded()) {
    return;
  }

  readIncoming();

  const unsigned long now = millis();

  if (now - lastPollMs_ > AppConfig::VMIX_POLL_MS) {
    client_.print("TALLY\r\n");
    lastPollMs_ = now;
  }

  if (now - lastDataMs_ > AppConfig::VMIX_DATA_TIMEOUT_MS) {
    Serial.println("[vMix] Timeout, disconnecting");
    disconnect();
  }
}