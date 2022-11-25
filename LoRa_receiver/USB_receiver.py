import serial
import re

port_name = "COM10"

encodings = {
    #BMS_Pack
    10: "PackCurrent",
    11: "PackVoltage",
    12: "PackStateOfHealth",
    13: "AvgPackCurrent",
    #BMS_Cell
    14: "LowCellVoltage",
    15: "HighCellVoltage",
    16: "AvgCellVoltage",
    #BMS_Failsafes
    17: "VoltageFailsafeActive",
    18: "CurrentFailsafeActive",
    19: "RelayFailsafeActive",
    20: "CellBalancingActive",
    21: "ChangeinterlockFailsafeActive",
    22: "ThermistorB_valueTableInvalid",
    23: "InputPoweruSupplyFailed",
    #BMS_Temperatures
    24: "HighestTemperature",
    25: "LowestTemperature",
    26: "AverageTemperature",
    27: "InternalTemperature",

    #MC_Temperatures
    28: "HeatsinkTemperature",
    29: "MotorTemperature",
    #MC_CurrentVoltage
    30: "BusCurrent",
    31: "BusVoltage",
    #MC_Velocity
    32: "MotorVelocity",
    33: "VehicleVelocity",
    #MC_ErrorFlags
    34: "MotorOverSpeed",
    35: "DesaturationFault",
    36: "RailUnderVoltage",
    37: "ConfigReadError",
    38: "WatchdogCausedLastReset",
    39: "BadMotorPositionHallSequence",
    40: "DCBusOverVoltage",
    41: "SoftwareOverCurrent",
    42: "HardwareOverCurrent",
    #MC_LimitFlags
    43: "IPMTemperatureOrMotorTemperature",
    44: "BusVoltageLowerLimit",
    45: "BusVoltageUpperLimit",
    46: "BusCurrent",
    47: "Velocity",
    48: "MotorCurrent",
    49: "OutputVoltagePWM"
}

ser = serial.Serial(
    port = port_name,
    baudrate=9600
)

def getAllData():
    data = str(ser.read_all())
    return data

def getRawData():
    data = str(ser.read_all())

    if not contains("recv", data) or not contains("natl", data):
        return None


    index = 0
    counter = 0
    for char in data:
        if char == "r" and counter == 0:
            counter = 1
        elif char == "r" and counter == 1:
            index = index - 2
            break
        
        index = index + 1

    finalData = ""
    print(data)
    while data[index] != ",":
        finalData = data[index] + finalData
        index = index - 1
    
    return finalData

def contains(string: str, txt: str):
    if re.search(string, txt) == None:
        return False
    else:
        return True


while(True):
    #data = getRawData()
    data = str(ser.read_all())
    if data != "b''":
        print(data)
   # if data != None:
        #print(data)
    
    
    #data = getRawData()
    #if data == None:
    #    pass
    #else:
    #    print(data)

