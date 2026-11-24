import serial
import time


class BluetoothComm:
    def __init__(self, port: str, baudrate: int = 115200):
        self.port = port
        self.baudrate = baudrate
        self.conn = None

    def connect(self) -> bool:
        try:
            self.conn = serial.Serial(self.port, self.baudrate, timeout=0.1, write_timeout=0.5)
            time.sleep(1.5)  # Jeda singkat agar jalur serial stabil
            print(f"[BT] Terhubung ke ESP32 pada port {self.port}")
            return True
        except serial.SerialException as err:
            print(f"[BT] Gagal membuka port {self.port}: {err}")
            return False

    def send_motor_command(self, left_speed: int, right_speed: int) -> bool:
        if self.conn is None or not self.conn.is_open:
            return False

        # Format paket: M:<kiri>,<kanan>\n
        packet = f"M:{left_speed},{right_speed}\n"
        try:
            self.conn.write(packet.encode("ascii"))
            return True
        except serial.SerialException as err:
            print(f"[BT] Koneksi terputus saat mengirim data: {err}")
            return False

    def close(self):
        if self.conn and self.conn.is_open:
            # Kirim perintah berhenti sebelum memutus koneksi
            self.send_motor_command(0, 0)
            self.conn.close()
            print("[BT] Koneksi Bluetooth ditutup.")