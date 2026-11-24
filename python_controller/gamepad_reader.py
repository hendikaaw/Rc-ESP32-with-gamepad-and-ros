import pygame


class GamepadReader:
    def __init__(self, deadzone: float = 0.15):
        self.deadzone = deadzone
        self.joystick = None

    def begin(self) -> bool:
        pygame.init()
        pygame.joystick.init()

        if pygame.joystick.get_count() == 0:
            print("[GAMEPAD] Tidak ada stik terdeteksi. Hubungkan gamepad terlebih dahulu.")
            return False

        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()
        print(f"[GAMEPAD] Terhubung ke: {self.joystick.get_name()}")
        return True

    def _apply_deadzone(self, value: float) -> float:
        return 0.0 if abs(value) < self.deadzone else value

    def read_motor_speeds(self) -> tuple[int, int]:
        """Membaca analog kiri dan mengembalikan (left_speed, right_speed) dalam rentang -255..255."""
        pygame.event.pump()

        if self.joystick is None:
            return 0, 0

        # Tombol 0 (Cross di PS / A di Xbox) berfungsi sebagai rem darurat
        if self.joystick.get_numbuttons() > 0 and self.joystick.get_button(0):
            return 0, 0

        # Axis 0: Kiri (-1.0) s.d. Kanan (+1.0)
        # Axis 1: Atas (-1.0) s.d. Bawah (+1.0) -> dibalik agar maju bernilai positif
        steering = self._apply_deadzone(self.joystick.get_axis(0))
        throttle = self._apply_deadzone(-self.joystick.get_axis(1))

        # Pencampuran kecepatan roda kiri & kanan (Arcade Drive)
        left_raw = max(-1.0, min(1.0, throttle + steering))
        right_raw = max(-1.0, min(1.0, throttle - steering))

        left_speed = int(left_raw * 255)
        right_speed = int(right_raw * 255)
        return left_speed, right_speed

    def close(self):
        pygame.quit()