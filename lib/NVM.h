/**
 * NVM.h
 * A student developed API that write to the C3's NVM for a specific case
 * The case is the structs "button" and "profile" defined below
 * They are metadata for a larger website project. 
*/
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "Arduino.h"

struct button
{
  char name[100];
  char protocol[100];
  uint64_t data[5];
};

struct profile
{
  int number;
  char name[100];
  char protocol[100];
  button buttons[3];
};

uint64_t flash_read_u64(const char *key)
{
  // Open
  /* Serial.printf("\n");
  Serial.printf("Attempting to READ: %s\n", key);
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... "); */
  nvs_handle_t my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  }
  else
  {
    // Serial.printf("Done\n");

    // Read
    uint64_t value;
    // Serial.printf("Reading from NVS ... ");
    err = nvs_get_u64(my_handle, key, &value);
    switch (err)
    {
    case ESP_OK:
      // Serial.printf("Done\n");
      // Serial.printf("Reported value = %" PRIu32 "\n", value);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      Serial.printf("The value is not initialized yet!\n");
      break;
    default:
      Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);

    // Return
    return (value);
  }
  return (0);
}

void flash_read_str(const char *key, char * value)
{
  // Open
  /* Serial.printf("\n");
  Serial.printf("Attempting to READ: %s\n", key);
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... "); */
  nvs_handle_t my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  }
  else
  {
    // Serial.printf("Done\n");

    // Read
    size_t size = 0;
    // Serial.printf("Reading from NVS ... ");
    err = nvs_get_str(my_handle, key, NULL, &size);
    char read[size]; 
    switch (err)
    {
    case ESP_OK:
      nvs_get_str(my_handle, key, read, &size);
      strcpy(value, read); 
      // Serial.printf("Done\n");
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      Serial.printf("The value is not initialized yet!\n");
      break;
    default:
      Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
  }
}

void flash_write_u64(const char *key, uint64_t value)
{
  // Open
  /* Serial.printf("\n");
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... "); */
  nvs_handle_t my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  }
  else
  {
    // Serial.printf("Done\n");

    // Write
    // Serial.printf("Writing UINT32_T to: %s\nWith value: %" PRIu32 "\n", key, value);
    err = nvs_set_u64(my_handle, key, value);
    if(err != ESP_OK) {
      Serial.printf("Failed!\n");
    }

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    //Serial.printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    if(err != ESP_OK) {
      Serial.printf("Failed!\n");
    }

    // Close
    nvs_close(my_handle);
  }
}

void flash_write_str(const char *key, char *value)
{
  // Open
  /* Serial.printf("\n");
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... "); */ 
  nvs_handle_t my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  }
  else
  {
    //Serial.printf("Done\n");

    // Write
    //Serial.printf("Writing STR to: %s\nWith value: %s\n", key, my_value); 
    err = nvs_set_str(my_handle, key, value);
    if(err != ESP_OK) {
      Serial.printf("Failed!\n");
    }

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    // Serial.printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    if(err != ESP_OK) {
      Serial.printf("Failed!\n");
    }

    // Close
    nvs_close(my_handle);
  }
}

