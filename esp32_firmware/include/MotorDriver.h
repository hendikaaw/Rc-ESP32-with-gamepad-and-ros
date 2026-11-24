#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
  MotorDriver(int enaRight, int rPin1, int rPin2,
              int enaLeft,  int lPin1, int lPin2,
              int rightChannel = 4, int leftChannel = 5);

  void begin();
  void rotate(int rightMotorSpeed, int leftMotorSpeed);
  void stop();

private:
  int _enaRight, _rPin1, _rPin2;
  int _enaLeft,  _lPin1, _lPin2;
  int _rightChannel, _leftChannel;

  static const int PWM_FREQ = 1000;     // Frekuensi PWM 1 kHz (sesuai karakter L298N)
  static const int PWM_RESOLUTION = 8;  // Resolusi 8-bit (duty cycle 0 s.d. 255)

  void setSingleMotor(int enaPin, int pin1, int pin2, int pwmChannel, int speed);
};

#endif