#include "SoftwareSerial.h"
#include "string.h"
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.h"
#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.cpp"


#define TXpin 11 
#define RXpin 10
#define WORK_MODE LoRaP2P   //  LoRaWAN or LoRaP2P
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define DebugSerial Serial
SoftwareSerial RAKSerial(RXpin,TXpin);    // Declare a virtual serial port
int RESET_PIN = 12;
int ERROR_PIN = 13;
int SERIAL_AVAILABLE_PIN = 9;
RAK811 RAKLoRa(RAKSerial,DebugSerial);

void setup() 
{
  pinMode(RESET_PIN, OUTPUT);
  pinMode(ERROR_PIN, OUTPUT);
  pinMode(SERIAL_AVAILABLE_PIN, OUTPUT);
  
  DebugSerial.begin(9600);
  Serial.begin(9600);
  
  while(DebugSerial.read()>= 0) {}  
  while(!DebugSerial);

  DebugSerial.println("StartUP");
  DebugSerial.println("Reset");

  digitalWrite(RESET_PIN, LOW);   // turn the pin low to Reset
  digitalWrite(RESET_PIN, HIGH);    // then high to enable
  DebugSerial.println("Success");

  //DebugSerial.println("Exit boot mode: " + RAKLoRa.rk_exitBootMode());

  RAKSerial.begin(9600); // Arduino Shield
  delay(200);
  DebugSerial.println(RAKLoRa.rk_getBand());
  delay(200);

  DebugSerial.println("Current version: " + RAKLoRa.rk_currentVersion());

  /*   Workmode 0 = LoRaWAN, 1 = P2P    */
  DebugSerial.println("Initializing workmode");

  RAKLoRa.rk_setWorkingMode(1); //Sets work mode to P2P
  DebugSerial.println("Current mode: " + RAKLoRa.rk_getCurrentMode());
  delay(200);
  DebugSerial.println("Work mode initialized");


  DebugSerial.println("Initializing p2p");
  String P2pInitialized = RAKLoRa.rk_initP2P("869525000", "10", "0", "1", "8", "14");
  DebugSerial.println("P2P initialized: " + P2pInitialized);


  //DebugSerial.print("Sending packets status: " + RAKLoRa.rk_sendP2PData("100", "1000", "1337"));
}

int i = 10;
void loop() 
{
  digitalWrite(SERIAL_AVAILABLE_PIN, HIGH);
  while(!Serial.available());
  String status = RAKLoRa.rk_sendP2PData("1", "1", String(Serial.read()));
  digitalWrite(SERIAL_AVAILABLE_PIN, LOW);
  //DebugSerial.println("Sending packets status: " + status);

  if(!status.compareTo("ERROR-1"))
  {
    digitalWrite(ERROR_PIN, HIGH);
  }
  else
  {
    digitalWrite(ERROR_PIN, LOW);
  }

  //i++;
  delay(200);
}


