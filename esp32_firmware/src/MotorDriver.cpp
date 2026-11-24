#include "MotorDriver.h"

MotorDriver::MotorDriver(int enaRight, int rPin1, int rPin2,
                         int enaLeft,  int lPin1, int lPin2,
                         int rightChannel, int leftChannel)
  : _enaRight(enaRight), _rPin1(rPin1), _rPin2(rPin2),
    _enaLeft(enaLeft),   _lPin1(lPin1), _lPin2(lPin2),
    _rightChannel(rightChannel), _leftChannel(leftChannel) {}

void MotorDriver::begin() {
  pinMode(_enaRight, OUTPUT);
  pinMode(_rPin1, OUTPUT);
  pinMode(_rPin2, OUTPUT);

  pinMode(_enaLeft, OUTPUT);
  pinMode(_lPin1, OUTPUT);
  pinMode(_lPin2, OUTPUT);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(_enaRight, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(_enaLeft, PWM_FREQ, PWM_RESOLUTION);
#else
  ledcSetup(_rightChannel, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(_leftChannel, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(_enaRight, _rightChannel);
  ledcAttachPin(_enaLeft, _leftChannel);
#endif

  // Pastikan motor diam saat ESP32 pertama kali menyala
  stop();
}

void MotorDriver::rotate(int rightMotorSpeed, int leftMotorSpeed) {
  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);
  leftMotorSpeed  = constrain(leftMotorSpeed, -255, 255);

  setSingleMotor(_enaRight, _rPin1, _rPin2, _rightChannel, rightMotorSpeed);
  setSingleMotor(_enaLeft,  _lPin1, _lPin2, _leftChannel,  leftMotorSpeed);
}

void MotorDriver::stop() {
  rotate(0, 0);
}

void MotorDriver::setSingleMotor(int enaPin, int pin1, int pin2, int pwmChannel, int speed) {
  if (speed > 0) {
    // Putar maju
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (speed < 0) {
    // Putar mundur
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  } else {
    // Berhenti
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(enaPin, abs(speed));
#else
  ledcWrite(pwmChannel, abs(speed));
#endif
}