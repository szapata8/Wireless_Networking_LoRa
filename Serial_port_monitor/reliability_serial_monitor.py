import serial
import serial.tools.list_ports
import serial.serialutil
import json
import time

BAUD_RATE = 9600
COM_PORT = "COM3"
ESP32_ID = "PID=10C4:EA60"
INITIALIZATION_MESSAGE_LENGTH = 42

FILL_UP_COM_PORT_TIME = 0.1 # unit: seconds
WAIT_BETWEEN_TRANSMISSION = 2 # unit: seconds


def find_esp32_port():
    ports = list(serial.tools.list_ports.comports())
    for i, port in enumerate(ports):
        # print(f"port #{i}: {port}. hwid: {port.hwid}. name: {port.name}. description: {port.description}. device: {port.device}")
        if ESP32_ID in port.hwid:
            return port.name
    print("ERROR: COM port for ESP32 not found")
    return None

class Reliability_ESP32_listener:
    def __init__(self):
        self.com_port_name = find_esp32_port()
        self.com_port = serial.Serial(port = self.com_port_name, baudrate = BAUD_RATE, timeout=1)
        while self.com_port.inWaiting() < INITIALIZATION_MESSAGE_LENGTH:
            # time.sleep(1)
            continue
        bytesToRead = self.com_port.inWaiting() 
        response = self.com_port.read(bytesToRead)
        self.com_port.flushInput()
        print(f"ESP32 detected: {response}")
        self.done_receiving = False
        self.results = list()

    def monitor_serial_port(self):

        transmission_counter = 0

        while True:

            # Listen to com port until theres a #### (Starting frame)
            [bytesToRead, response] = self.read_buffer()
            while "#" not in str(response):
                [bytesToRead, response] = self.read_buffer()

            for i in range(7):

                # Wait WAIT_BETWEEN_TRANSMISSION (2000 ms)
                time.sleep(WAIT_BETWEEN_TRANSMISSION)

                # wait 100ms
                time.sleep(FILL_UP_COM_PORT_TIME)

                # read whatever is in the com port
                [bytesToRead, packet_received] = self.read_buffer()
                print(f"[bytesToRead, packet_received]: \n{[bytesToRead, str(packet_received)]}")
                self.results.append(str(packet_received))

            print("done with transmission")
            print(f"self.results: \n{self.results}")
            transmission_counter += 1
            results_file = open(f"reliability_results\\reliability_{transmission_counter}.json", "a")
            json.dump(self.results, results_file)
            print("Results recorded on file")
            results_file.close()
            self.results = list()

    def read_buffer(self):
        bytesToRead = self.com_port.inWaiting() 
        response = self.com_port.read(bytesToRead)
        return [bytesToRead, response]

    def done(self):
        self.com_port.close()


if __name__ == "__main__":
    esp32 = Reliability_ESP32_listener()
    esp32.monitor_serial_port()
    esp32.done()



