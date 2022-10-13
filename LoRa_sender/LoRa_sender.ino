#include "RAK811.h"
//Include Serial Library
#include "SoftwareSerial.h"

//Set here if you want to use LoRaWAN or P2P Modes
#define WORK_MODE LoRaP2P   //  LoRaWAN or LoRaP2P
//This sets the code to ABP
#define JOIN_MODE ABP


//Specify Data Rate
//For EU & AU This is recommended to be 5 for SF7, For US use 4
String dataRate = "5";


//The Pi Supply shield uses pins 11 & 10 for serial.
#define TXpin 11 
#define RXpin 10
#define RESET_PIN 12
//The DebugSerial allows the board to respond back via the USB Serial
#define DebugSerial Serial
//Setup the serial port for the board
SoftwareSerial RAKSerial(RXpin,TXpin);    // Declare a virtual serial port


bool InitLoRaWAN(void);
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


