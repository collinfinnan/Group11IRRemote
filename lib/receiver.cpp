#include <Arduino.h>
#include <IRremote.h>

const int receiver_pin = 5;
IRrecv irrecv(receiver_pin);
decode_results results;

void setup() {
    Serial.begin(115200);
    irrecv.enableIRIn();
}

void loop() {
    if (irrecv.decode(&results)) {
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