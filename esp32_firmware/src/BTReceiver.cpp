#include "BTReceiver.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth Classic tidak aktif pada konfigurasi board ini!
#endif

bool BTReceiver::begin(const char* deviceName, unsigned long timeoutMs) {
  _timeoutMs = timeoutMs;
  _rxBuffer.reserve(32);
  _currentCmd = {0, 0, false};
  return _btSerial.begin(deviceName);
}

void BTReceiver::update() {
  _currentCmd.isConnected = _btSerial.hasClient();

  if (!_currentCmd.isConnected) {
    _currentCmd.leftSpeed = 0;
    _currentCmd.rightSpeed = 0;
    _rxBuffer = "";
    return;
  }

  // Baca karakter hingga menemukan baris baru ('\n')
  while (_btSerial.available()) {
    char c = static_cast<char>(_btSerial.read());
    if (c == '\n') {
      parseLine(_rxBuffer);
      _rxBuffer = "";
    } else if (c != '\r' && _rxBuffer.length() < 30) {
      _rxBuffer += c;
    }
  }

  // Watchdog: hentikan motor jika tidak ada paket masuk dalam batas waktu
  if (millis() - _lastPacketTime > _timeoutMs) {
    _currentCmd.leftSpeed = 0;
    _currentCmd.rightSpeed = 0;
  }
}

void BTReceiver::parseLine(const String& line) {
  int leftVal = 0;
  int rightVal = 0;

  // Mem-parsing format "M:<kiri>,<kanan>"
  if (sscanf(line.c_str(), "M:%d,%d", &leftVal, &rightVal) == 2) {
    _currentCmd.leftSpeed  = constrain(leftVal, -255, 255);
    _currentCmd.rightSpeed = constrain(rightVal, -255, 255);
    _lastPacketTime = millis();
  }
}

MotorCommand BTReceiver::getCommand() {
  return _currentCmd;
}