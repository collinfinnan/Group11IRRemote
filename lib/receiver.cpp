/**
 * IR Remote Control - Group 11
 * Notre Dame Senior Design EE - Spring 2023
 * Demo for Remote Proof of Concept
 * Objectives -
 *    Receive signals and print out code and type
 *    Control TV power and volume via website
 *    Organize code within Github
 * Date - 02/21/23
 **/

/* Include needed packages */
#include <Arduino.h>
#include <IRremote.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "../lib/IRfunctions.h"

void setup()
{
  Serial.begin(115200); 
  delay(1000); 
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.printf("\nVALUE: ");
    Serial.println(results.value); 
    delay(500); 
    irrecv.resume();
  }
}