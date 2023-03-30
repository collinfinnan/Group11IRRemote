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

/* Default bits sent for each protocol */
#define SAMSUNG_BITS 32
#define NEC_BITS 32

/* Enumeration for default buttons passed as arguments */
enum default_button
{
    POWERON,
    POWEROFF,
    VOLUMEUP,
    VOLUMEDOWN,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    ZERO
};

/* Samsung default codes */
#define S_PowerOn 0xE0E040BF;
#define S_PowerOff 0xE0E019E6;
#define S_VolumeUp 0xE0E0E01F;
#define S_VolumeDown 0xE0E0D02F;
#define S_ChannelUp 0xE0E048B7;
#define S_ChannelDown 0xE0E008F7;
#define S_Mute 0xE0E0F00F;
#define S_One 0xE0E020DF;
#define S_Two 0xE0E0A05F;   
#define S_Three 0xE0E0609F; 
#define S_Four 0xE0E010EF;
#define S_Five 0xE0E0906F; 
#define S_Six 0xE0E050AF;
#define S_Seven 0xE0E030CF;
#define S_Eight 0xE0E0B04F; 
#define S_Nine 0xE0E0708F;
#define S_Zero 0xE0E08877; 
#define S_Del 0xE0E0C43B;
#define S_CHList 0xE0E0D629;
#define S_Soure 0xE0E0807F; 
#define S_Menu 0xE0E058A7;
#define S_Home 0xE0E09E61;  
#define S_MagicInfo 0xE0E00CF3; 
#define S_Tools 0xE0E0D22D; 
#define S_Info 0xE0E0F807;
#define S_Up 0xE0E006F9;
#define S_Left 0xE0E0A659;
#define S_Down 0xE0E08679; 
#define S_S_Right 0xE0E046B9; 
#define S_Enter 0xE0E016E9; 
#define S_Return 0xE0E01AE5; 
#define S_Exit 0xE0E0B44B;
#define S_Rewind 0xE0E0A25B;
#define S_Play 0xE0E0E21D; 
#define S_Pause 0xE0E052AD; 
#define S_FastForward 0xE0E012ED;
#define S_Stop 0xE0E0629D;

/* NEC default codes */
#define NEC_PowerOn 0xE0E040BF0;
#define NEC_PowerOff 0xE0E019E60;
#define NEC_VolumeUp 0xE0E0E01F0;
#define NEC_VolumeDown 0xE0E0D02F0;
#define NEC_ChannelUp 0xE0E048B70;
#define NEC_ChannelDown 0xE0E008F70;
#define NEC_Mute 0xE0E0F00F0;
#define NEC_One 0xE0E020DF0;
#define NEC_Two 0xE0E0A05F0;   
#define NEC_Three 0xE0E0609F0; 
#define NEC_Four 0xE0E010EF0;
#define NEC_Five 0xE0E0906F0; 
#define NEC_Six 0xE0E050AF0;
#define NEC_Seven 0xE0E030CF0;
#define NEC_Eight 0xE0E0B04F0; 
#define NEC_Nine 0xE0E0708F0;
#define NEC_Zero 0xE0E088770; 
#define NEC_Del 0xE0E0C43B0;
#define NEC_CHList 0xE0E0D6290;
#define NEC_Soure 0xE0E0807F0; 
#define NEC_Menu 0xE0E058A70;
#define NEC_Home 0xE0E09E610;  
#define NEC_MagicInfo 0xE0E00CF30; 
#define NEC_Tools 0xE0E0D22D0; 
#define NEC_Info 0xE0E0F8070;
#define NEC_Up 0xE0E006F90;
#define NEC_Left 0xE0E0A6590;
#define NEC_Down 0xE0E086790; 
#define NEC_S_Right 0xE0E046B90; 
#define NEC_Enter 0xE0E016E90; 
#define NEC_Return 0xE0E01AE50; 
#define NEC_Exit 0xE0E0B44B0;
#define NEC_Rewind 0xE0E0A25B0;
#define NEC_Play 0xE0E0E21D0; 
#define NEC_Pause 0xE0E052AD0; 
#define NEC_FastForward 0xE0E012ED0;
#define NEC_Stop 0xE0E0629D0;