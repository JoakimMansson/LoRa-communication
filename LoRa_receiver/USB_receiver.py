import serial
import re

port_name = "COM9"

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
    data = getRawData()
    if data != None:
        print(data)
    
    
    #data = getRawData()
    #if data == None:
    #    pass
    #else:
    #    print(data)

