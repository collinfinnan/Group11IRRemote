/**
 * IRfunctions.h
 * IR Remote Control Group 11
 * Senior Design 2023, University of Notre Dame EE
 * 03/07/23
 *
 * Generates memory sensitive functions for IR signal sending and receiving. 
 */
#include <Arduino.h>
#include <IRremote.h>
#include "../lib/NVM.h"
#include "settings.h"

/* Function Prototypes */
void receiver_decode(char *protocol);
bool battery_read();
button button_record(const char *name);
void button_send_custom(button b);
void button_send_default(char * protocol, int b);

/* Declare Receiver and Sender */
IRrecv irrecv(PINRECV);
decode_results results;
IRsend irsend(PINSEND);

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
bool battery_read()
{

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
  float VTRANS = 0.0;
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
  float power = voltage_off * voltage_off / (R2);

  /* Print results */
  Serial.printf("ON VOLTAGE: %f\nPERCENT: %f\nOFF POWER DISAPPATION: %f\n\n", voltage, percent, power);
  
  if(percent > 0.8)
  {
    return(true); 
  }
  return(false); 
}

/**
 * void receiver_decode()
 * Params: none
 * Return: char *protocol - protocol will be a string describing the type of signal being sent
 * Usage: receiver_decode();
 * Description: this function uses already capture IR receiver data to decode and classify protocol
 */
void receiver_decode(char * protocol)
{
  switch (results.decode_type)
  {
  case NEC:
    strcpy(protocol, "NEC");
    break;
  case SONY:
    strcpy(protocol, "SONY");
    break;
  case RC5:
    strcpy(protocol, "RC5");
    break;
  case RC6:
    strcpy(protocol, "RC6");
    break;
  case DISH:
    strcpy(protocol, "DISH");
    break;
  case SHARP:
    strcpy(protocol, "SHARP");
    break;
  case JVC:
    strcpy(protocol, "JVC");
    break;
  case SANYO:
    strcpy(protocol, "SANYO");
    break;
  case MITSUBISHI:
    strcpy(protocol, "MITSUBISHI");
    break;
  case SAMSUNG:
    strcpy(protocol, "SAMSUNG");
    break;
  case LG:
    strcpy(protocol, "LG");
    break;
  case WHYNTER:
    strcpy(protocol, "WHYNTER");
    break;
  case AIWA_RC_T501:
    strcpy(protocol, "AIWA_RC_T501");
    break;
  case PANASONIC:
    strcpy(protocol, "PANASONIC");
    break;
  case DENON:
    strcpy(protocol, "DENON");
    break;
  default:
  case UNKNOWN:
    strcpy(protocol, "UNKNOWN");
    break;
  }
  return; 
}

/**
 * button button_record(const char *)
 * Params: 
 *    - const char * name: name of the button to be created. 
 * Return: 
 *    - button : the generated button that contains data, name, and protocol
 * Description: When you'd like to record a new button, call this function with the name of the
 *              new button! It will return a button struct costumized for your needs. 
*/
button button_record(const char *name)
{

  /* Initialize button */
  button b;
  strcpy(b.name, name); 
  strcpy(b.protocol, "SAMSUNG"); 
  for(int i = 0; i < 5; i++)
  {
    b.data[i] = 0; 
  }

  /* Initialize counter */
  int count = 0;
  int timeout = 0; 

  /* Do something within this while loop */
  while (count < 5 && timeout < 100)
  {
    Serial.printf("Strugling\n");
    delay(100); 
    timeout++; 
    /* Listen for IR Signal */
    /* if (irrecv.decode(&results))
    {
      b.data[count] = results.value;
      receiver_decode(b.protocol);
      Serial.printf("\nRECEIVED COUNT: %d\n\tDATA: %" PRIx64 "\n\tPROTOCOL: %s\n", count, b.data[count], b.protocol);
      count++;
      timeout = 0; 
      delay(500);
      irrecv.resume();
    } */
  }

  /* Return button */ 
  return(b); 
}

/**
 * void button_send(button b)
 * Params: 
 *    - button b: button whose data is to be actuated 
 * Return: 
 *    - nothing
 * Description: When you have a button that you want to send, just use this function! 
 *              Accounts for all the supported datatypes. 
 * Future Expansion: 
 *      -  Account for even more protocols
*/
void button_send_custom(button b)
{
  int nbits; 
  delay(100); 
  if(!strcmp(b.protocol, "SAMSUNG"))
  {
    int count = 0; 
    nbits = SAMSUNG_BITS; 
    while(count < 5 && b.data[count] != 0) 
    {
      irsend.sendSAMSUNG(b.data[count], nbits);
      Serial.printf("DATA SENT: %"PRIx64"\n", b.data[count]);
      count++; 
      delay(500); 
    }
  }
  else if(!strcmp(b.protocol, "NEC"))
  {
    int count = 0; 
    nbits = NEC_BITS; 
    while(count < 5 && b.data[count] != 0) 
    {
      irsend.sendNEC(b.data[count], nbits);
      count++; 
      delay(500); 
    }
  } 
  else
  {
    Serial.printf("Unknown protocol: %s! Error!\n", b.protocol); 
  }
}

/**
 *  button_send_default(button b)
 * Params: 
 *    - button b: button whose data is to be actuated 
 * Return: 
 *    - nothing
 * Description: When you have a button that you want to send, just use this function! 
 *              Accounts for all the supported datatypes. 
 * Future Expansion: 
 *      -  Account for even more protocols
*/
void button_send_default(char * protocol, int b)
{
  unsigned long data; 
  int nbits; 
  delay(100);
  switch(b)
  {
    case POWERON:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_PowerOn; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_PowerOn; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break; 
    }
    case POWEROFF:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_PowerOff; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_PowerOff; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case VOLUMEDOWN:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_VolumeDown; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_VolumeDown; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case VOLUMEUP:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_VolumeUp; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_VolumeUp; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case ZERO:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Zero; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Zero; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case ONE:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_One; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_One; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case TWO:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Two; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Two; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case THREE:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Three; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Three; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case FOUR:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Four; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Four; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case FIVE:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Five; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Five; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case SIX:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Six; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Six; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case SEVEN:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Seven; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Seven; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case EIGHT:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Eight; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Eight; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case NINE:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Nine; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Nine; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case ENTER:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Enter; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Enter; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case UP:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Up; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Up; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case DOWN:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Down; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Down; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case LEFT:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Left; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Left; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case RIGHT:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Right; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Right; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    case HOME:
    {
      if(!strcmp(protocol, "SAMSUNG"))
      {
        data = S_Home; 
        nbits = SAMSUNG_BITS; 
        irsend.sendSAMSUNG(data, nbits); 
      } 
      else if(!strcmp(protocol, "NEC"))
      {
        data = NEC_Home; 
        nbits = NEC_BITS; 
        irsend.sendNEC(data, nbits);
      }
      else
      {
        Serial.printf("Unsupported protocol, unable to send!!!\n");
      }
      break;
    }
    default:
    {
      Serial.printf("DEFAULT\n");
      break; 
    }
  }
}