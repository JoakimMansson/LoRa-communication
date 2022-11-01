#include <SoftwareSerial.h>

class LoRa_P2P
{

  private:
    SoftwareSerial RAKSerial;

  
  public:

    LoRa(int rxPin, int txPin) : RAKSerial(rxPin, txPin)
    {}
  
    // Sets working mode for RAK811 to p2p
    void config_p2p()
    {
      String command = "at+set_config=lora:work_mode:1";
      RAKSerial.println(command);
    }

    // Sets frequency, spreadfactirm bandwidth, codingrate, preamble and power
    bool set_config(String frequency, int SF, int BW, int CR, int PRlen, int PWR)
    {
      String command = "at+set_config=lorap2p:" + frequency + ":" + SF + ":" + BW + ":" + CR + ":" + PRlen + ":" + PWR;
      String ret = RAKSerial.println(command);
      cout << ret;
      if(ret == "OK")
      {
        return True;
      }
      else
      {
        return False;
      }
    }

    bool set_transfer_mode(bool is_receiver))
    {
      String command = "at+set_config=lorap2p: transfer_mode:";

      if(is_receiver)
      {
        command += 1;
      }
      else
      {
        command += 2;
      }
      
      String ret = RAKSerial.println(command);
      cout << ret;
    }

    bool send_data(String data)
    {
      String command = "at+send=lorap2p:" + data

    }

};

int main()
{
  LoRa test(10, 9);
  return 0;
}