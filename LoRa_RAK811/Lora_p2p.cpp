#include "Arduino.h"

class LoRa_P2P
{

  private:
    SoftwareSerial *RAKSerial;

    // Sets working mode for RAK811 to p2p
    void config_p2p()
    {
      char command[30] = "at+set_config=lora:work_mode:1";
      RAKSerial->write(command);
    }
  
  public:

    LoRa_P2P(int rxPin, int txPin)
    {
      this->RAKSerial = new SoftwareSerial(rxPin, txPin);
      RAKSerial->begin(9600);
      Serial.begin(9600);
      config_p2p();
    }
    

    // Sets frequency, spreadfactirm bandwidth, codingrate, preamble and power
    bool set_config(char frequency[], uint8_t SF, uint8_t BW, uint8_t CR, uint8_t PRlen, uint8_t PWR)
    {
      //char double_dots[1] = ":";
      //char command[] = {frequency, SF, BW, CR, PRlen, PWR};
      //char command[32] = {"at+set_config=lorap2p:" + String(frequency) + ":" + String(SF) + ":" + String(BW) + ":" + String(CR) + ":" + String(PRlen) + ":" + String(PWR)};
    
      RAKSerial->write("at+set_config=lorap2p:869525000:12:0:1:8:20");
     
    }

    bool set_transfer_mode(bool is_receiver)
    {
      char command[12] = "at+set_config=lorap2p: transfer_mode:";

      if(is_receiver)
      {
        for(int i = 0; i < sizeof(command); i++)
        {
          if(command[i] == NULL)
          {
            command[i] = "1";
            break;
          }
        }
      }
      else
      {
        for(int i = 0; i < sizeof(command); i++)
        {
          if(command[i] == NULL)
          {
            command[i] = "2";
            break;
          }
        }
      }

      Serial.print("Command: ");
      Serial.println(command);
      RAKSerial->write(command);
    }

    bool send_data(String data)
    {
      char command[12] = "at+send=lorap2p:";
      strcpy(command, data.c_str());
      RAKSerial->write(command);
    }

    void start_recv()
    {
      RAKSerial->listen();

      while(RAKSerial->available() > 0)
      {
        char inByte = RAKSerial->read();
        Serial.write(inByte);
      }      
    } 

};

