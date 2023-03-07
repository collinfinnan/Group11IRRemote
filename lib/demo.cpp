/**
 * demo.cpp
 * IR Remote Control Group 11
 * Senior Design 2023, University of Notre Dame EE
 * 03/07/23
 * 
 * This code highlights the hardware functionality of different subsystems. 
*/
#include <Arduino.h>
#include <IRremote.h>
#include "../lib/SamsungCodes.h"
#include "../lib/settings.h"

/* Demo Type */
#define RECV 1
#define BAT 2
#define SEND 3
#define DEMOTYPE RECV 

/* Function Prototypes */
void receiver_decode();
void battery_read(); 
void volumeSendDemo();

/* Declare Receiver Objects */
IRrecv irrecv(PINRECV);
decode_results results;

/* Declare Send Object */
IRsend irsend(PINSEND);
uint32_t command;
uint8_t len = 32; 

void setup()
{
    /* Begin the serial monitor */
    Serial.begin(115200);

    /* Set pinMode for battery reader / writer */
    pinMode(PINBATW, OUTPUT);
    digitalWrite(PINBATW, LOW); 
    pinMode(PINBATR, INPUT); 

    /* Begin collecting IR data from the pin */
    irrecv.enableIRIn();
}

void loop()
{
    if(DEMOTYPE == RECV) {
        receiver_decode(); 
    } else if (DEMOTYPE == SEND) {
        volumeSendDemo();
    } else if (DEMOTYPE == BAT) {
        battery_read(); 
    }
}

/**
 * float battery_read()
 * Params: none
 * Return: none
 * Usage: battery_read(); 
 * Description: this function uses pre-defined pin settings to find the output voltage of battery
 *                  - Calculates theorhetical needed read value
 *                  - Calculates actual value
 *                  - Calculates "off" mode power disappation 
*/
void battery_read() {

    /* ESP32 analog read constants */
    const float ESP32_pin_voltage = 3.3; 
    const int ESP32_ADC_scale = 4096; 

    /* Set gate voltage */
    digitalWrite(PINBATW, HIGH); 
    delay(10); 

    /* Read the voltage divider */
    int discrete_voltage = analogRead(PINBATR); 

    /* Calculate voltage */
    float voltage = (float)discrete_voltage / ESP32_ADC_scale * 3.3; 

    /* Calculate % - Note, for 0.3 mA, the drop across transistor will be basically nothing */
    float VTRANS = 0; 
    float R1 = 10000; 
    float R2 = 10000; 
    float voltage_calc = (ESP32_pin_voltage - VTRANS) * R2 / (R1 + R2); 
    float percent = voltage / voltage_calc; 

    /* Read off characteristics */
    digitalWrite(PINBATW, LOW); 
    delay(10); 
    discrete_voltage = analogRead(PINBATR); 

    /* Calculate voltage and off power */
    float voltage_off = (float)discrete_voltage / ESP32_ADC_scale * 3.3; 
    float power = voltage_off*voltage_off / (R2); 

    /* Print results */
    Serial.printf("ON VOLTAGE: %f\nPERCENT: %f\nOFF POWER DISAPPATION: %f\n\n",voltage, percent, power);
    delay(2000);  

}

/**
 * void receiver_decode()
 * Params: none
 * Return: none
 * Usage: receiver_decode(); 
 * Description: this function uses pre-defined pin settings to classify incoming IR signals
 *                  - Prints protocol
 *                  - Prints data value
 *                  - Eventual usage for adding buttons
*/
void receiver_decode() {
    if (irrecv.decode(&results))
    {
        Serial.print("PROTOCOL: ");
        switch (results.decode_type)
        {
        case NEC:
            Serial.println("NEC");
            break;
        case SONY:
            Serial.println("SONY");
            break;
        case RC5:
            Serial.println("RC5");
            break;
        case RC6:
            Serial.println("RC6");
            break;
        case DISH:
            Serial.println("DISH");
            break;
        case SHARP:
            Serial.println("SHARP");
            break;
        case JVC:
            Serial.println("JVC");
            break;
        case SANYO:
            Serial.println("SANYO");
            break;
        case MITSUBISHI:
            Serial.println("MITSUBISHI");
            break;
        case SAMSUNG:
            Serial.println("SAMSUNG");
            break;
        case LG:
            Serial.println("LG");
            break;
        case WHYNTER:
            Serial.println("WHYNTER");
            break;
        case AIWA_RC_T501:
            Serial.println("AIWA_RC_T501");
            break;
        case PANASONIC:
            Serial.println("PANASONIC");
            break;
        case DENON:
            Serial.println("DENON");
            break;
        default:
        case UNKNOWN:
            Serial.println("UNKNOWN");
            break;
        }
        Serial.printf("VALUE: %X", results.value);
        irrecv.resume();
    }
}

/**
 * void volumeSendDemo()
 * Params: none
 * Return: none
 * Usage: volumeSendDemo()
 * Description: this function send volume up - volume down to show IR trasmission 
*/
void volumeSendDemo() {
    command = VolumeUp; 
    irsend.sendSAMSUNG(command, len); 
    delay(1000);
    command = VolumeDown; 
    irsend.sendSAMSUNG(command, len); 
    delay(1000); 
}