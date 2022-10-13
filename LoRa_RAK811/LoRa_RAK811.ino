#include <SoftwareSerial.h>

class LoRa
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

    bool set_config(String frequency, int SF, int BW, int CR, int PRlen, int PWR)
    {
      String command = "at+set_config=lorap2p:" + frequency + ":" + SF + ":" + BW + ":" + CR + ":" + PRlen + ":" + PWR;
      String ret = RAKSerial.println(command);
      cout << ret;
      
    }

};

int main()
{
  LoRa test(10, 9);
  return 0;
}