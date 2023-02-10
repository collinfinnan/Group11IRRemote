#include <IRremote.h>

const int RECV_PIN = 6;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Code: "); 
    Serial.println(results.value); 
    if(results.decode_type == SAMSUNG) {
      Serial.println("Samsung"); 
    } else {
      Serial.println("???");
    }
    irrecv.resume();
  }
  delay(1000);
}