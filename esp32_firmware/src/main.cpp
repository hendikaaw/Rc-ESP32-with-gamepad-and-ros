#include <Arduino.h>
#include "BTReceiver.h"
#include "MotorDriver.h"

// Pinout aman ESP32 WROOM-32D ke Driver L298N
#define ENABLE_RIGHT_MOTOR 22  // ENA
#define RIGHT_MOTOR_PIN1   16  // IN1
#define RIGHT_MOTOR_PIN2   17  // IN2

#define ENABLE_LEFT_MOTOR  23  // ENB
#define LEFT_MOTOR_PIN1    18  // IN3
#define LEFT_MOTOR_PIN2    19  // IN4

#define BT_DEVICE_NAME     "ESP32_RC_CAR"
#define FAILSAFE_TIMEOUT   500  // Stop otomatis jika 500 ms tanpa sinyal

BTReceiver btReceiver;
MotorDriver motors(
  ENABLE_RIGHT_MOTOR, RIGHT_MOTOR_PIN1, RIGHT_MOTOR_PIN2,
  ENABLE_LEFT_MOTOR,  LEFT_MOTOR_PIN1,  LEFT_MOTOR_PIN2
);

bool lastConnState = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== RC Car ESP32 (C++ Firmware + Python Controller) ===");

  motors.begin();

  if (btReceiver.begin(BT_DEVICE_NAME, FAILSAFE_TIMEOUT)) {
    Serial.print("[BT] Menunggu koneksi Python ke: ");
    Serial.println(BT_DEVICE_NAME);
  } else {
    Serial.println("[BT] Gagal menginisialisasi Bluetooth!");
  }
}

void loop() {
  // 1. Terima & parse paket dari Python
  btReceiver.update();

  // 2. Ambil perintah kecepatan terbaru
  MotorCommand cmd = btReceiver.getCommand();

  // 3. Log perubahan status koneksi
  if (cmd.isConnected != lastConnState) {
    lastConnState = cmd.isConnected;
    Serial.println(cmd.isConnected ? "[BT] Python Controller terhubung!"
                                   : "[BT] Koneksi putus! Motor dihentikan.");
  }

  // 4. Jalankan motor (urutan parameter rotate: kanan, kiri)
  motors.rotate(cmd.rightSpeed, cmd.leftSpeed);

  delay(10);
}