#include "SoftwareSerial.h"
#include <Wire.h>
#include <Arduino.h>

// LAPTOP
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.h"
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.cpp"

// STATIONARY
//#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.h"
//#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.cpp"

// For debugging
#define DEBUG 1

#if DEBUG == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#define TXpin 11 
#define RXpin 10
#define WORK_MODE LoRaP2P   //  LoRaWAN or LoRaP2P
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
SoftwareSerial RAKSerial(RXpin,TXpin);    // Declare a virtual serial port
int RESET_PIN = 12;
int ERROR_PIN = 13;
int SERIAL_AVAILABLE_PIN = 9;
RAK811 RAKLoRa(RAKSerial,Serial);

// Motor Controller
float HeatsinkTemp = 0.0;
float MotorTemp = 0.0;

float BusCurrent = 0.0;
float BusVoltage = 0.0;

float MotorVelocity = 0.0;
float VehicleVelocity = 0.0;




void setUART(int current_LoRa_baud, int new_LoRa_baud)
{
  digitalWrite(RESET_PIN, LOW);   // turn the pin low to Reset
  delay(400);
  digitalWrite(RESET_PIN, HIGH);    // then high to enable
  Serial.println("Reset RAK");
  delay(400);

  RAKSerial.begin(current_LoRa_baud); // Arduino Shield
  String uart_status = RAKLoRa.rk_setUARTConfig(new_LoRa_baud, 8, 0, 0, 0);
  Serial.println("UART SET STATUS: " + uart_status);
  
  delay(400);
  digitalWrite(RESET_PIN, LOW);   // turn the pin low to Reset
  delay(400);
  digitalWrite(RESET_PIN, HIGH);    // then high to enable
}


void setup() 
{
  pinMode(RESET_PIN, OUTPUT);
  pinMode(ERROR_PIN, OUTPUT);
  pinMode(SERIAL_AVAILABLE_PIN, OUTPUT);
  pinMode(9, INPUT);
  
  Serial.begin(19200);
  
  while(Serial.read()>= 0) {}  
  while(!Serial);

  Serial.println("StartUP");

  int current_baud = 19200;
  setUART(current_baud, 19200); //Sets UART -> "at+uart=31250,8,0,0,0"

  RAKSerial.begin(19200); // Arduino Shield
  delay(200);
  Serial.println(RAKLoRa.rk_getBand());
  delay(200);

  Serial.println("Current version: " + RAKLoRa.rk_currentVersion());

  /*   Workmode 0 = LoRaWAN, 1 = P2P    */
  Serial.println("Initializing workmode");

  RAKLoRa.rk_setWorkingMode(1); //Sets work mode to P2P
  Serial.println("Current mode: " + RAKLoRa.rk_getCurrentMode());
  delay(200);
  Serial.println("Work mode initialized");


  Serial.println("Initializing p2p");
  String P2pInitialized = RAKLoRa.rk_initP2P("869525000", "9", "2", "1", "8", "20");
  Serial.println("P2P initialized: " + P2pInitialized);

  //String setUART = RAKLoRa.rk_setUARTConfig(9600, 8, 0, 0, 0);
  //DebugSerial.println("UART conf.   successful: " + String(setUART));
  Serial.setTimeout(5);
  RAKSerial.setTimeout(5);

  Wire.begin(8); // Starting I2C communication on channel 8
  Wire.onReceive(I2C_receive); // Setting interrupt to: I2C_receive()
}

// Interrupt function which gets called when data
// is available on I2C
void I2C_receive() 
{
  String data = "";
  String data_segment = "";
  while(Wire.available()) // To read in data backwards (1744 18 0 0 5 188 0 199 114) -> (114 199 0 188 5 0 0 18 1744)
  {
    char c = Wire.read();
    if(c == ' ')
    {
      data += data_segment + ' ';
      data_segment = "";
    }
    else
    {
      data_segment += c;
    }
  }

  
  String ID = "";
  for(int i = 0; i < data.length(); i++)
  {    
    String c = data.substring(i, i+1);
    if(c != " ")
    {
      ID += c;
    }
    else
    {
      data = data.substring(i+1);
      break;
    }
  }
  
  debugln("Data: " + data);
  debugln("ID: " + ID);
  update_data(ID, data);  
  //Serial.print("Unfiltered data: ");
  //Serial.println(data);
  //filter_data(data);
}


/// Function to extract a specified number of bytes from a string and convert to decimal number
double extractBytesToDecimal(String data, int startByte, int numBytes) {
  // Extract the specified number of bytes from the string
  String byteStr = data.substring(startByte, startByte + numBytes);

  // Calculate startbyte index position ex. startByte: 4 = index: 14 (65 160 0 0 68 (250 0 0 1027))
  int startIndex = 0;
  int byteCounter = 0; // Bytes inc. for each " "
  for(int i = 0; i < data.length(); i++)
  {

    if(byteCounter == startByte)
    {
      startIndex = i;
      break;
    }

    if(data.substring(i, i+1) == " ")
    {
      byteCounter++;
    }
  
  }

  debugln("Start index: " + String(startIndex));


  
  byte bytes[numBytes];

  byteCounter = 0;
  String byte_data = "";
  for(int i = startIndex; i < data.length(); i++)
  {

    String data_substr = data.substring(i, i+1);

    if(byteCounter == numBytes)
    {
      break;
    }
    else if(data_substr == " ")
    {
      debugln(byte_data);
      bytes[byteCounter] = (byte) strtoul(byte_data.c_str(), NULL, 10);
      byteCounter++;
      byte_data = "";
    }
    else
    {
      byte_data += data_substr; 
    }

  }


  /* For debugging of output bytes
  for(int i = 0; i < numBytes; i++)
  {
    debug(bytes[i]);
    debug(" ");
  }
  debugln();
  */
  
  float value;
  memcpy(&value, bytes, numBytes);
  // Return the decimal value
  return value;
}

void update_data(String ID, String data)
{


  if(ID == "1025") //0x401 Status Information
  {
  }

  if(ID.compareTo("1026") == 0) //0x402 Bus Measurement
  {

  }

  if(ID.compareTo("1027") == 0) //0x403 Velocity Measurement
  {
    double vehicle_velocity = extractBytesToDecimal(data, 0, 4);
    debugln();
    double motor_velocity = extractBytesToDecimal(data, 4, 4);

    debugln("Vehicle vel: " + String(vehicle_velocity));
    debugln("Motor vel: " + String(motor_velocity));
    
  }
  if(ID.compareTo("1035") == 0) //0x40B MC Temperatures
  {

  }
  
}

void loop() 
{

  //Serial.println("Doin loop");
/*
  if(Serial.available())
  {
    String input = Serial.readStringUntil(".");  // Read the data from the serial port
    Serial.flush();
    char data[input.length()+1];
    Serial.println(input);
   
      // make sure that the new string is null terminated
    data[input.length()] = '\0';
     
    for (int i = 0; i < input.length(); i++) 
    {
        data[i] = input[i];
    }

    // data must > 10
    RAKLoRa.rk_sendP2PData(1, "10", data);
    digitalWrite(ERROR_PIN, HIGH);
    Serial.println(data);
  }
  else
  {
    char data[] = "305069";
    RAKLoRa.rk_sendP2PData(1, "10", data);
    delay(200);
  }
  */
}