button flash_read_button(int p_num, int b_num)
{
  button b;
  switch (p_num)
  {
  case 1:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_read_str("p1_b1_name", b.name);
      flash_read_str("p1_b1_proto", b.protocol);
      b.data[0] = flash_read_u64("p1_b1_d1");
      b.data[1] = flash_read_u64("p1_b1_d2");
      b.data[2] = flash_read_u64("p1_b1_d3");
      b.data[3] = flash_read_u64("p1_b1_d4");
      b.data[4] = flash_read_u64("p1_b1_d5");
      break;
    }
    case 2:
    {
      flash_read_str("p1_b2_name", b.name);
      flash_read_str("p1_b2_proto", b.protocol);
      b.data[0] = flash_read_u64("p1_b2_d1");
      b.data[1] = flash_read_u64("p1_b2_d2");
      b.data[2] = flash_read_u64("p1_b2_d3");
      b.data[3] = flash_read_u64("p1_b2_d4");
      b.data[4] = flash_read_u64("p1_b2_d5");
      break;
    }
    case 3:
    {
      flash_read_str("p1_b3_name", b.name);
      flash_read_str("p1_b3_proto", b.protocol);
      b.data[0] = flash_read_u64("p1_b3_d1");
      b.data[1] = flash_read_u64("p1_b3_d2");
      b.data[2] = flash_read_u64("p1_b3_d3");
      b.data[3] = flash_read_u64("p1_b3_d4");
      b.data[4] = flash_read_u64("p1_b3_d5");
      break;
    }
    }
    break;
  }
  case 2:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_read_str("p2_b1_name", b.name);
      flash_read_str("p2_b1_proto", b.protocol);
      b.data[0] = flash_read_u64("p2_b1_d1");
      b.data[1] = flash_read_u64("p2_b1_d2");
      b.data[2] = flash_read_u64("p2_b1_d3");
      b.data[3] = flash_read_u64("p2_b1_d4");
      b.data[4] = flash_read_u64("p2_b1_d5");
      break;
    }
    case 2:
    {
      flash_read_str("p2_b2_name", b.name);
      flash_read_str("p2_b2_proto", b.protocol);
      b.data[0] = flash_read_u64("p2_b2_d1");
      b.data[1] = flash_read_u64("p2_b2_d2");
      b.data[2] = flash_read_u64("p2_b2_d3");
      b.data[3] = flash_read_u64("p2_b2_d4");
      b.data[4] = flash_read_u64("p2_b2_d5");
      break;
    }
    case 3:
    {
      flash_read_str("p2_b3_name", b.name);
      flash_read_str("p2_b3_proto", b.protocol);
      b.data[0] = flash_read_u64("p2_b3_d1");
      b.data[1] = flash_read_u64("p2_b3_d2");
      b.data[2] = flash_read_u64("p2_b3_d3");
      b.data[3] = flash_read_u64("p2_b3_d4");
      b.data[4] = flash_read_u64("p2_b3_d5");
      break;
    }
    }
    break;
  }
  case 3:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_read_str("p3_b1_name", b.name);
      flash_read_str("p3_b1_proto", b.protocol);
      b.data[0] = flash_read_u64("p3_b1_d1");
      b.data[1] = flash_read_u64("p3_b1_d2");
      b.data[2] = flash_read_u64("p3_b1_d3");
      b.data[3] = flash_read_u64("p3_b1_d4");
      b.data[4] = flash_read_u64("p3_b1_d5");
      break;
    }
    case 2:
    {
      flash_read_str("p3_b2_name", b.name);
      flash_read_str("p3_b2_proto", b.protocol);
      b.data[0] = flash_read_u64("p3_b2_d1");
      b.data[1] = flash_read_u64("p3_b2_d2");
      b.data[2] = flash_read_u64("p3_b2_d3");
      b.data[3] = flash_read_u64("p3_b2_d4");
      b.data[4] = flash_read_u64("p3_b2_d5");
      break;
    }
    case 3:
    {
      flash_read_str("p3_b3_name", b.name);
      flash_read_str("p3_b3_proto", b.protocol);
      b.data[0] = flash_read_u64("p3_b3_d1");
      b.data[1] = flash_read_u64("p3_b3_d2");
      b.data[2] = flash_read_u64("p3_b3_d3");
      b.data[3] = flash_read_u64("p3_b3_d4");
      b.data[4] = flash_read_u64("p3_b3_d5");
      break;
    }
    }
    break;
  }
  }
  return (b);
}

