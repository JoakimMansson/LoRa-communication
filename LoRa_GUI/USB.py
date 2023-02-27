import serial
from serial.tools import list_ports
import time

from LoRa_Converter import LoRaConverter



class USB():

    def __init__(self, port=None, baud=9600) -> None:
        if port is None:
            port = self.get_available_port()
        self.ser = serial.Serial(port, baud)
    
    def get_available_port(self):
        ports = list(list_ports.comports())
        for port in ports:
            try:
                temp_ser = serial.Serial(port.device, 9600)
                temp_ser.close()
                return port.device
            except Exception as e:
                print("Port " + str(port.device) + " not available")

        return None
    

    def print_all_ports(self):
        ports = list(list_ports.comports())
        ports_str = ""
        for port in ports:
            ports_str += "[" + port.device + "]"

        if ports_str == "":
            print("None available ports")
        
        print(ports_str)

    def get_DIGITS_USB(self):
        data = str(self.ser.readline())

        # Remove all non-digits, ex b'1029\r\r\n' -> 1029
        #filtered_data = ''.join(filter(lambda x: x.isdigit(), data))

        return data
    
    def send_string(self, data: str):
        data_bytes = data.encode()
        self.ser.write(data_bytes)
    

if __name__ == '__main__':
    reader = USB()
    converter = LoRaConverter(define_comma_value="99", define_new_value="69")
    while True:
        data = reader.get_DIGITS_USB()
        if data != "":
            print(data)#,converter.unparse_value(data))

