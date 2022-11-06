#include "SoftwareSerial.h"

#define TXpin 11 
#define RXpin 10


RAK811 RAKLoRa(RAKSerial,DebugSerial);


void setup() 
{
  RAKSerial.begin(9600);
  DebugSerial.begin(9600);
  Serial.begin(9600);

  // mode = 1: sets LoRaP2P
  RAKLoRa.rk_setWorkingMode(1);
  RAKLoRa.rk_setConfig("lorap2p:transfer_mode", "1");
  //lorap2p:869525000:7:0:1:5:5
  RAKLoRa.rk_initP2P("869525000", 7, 0, 1, 5, 5);
}

void loop() 
{

}