void flash_write_button(int p_num, int b_num, button b)
{

  switch (p_num)
  {
  case 1:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_write_str("p1_b1_name", b.name);
      flash_write_str("p1_b1_proto", b.protocol);
      flash_write_u64("p1_b1_d1", b.data[0]);
      flash_write_u64("p1_b1_d2", b.data[1]);
      flash_write_u64("p1_b1_d3", b.data[2]);
      flash_write_u64("p1_b1_d4", b.data[3]);
      flash_write_u64("p1_b1_d5", b.data[4]);
      break;
    }
    case 2:
    {
      flash_write_str("p1_b2_name", b.name);
      flash_write_str("p1_b2_proto", b.protocol);
      flash_write_u64("p1_b2_d1", b.data[0]);
      flash_write_u64("p1_b2_d2", b.data[1]);
      flash_write_u64("p1_b2_d3", b.data[2]);
      flash_write_u64("p1_b2_d4", b.data[3]);
      flash_write_u64("p1_b2_d5", b.data[4]);
      break;
    }
    case 3:
    {
      flash_write_str("p1_b3_name", b.name);
      flash_write_str("p1_b3_proto", b.protocol);
      flash_write_u64("p1_b3_d1", b.data[0]);
      flash_write_u64("p1_b3_d2", b.data[1]);
      flash_write_u64("p1_b3_d3", b.data[2]);
      flash_write_u64("p1_b3_d4", b.data[3]);
      flash_write_u64("p1_b3_d5", b.data[4]);
      break;
    }
    }
    break;
  }
  case 2:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_write_str("p2_b1_name", b.name);
      flash_write_str("p2_b1_proto", b.protocol);
      flash_write_u64("p2_b1_d1", b.data[0]);
      flash_write_u64("p2_b1_d2", b.data[1]);
      flash_write_u64("p2_b1_d3", b.data[2]);
      flash_write_u64("p2_b1_d4", b.data[3]);
      flash_write_u64("p2_b1_d5", b.data[4]);
      break;
    }
    case 2:
    {
      flash_write_str("p2_b2_name", b.name);
      flash_write_str("p2_b2_proto", b.protocol);
      flash_write_u64("p2_b2_d1", b.data[0]);
      flash_write_u64("p2_b2_d2", b.data[1]);
      flash_write_u64("p2_b2_d3", b.data[2]);
      flash_write_u64("p2_b2_d4", b.data[3]);
      flash_write_u64("p2_b2_d5", b.data[4]);
      break;
    }
    case 3:
    {
      flash_write_str("p2_b3_name", b.name);
      flash_write_str("p2_b3_proto", b.protocol);
      flash_write_u64("p2_b3_d1", b.data[0]);
      flash_write_u64("p2_b3_d2", b.data[1]);
      flash_write_u64("p2_b3_d3", b.data[2]);
      flash_write_u64("p2_b3_d4", b.data[3]);
      flash_write_u64("p2_b3_d5", b.data[4]);
      break;
    }
    }
    break;
  }
  case 3:
  {
    switch (b_num)
    {
    case 1:
    {
      flash_write_str("p3_b1_name", b.name);
      flash_write_str("p3_b1_proto", b.protocol);
      flash_write_u64("p3_b1_d1", b.data[0]);
      flash_write_u64("p3_b1_d2", b.data[1]);
      flash_write_u64("p3_b1_d3", b.data[2]);
      flash_write_u64("p3_b1_d4", b.data[3]);
      flash_write_u64("p3_b1_d5", b.data[4]);
      break;
    }
    case 2:
    {
      flash_write_str("p3_b2_name", b.name);
      flash_write_str("p3_b2_proto", b.protocol);
      flash_write_u64("p3_b2_d1", b.data[0]);
      flash_write_u64("p3_b2_d2", b.data[1]);
      flash_write_u64("p3_b2_d3", b.data[2]);
      flash_write_u64("p3_b2_d4", b.data[3]);
      flash_write_u64("p3_b2_d5", b.data[4]);
      break;
    }
    case 3:
    {
      flash_write_str("p3_b3_name", b.name);
      flash_write_str("p3_b3_proto", b.protocol);
      flash_write_u64("p3_b3_d1", b.data[0]);
      flash_write_u64("p3_b3_d2", b.data[1]);
      flash_write_u64("p3_b3_d3", b.data[2]);
      flash_write_u64("p3_b3_d4", b.data[3]);
      flash_write_u64("p3_b3_d5", b.data[4]);
      break;
    }
    }
    break;
  }
  }
}

profile flash_read_profile(int p_num)
{
  profile p; 
  p.number = p_num; 
  switch(p_num)
  {
    case 1:
    {
      flash_read_str("p1_name", p.name); 
      flash_read_str("p1_protocol", p.protocol);
      p.buttons[0] = flash_read_button(1, 1); 
      p.buttons[1] = flash_read_button(1, 2); 
      p.buttons[2] = flash_read_button(1, 3); 
      break; 
    }
    case 2:
    {
      flash_read_str("p2_name", p.name); 
      flash_read_str("p2_protocol", p.protocol);
      p.buttons[0] = flash_read_button(2, 1); 
      p.buttons[1] = flash_read_button(2, 2); 
      p.buttons[2] = flash_read_button(2, 3); 
      break; 
    }
    case 3:
    {
      flash_read_str("p3_name", p.name); 
      flash_read_str("p3_protocol", p.protocol);
      p.buttons[0] = flash_read_button(3, 1); 
      p.buttons[1] = flash_read_button(3, 2); 
      p.buttons[2] = flash_read_button(3, 3); 
      break; 
    }
  }
  return(p); 
}

