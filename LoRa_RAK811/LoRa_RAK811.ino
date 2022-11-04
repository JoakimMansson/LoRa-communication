#include <SoftwareSerial.h>
#include "string.h"

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
      config_p2p();

    }
    

    // Sets frequency, spreadfactirm bandwidth, codingrate, preamble and power
    bool set_config(char frequency[], int SF, int BW, int CR, int PRlen, int PWR)
    {
      char double_dots[1] = ":";
      char command[23] = "at+set_config=lorap2p:" + frequency + ":" + char(SF) + ":" + char(BW) + ":" + char(CR) + ":" + char(PRlen) + ":" + char(PWR);
      String ret = RAKSerial->write(command);
      if(ret == "OK")
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    bool set_transfer_mode(bool is_receiver)
    {
      char command[12] = "at+set_config=lorap2p: transfer_mode:";

      if(is_receiver)
      {
        command += 1;
      }
      else
      {
        command += 2;
      }
      
      String ret = RAKSerial->write(command);
    }

    bool send_data(String data)
    {
      char command[12] = "at+send=lorap2p:" + data;
      String ret = RAKSerial.write(command);

      if(ret == "OK")
      {
        return true;
      }
      else
      {
        return false;
      }

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

int main()
{
  LoRa test(10, 9);
  return 0;
}