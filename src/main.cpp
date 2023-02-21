#include <Arduino.h>
#include <IRremote.h>
IRrecv irrecv(5);
IRsend irsend; 
decode_results results;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop() {

  uint32_t PowerOn = 0xE0E040BF;
  uint32_t PowerOff = 0xE0E019E6;
  uint32_t VolumeUp = 0xE0E0E01F;
  uint32_t VolumeDown = 0xE0E0D02F;
  uint32_t ChannelUp = 0xE0E048B7;
  uint32_t ChannelDown = 0xE0E008F7;
  uint32_t Mute = 0xE0E0F00F;
  uint32_t One = 0xE0E020DF;
  uint32_t Two = 0xE0E0A05F;   
  uint32_t Three = 0xE0E0609F; 
  uint32_t Four = 0xE0E010EF;
  uint32_t Five = 0xE0E0906F; 
  uint32_t Six = 0xE0E050AF;
  uint32_t Seven = 0xE0E030CF;
  uint32_t Eight = 0xE0E0B04F; 
  uint32_t Nine = 0xE0E0708F;
  uint32_t Zero = 0xE0E08877; 
  uint32_t Del = 0xE0E0C43B;
  uint32_t CHList = 0xE0E0D629;
  uint32_t Soure = 0xE0E0807F; 
  uint32_t Menu = 0xE0E058A7;
  uint32_t Home = 0xE0E09E61;  
  uint32_t MagicInfo = 0xE0E00CF3; 
  uint32_t Tools = 0xE0E0D22D; 
  uint32_t Info = 0xE0E0F807;
  uint32_t Up = 0xE0E006F9;
  uint32_t Left = 0xE0E0A659;
  uint32_t Down = 0xE0E08679; 
  uint32_t Right = 0xE0E046B9; 
  uint32_t Enter = 0xE0E016E9; 
  uint32_t Return = 0xE0E01AE5; 
  uint32_t Exit = 0xE0E0B44B;
  uint32_t Rewind = 0xE0E0A25B;
  uint32_t Play = 0xE0E0E21D; 
  uint32_t Pause = 0xE0E052AD; 
  uint32_t FastForward = 0xE0E012ED;
  uint32_t Stop = 0xE0E0629D; 




  
  
  uint8_t len = 32; 
  byte SEND_PIN = 26;
  IRsend irsend(SEND_PIN);
  irsend.sendSAMSUNG(PowerOn, len);
  // put your main code here, to run repeatedly:

  if (irrecv.decode(&results)){
    int value = results.value;
    Serial.println(" ");
    Serial.print("Code: "); 
    Serial.println(results.value);
    if(results.decode_type == SAMSUNG) {
      Serial.println("Samsung!");
    } else {
      Serial.println("???");
    }
    Serial.println(" ");
    irrecv.resume();
    Serial.println("******");
  } 

  delay(1000);
}