void flash_write_profile(profile p)
{
  switch (p.number)
  {
  case 1:
  {
    flash_write_str("p1_name", p.name);
    flash_write_str("p1_protocol", p.protocol); 
    flash_write_button(1, 1, p.buttons[0]);
    flash_write_button(1, 2, p.buttons[1]);
    flash_write_button(1, 3, p.buttons[2]);
    break; 
  }
  case 2:
  {
    flash_write_str("p2_name", p.name);
    flash_write_str("p2_protocol", p.protocol);
    flash_write_button(2, 1, p.buttons[0]);
    flash_write_button(2, 2, p.buttons[1]);
    flash_write_button(2, 3, p.buttons[2]);
    break; 
  }
  case 3:
  {
    flash_write_str("p3_name", p.name);
    flash_write_str("p3_protocol", p.protocol);
    flash_write_button(3, 1, p.buttons[0]);
    flash_write_button(3, 2, p.buttons[1]);
    flash_write_button(3, 3, p.buttons[2]);
    break; 
  }
  }
}

void button_reset(int p_num, int b_num)
{
    button b_blank; 
    strcpy(b_blank.name, ""); 
    strcpy(b_blank.protocol, ""); 
    for(int i = 0; i < 5; i++)
    {
        b_blank.data[i] = 0; 
    }
    flash_write_button(p_num, b_num, b_blank);
}

void profile_reset(int p_num)
{ 

    profile p; 
    p.number = p_num; 
    strcpy(p.name, "___"); 
    strcpy(p.protocol, "___"); 
    button b_blank; 
    strcpy(b_blank.name, "___"); 
    strcpy(b_blank.protocol, "___"); 
    for(int i = 0; i < 5; i++)
    {
        b_blank.data[i] = 0; 
    }
    for(int i = 0; i < 3; i++)
    {
        p.buttons[i] = b_blank; 
    }
    flash_write_profile(p);
}

void flash_reset()
{
    profile_reset(1); 
    profile_reset(2); 
    profile_reset(3); 
}

void print_button(button b)
{
  Serial.printf("\tBUTTON PRINT: \n");
  Serial.printf("\t\tNAME: %s\n\t\tPROTOCOL: %s\n\t\tDATA(index): \n", b.name, b.protocol);
  for (int i = 0; i < 5; i++)
  {
    Serial.printf("\t\t\t%d.) %" PRIx64 "\n", i, b.data[i]);
  }
}

void print_profile(profile p)
{
  Serial.printf("\nPROFILE PRINT\n\tNUMBER : %d\n\tNAME: %s\n\tDEFAULT PROTOCOL: %s\n\tBUTTONS: \n", p.number, p.name, p.protocol);
  for (int i = 0; i < 3; i++)
  {
    Serial.printf("\n\t%d.)\n", i);
    print_button(p.buttons[i]);
  }
}

void generate_fake_profiles(void)
{
  flash_reset(); 
  profile p1; 
  profile p2; 
  profile p3; 

  p1.number = 1; 
  p2.number = 2; 
  p3.number = 3; 

  strcpy(p1.name, "Mom"); 
  strcpy(p2.name, "Dad"); 
  strcpy(p3.name, "Kidz"); 

  strcpy(p1.protocol, "SAMSUNG"); 
  strcpy(p2.protocol, "SAMSUNG"); 
  strcpy(p3.protocol, "SAMSUNG"); 

  button b[3]; 
  strcpy(b[0].name, "NETFLIX"); 
  strcpy(b[0].protocol, "SAMSUNG"); 
  for(int i = 0; i < 5; i++)
  {
    b[0].data[i] = 0; 
  }

  strcpy(b[1].name, "HULU"); 
  strcpy(b[1].protocol, "SAMSUNG"); 
  for(int i = 0; i < 5; i++)
  {
    b[1].data[i] = 0; 
  }

  strcpy(b[2].name, "BLANK"); 
  strcpy(b[2].protocol, "SAMSUNG"); 
  for(int i = 0; i < 5; i++)
  {
    b[2].data[i] = 0; 
  }

  for(int i = 0; i < 3; i++)
  {
    p1.buttons[i] = b[i];
    p2.buttons[i] = b[i]; 
    p3.buttons[i] = b[i];
  }

  flash_write_profile(p1);
  flash_write_profile(p2);
  flash_write_profile(p3);
}