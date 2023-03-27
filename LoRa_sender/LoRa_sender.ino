#include "SoftwareSerial.h"

// LAPTOP
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.h"
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.cpp"

// STATIONARY
//#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.h"
//#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.cpp"


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

  //DebugSerial.print("Sending packets status: " + RAKLoRa.rk_sendP2PData("100", "1000", "1337"));
}

String filter_data(String input_str)
{
  String new_str = "";
  for(int i = 0; i < input_str.length(); i++)
  {
    unsigned int ascii_val = input_str.charAt(i);
    if(ascii_val >= 48 && ascii_val <=  57)
    {
      new_str += input_str.substring(i, i+1);
    }
  }
  return new_str;
}

void loop() 
{

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
  }
  else
  {
    char data[] = "30133769";
    RAKLoRa.rk_sendP2PData(1, "10", data);
    delay(200);
  }
}


