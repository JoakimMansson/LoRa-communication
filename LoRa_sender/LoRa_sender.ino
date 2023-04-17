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
  while(Wire.available())
  {
    char c = Wire.read();
    if(c == ' ')
    {
      data = data_segment + ' ' + data;
      data_segment = "";
    }
    else
    {
      data_segment += c;
    }
  }

  debugln("Data: " + data);


  String ID = "";
  for(int i = data.length(); i >= 0; i--)
  {
    if(i-1 < 0) break;
    
    String substr = data.substring(i-1, i);
    if(substr != " ")
    {
      ID += substr;
    }
    else
    {
      data = data.substring(0, i); // DOUBLE CHECK
      break;
    }
  }

  //update_data(ID, data);  
  //Serial.print("Unfiltered data: ");
  //Serial.println(data);
  //filter_data(data);
}


// Input data ex: 8 130 8 5 115 130 8 210
// Start byte ex: 3 -> 8
// Nr bytes ex: 2
// OUTPUT ex: 8 5
String get_data_substr(String input_data, int start_byte, int nr_bytes)
{
  // To extract correct string index
  int str_start_index = 0;
  int nr_bytes_counter = 0;
  if(start_byte != 0)
  {
    for(int i = 0; i < input_data.length(); i++)
    {
      if(nr_bytes_counter == start_byte)
      {
        str_start_index = i;
        break;
      }
      if(input_data.substring(i,i+1) == " ")
      {
        nr_bytes_counter += 1;
      }
      
    }
  }
    
  String extracted_data = "";
  nr_bytes_counter = 0;
  int current_index = str_start_index;
 while(nr_bytes_counter < nr_bytes)
  {
    if(input_data.substring(current_index, current_index+1) == " ")
    {
      nr_bytes_counter += 1;
    }
    current_index += 1;
    
    if(current_index >= input_data.length())
    {
      // Reached end of input string
      break;
    }
  }

  extracted_data = input_data.substring(str_start_index, current_index);
  return extracted_data;
}

uint32_t get_bits_value(uint32_t data, int start_bit, int end_excluding_bit)
{
  String bit_string = String(data, BIN);

  if(start_bit == end_excluding_bit)
  {
    return byte(atoi(bit_string.substring(start_bit, start_bit+1).c_str()));
  }
  else
  {
    return bit_string_to_int(bit_string.substring(start_bit, end_excluding_bit));
  }
}

uint32_t bit_string_to_int(String bit_string)
{
  debugln("bit_string: " + bit_string);
  uint32_t result = 0;
  for (int i = 0; i < bit_string.length(); i++) 
  {
    result = result << 1;
    if (bit_string.charAt(i) == '1') 
    {
      result |= 1;
    }
  }
  return result;
}

uint32_t reverse_bits(uint32_t bit)
{
  
  return 0;
}


void update_data(String ID, String data)
{
  
  debug("ID: " + ID);
  debug(", data: " + data);

  // Convert string to binary
  uint32_t binary_value = 0;
  char *value_token;
  value_token = strtok((char*)data.c_str(), " ");
  for (int i = 0; i < 8; i++) {
    byte value_byte = byte(atoi(value_token));
    bitWrite(binary_value, i * 8, value_byte);
    value_token = strtok(NULL, " ");
  }
  
  // Print binary value

  if(ID == "1025") //0x401 Status Information
  {
  }

  if(ID.compareTo("1026") == 0) //0x402 Bus Measurement
  {
    uint32_t bus_current = get_bits_value(binary_value, 0, 32);
    uint32_t bus_voltage = get_bits_value(binary_value, 32, 64);
    

    debugln("Bus voltage un-reversed: " + String(bus_voltage));
    debugln("Bus current un-reversed: " + String(bus_current));

    bus_voltage = reverse_bits(bus_voltage);
    bus_current = reverse_bits(bus_current);

    debugln("Bus voltage reversed: " + String(bus_voltage));
    debugln("Bus current reversed: " + String(bus_current));
  }

  if(ID.compareTo("1027") == 0) //0x403 Velocity Measurement
  {
    uint32_t motor_velocity = get_bits_value(binary_value, 0, 32);
    uint32_t vehicle_velocity = get_bits_value(binary_value, 32, 64);
    debugln("Motor vel: " + motor_velocity);
    debugln("Vehicle vel: "  + vehicle_velocity);
  }
  if(ID.compareTo("1035") == 0) //0x40B MC Temperatures
  {
    uint32_t motor_temp = get_bits_value(binary_value, 0, 32);
    uint32_t heatsink_temp = get_bits_value(binary_value, 32, 64);
    debugln("Motor vel: " + motor_temp);
    debugln("Vehicle vel: "  + heatsink_temp);
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


