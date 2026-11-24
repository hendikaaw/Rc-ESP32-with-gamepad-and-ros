import time
from gamepad_reader import GamepadReader
from bt_comm import BluetoothComm

# Ganti sesuai port Outgoing Bluetooth ESP32 di PC:
# Windows contoh: "COM6" | Linux contoh: "/dev/rfcomm0" | macOS: "/dev/tty.ESP32_RC_CAR"
BT_PORT = "COM6"
SEND_INTERVAL_SEC = 0.05  # 20 Hz (setiap 50 ms)


def main():
    gamepad = GamepadReader(deadzone=0.15)
    bt = BluetoothComm(port=BT_PORT)

    if not gamepad.begin():
        return

    if not bt.connect():
        gamepad.close()
        return

    print("=== Kontrol Aktif! Tekan Ctrl+C untuk berhenti ===")
    try:
        while True:
            left_spd, right_spd = gamepad.read_motor_speeds()

            if not bt.send_motor_command(left_spd, right_spd):
                break

            print(f"\rKirim -> Kiri: {left_spd:4d} | Kanan: {right_spd:4d}   ", end="")
            time.sleep(SEND_INTERVAL_SEC)

    except KeyboardInterrupt:
        print("\n[MAIN] Program dihentikan oleh pengguna.")
    finally:
        bt.close()
        gamepad.close()


if __name__ == "__main__":
    main()