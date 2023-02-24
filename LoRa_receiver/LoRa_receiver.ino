#include "SoftwareSerial.h"

// LAPTOP
//#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.h"
//#include "C:\Users\jocke\OneDrive\Skrivbord\GitHub\LoRa-communication\RAK811.cpp"

// STATIONARY
#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.h"
#include "C:\Users\jocke\Desktop\GitStuff\LoRa-communication\RAK811.cpp"


#define TXpin 11 
#define RXpin 10
#define WORK_MODE LoRaP2P   //  LoRaWAN or LoRaP2P
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define DebugSerial Serial
SoftwareSerial RAKSerial(RXpin,TXpin);    // Declare a virtual serial port
int RESET_PIN = 12;
int RECEIVED_PIN = 13;
RAK811 RAKLoRa(RAKSerial,DebugSerial);

void setup() 
{
  pinMode(RESET_PIN, OUTPUT);
  pinMode(RECEIVED_PIN, OUTPUT);
  
  Serial.begin(9600);
  DebugSerial.begin(9600);
  
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


  //RAKLoRa.rk_reset(0);

  DebugSerial.println("Current version: " + RAKLoRa.rk_currentVersion());

  /*   Workmode 0 = LoRaWAN, 1 = P2P    */
  DebugSerial.println("Initializing workmode");
 
  RAKLoRa.rk_setWorkingMode(1); //Sets work mode to P2P
  DebugSerial.println("Current mode: " + RAKLoRa.rk_getCurrentMode());
  delay(200);
  DebugSerial.println("Work mode initialized");


  DebugSerial.println("Initializing p2p");
  String P2pInitialized = RAKLoRa.rk_initP2P("869525000", "12", "0", "1", "30", "14");
  DebugSerial.println("P2P initialized: " + P2pInitialized);

  
  DebugSerial.println("Receiving packets status: " + RAKLoRa.rk_recvP2PData(1));

  //String setUART = RAKLoRa.rk_setUARTConfig(9600, 8, 0, 0, 0);
  //DebugSerial.println("UART conf. successful: " + String(setUART));
}

void loop() 
{


  if(RAKSerial.available())
  {
    String filtered_data = "";
    String data = RAKSerial.readString();
    RAKSerial.flush();

    // Filters data "at+recv=0,0,-73,24,2,1029" -> filtered_data "1029"
    for(int i = data.length()-1; i >= 0; i--)
    {
      String substring = data.substring(i-1,i);

      if(substring.equals(","))
      {
        break;
      }
      else
      {
        filtered_data = substring + filtered_data;
      }
    }
   // Serial.println(data);
    // Print filered_data which can be read through Serial (UART)
    Serial.println(filtered_data);
    digitalWrite(RECEIVED_PIN, LOW);
  }
  digitalWrite(RECEIVED_PIN, HIGH);
  //digitalWrite(RECEIVED_PIN, LOW);
}

