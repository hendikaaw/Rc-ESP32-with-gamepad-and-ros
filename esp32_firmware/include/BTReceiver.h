#ifndef BT_RECEIVER_H
#define BT_RECEIVER_H

#include <Arduino.h>
#include <BluetoothSerial.h>

struct MotorCommand {
  int leftSpeed;
  int rightSpeed;
  bool isConnected;
};

class BTReceiver {
public:
  bool begin(const char* deviceName, unsigned long timeoutMs = 500);
  void update();
  MotorCommand getCommand();

private:
  BluetoothSerial _btSerial;
  String _rxBuffer;
  MotorCommand _currentCmd = {0, 0, false};
  unsigned long _lastPacketTime = 0;
  unsigned long _timeoutMs = 500;

  void parseLine(const String& line);
};

#endif