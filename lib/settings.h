/**
 * IR Remote Control - Group 11
 * Notre Dame Senior Design EE - Spring 2023
 * settings.h
 * Objectives - 
 *    Holds data for operation like:
 *          - WiFi network and password
 *          - Pin assignments for trasmitter and receiver
 *          - Timeout settings
 *      This is a working list that will grow as the codebase grows
 * Date - 02/21/23
 **/


/**
 * Pin definitions for various hardware tasks controlled by ESP32 
 * PINRECV - IR receiver's output is attached here (digital)
 * PINSEND - Leads to gate of MOSFET that drives the IRED (digital)
 * PINBATW - Lead to gate of MOSFET that controls voltage divider (digital)
 * PINBATR - Reads voltage divider voltage (analog)
 **/
#define PINRECV 15
#define PINSEND 19
#define PINBATW 18 
#define PINBATR 26 