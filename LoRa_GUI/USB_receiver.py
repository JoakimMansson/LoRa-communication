import serial
from serial.tools import list_ports


encodings = {
            #BMS_Pack
            10: "PackCurrent",
            11: "PackVoltage",
            12: "PackStateOfHealth",
            13: "AvgPackCurrent",
            14: "PackStateOfCharge",
            #BMS_Cell
            15: "LowCellVoltage",
            16: "HighCellVoltage",
            17: "AvgCellVoltage",
            #BMS_Failsafes
            18: "VoltageFailsafeActive",
            19: "CurrentFailsafeActive",
            20: "RelayFailsafeActive",
            21: "CellBalancingActive",
            22: "ChangeinterlockFailsafeActive",
            23: "ThermistorB_valueTableInvalid",
            24: "InputPowerSupplyFailed",
            #BMS_Temperatures
            25: "HighestTemperature",
            26: "LowestTemperature",
            27: "AverageTemperature",
            28: "InternalTemperature",

            #MC_Temperatures
            29: "HeatsinkTemperature",
            30: "MotorTemperature",
            #MC_CurrentVoltage
            31: "BusCurrent",
            32: "BusVoltage",
            #MC_Velocity
            33: "MotorVelocity",
            34: "VehicleVelocity",
            #MC_ErrorFlags
            35: "MotorOverSpeed",
            36: "DesaturationFault",
            37: "RailUnderVoltage",
            38: "ConfigReadError",
            39: "WatchdogCausedLastReset",
            40: "BadMotorPositionHallSequence",
            41: "DCBusOverVoltage",
            42: "SoftwareOverCurrent",
            43: "HardwareOverCurrent",
            #MC_LimitFlags
            44: "IPMTemperatureOrMotorTemperature",
            45: "BusVoltageLowerLimit",
            46: "BusVoltageUpperLimit",
            47: "BusCurrentLimit",
            48: "Velocity",
            49: "MotorCurrent",
            50: "OutputVoltagePWM"
        }


class ReadUSB():

    def __init__(self, port=None, baud=9600) -> None:
        if port is None:
            port = self.get_available_port()
        self.serial = serial.Serial(port, baud)
    
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
        data = str(self.serial.readline())

        # Remove all non-digits, ex b'1029\r\r\n' -> 1029
        filtered_data = ''.join(filter(lambda x: x.isdigit(), data))

        return filtered_data
    

if __name__ == '__main__':
    reader = ReadUSB()
    while True:
        data = reader.get_DIGITS_USB()
        print(data)

        

