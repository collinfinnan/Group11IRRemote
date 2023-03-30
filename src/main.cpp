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

/* Prototypes */
void host_server(void);
void notFound(AsyncWebServerRequest *request);

/* Define the servers (one for WiFi one for ESP32 as access point) */
AsyncWebServer server(80);
void build_html(void); 

/* WiFi credentials globals */
char ssid[100];
char password[100];

/* Params for AP site to enter wifi */
const char *PARAM_INPUT_1 = "input1";
const char *PARAM_INPUT_2 = "input2";
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_INPUT_4 = "input4";
const char *PARAM_INPUT_5 = "input5";
const char *PARAM_INPUT_6 = "input6";

/* Params to determine when AP can close */
bool finished_ssid = false; 
bool finished_password = false;

/* AP network and password defaults */
const char *ap_ssid = "ESP32";
const char *ap_password = "password";

/* HTML container global */
char html[10000]; 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Enter the WiFi SSID: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter the WiFi Password: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

/* HTTP header for storage and time keeping for timeouts */
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

/* Variables for recording functionality */
int count = 0;
int recording = 0;

/* Overall profile */
profile p;

void setup()
{
  /* Establish serial connection */
  Serial.begin(115200);
  delay(1000);

  /* Initialize memory */
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  flash_read_str("ssid", ssid);
  flash_read_str("password", password);

  /* Begin the WiFi server and connect to WiFi */
  WiFi.begin(ssid, password);
  int wifi_counter = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_counter < 20)
  {
    wifi_counter++;
    delay(500);
  }
  if (wifi_counter == 20)
  {
    host_server(); 
  }
  // Serial.println("WiFi connected.");
  // Serial.print("IP address: ");
  server.begin();

  /* Begin collecting IR data from the pin */
  irrecv.enableIRIn();

  /* Reset flash, create some profiles, and load profile 1 by default */
  flash_reset();
  generate_fake_profiles();
  p = flash_read_profile(1);

  // HOME PAGE
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  // TEXT BASED ENTRIES
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    int count = 0; 
    /* For Custom Button 1 */
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      strcpy(p.buttons[0].name, inputMessage.c_str());
      currentTime = millis(); 
      previousTime = currentTime;  
      while(count < 5 && (currentTime - previousTime) < 2000)
      {
        currentTime = millis(); 
        if (irrecv.decode(&results))
        {
          p.buttons[0].data[count] = results.value; 
          receiver_decode(p.buttons[0].protocol); 
          Serial.printf("CURRENT COUNT: %d\n", count); 
          count++; 
          delay(500); 
          irrecv.resume();
          previousTime = currentTime; 
        }
      }
    }
    /* For Custom Button 2 */
    else if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      strcpy(p.buttons[1].name, inputMessage.c_str());
      currentTime = millis(); 
      previousTime = currentTime;  
      while(count < 5 && (currentTime - previousTime) < 2000)
      {
        currentTime = millis(); 
        if (irrecv.decode(&results))
        {
          p.buttons[1].data[count] = results.value; 
          receiver_decode(p.buttons[1].protocol); 
          Serial.printf("CURRENT COUNT: %d\n", count); 
          count++; 
          delay(500); 
          irrecv.resume();
          previousTime = currentTime; 
        }
      }
    }
    /* For Custom Button 3 */
    else if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage = request->getParam(PARAM_INPUT_5)->value();
      strcpy(p.buttons[2].name, inputMessage.c_str());
      currentTime = millis(); 
      previousTime = currentTime;  
      while(count < 5 && (currentTime - previousTime) < 2000)
      {
        currentTime = millis(); 
        if (irrecv.decode(&results))
        {
          p.buttons[2].data[count] = results.value; 
          receiver_decode(p.buttons[0].protocol); 
          Serial.printf("CURRENT COUNT: %d\n", count); 
          count++; 
          delay(500); 
          irrecv.resume();
          previousTime = currentTime; 
        }
      }
    }
    /* For the Profile Value */
    else if (request->hasParam(PARAM_INPUT_6)) {
      inputMessage = request->getParam(PARAM_INPUT_6)->value();
      strcpy(p.name, inputMessage.c_str());  
    }
    flash_write_profile(p);
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* NUMBER BUTTONS */
  server.on("/ZERO", HTTP_GET, [](AsyncWebServerRequest *request){
    button_send_default(p.protocol, ZERO);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/ONE", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, ONE);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/TWO", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, TWO);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/THREE", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, THREE);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/FOUR", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, FOUR);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/FIVE", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, FIVE);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/SIX", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, SIX);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/SEVEN", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, SEVEN);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/EIGHT", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, EIGHT);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/NINE", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, NINE);
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* POWER AND VOLUME BUTTONS */
  server.on("/VOLUMEUP", HTTP_GET, [](AsyncWebServerRequest *request){
    button_send_default(p.protocol, VOLUMEUP); 
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/VOLUMEDOWN", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, VOLUMEDOWN);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/POWERON", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, POWERON);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/POWEROFF", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_default(p.protocol, POWEROFF);
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* CUSTOM BUTTONS */
  server.on("/CUSTOM1", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_custom(p.buttons[0]);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/CUSTOM2", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_custom(p.buttons[1]);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/CUSTOM3", HTTP_GET, [](AsyncWebServerRequest *request){ 
    button_send_custom(p.buttons[2]);
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* PROFILE BUTTONS */
  server.on("/PROFILE1", HTTP_GET, [](AsyncWebServerRequest *request){
    p = flash_read_profile(1); 
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/PROFILE2", HTTP_GET, [](AsyncWebServerRequest *request){ 
    p = flash_read_profile(2);
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/PROFILE3", HTTP_GET, [](AsyncWebServerRequest *request){ 
    p = flash_read_profile(3);
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* PROTOCOL BUTTONS */
  server.on("/SAMSUNG", HTTP_GET, [](AsyncWebServerRequest *request){ 
    strcpy(p.protocol, "SAMSUNG"); 
    flash_write_profile(p); 
    build_html(); 
    request->send_P(200, "text/html", html);
  });
  server.on("/NEC", HTTP_GET, [](AsyncWebServerRequest *request){ 
    strcpy(p.protocol, "NEC"); 
    flash_write_profile(p); 
    build_html(); 
    request->send_P(200, "text/html", html);
  });

  /* Start server */
  server.onNotFound(notFound);
  server.begin();

}

void loop()
{
}

/**
 * THIS IS FOR THE AP NOT THE WIFI SERVER
 * PLEASE 
 * DONT
 * MESS 
 * IT 
 * UP
*/
void host_server(void)
{ 

  /* Create access point WiFi connection and print IP */
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  /* Create server and start it */
  server.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      strcpy(ssid, inputMessage.c_str()); 
      flash_write_str("ssid", ssid); 
      finished_ssid = true; 
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      strcpy(password, inputMessage.c_str()); 
      flash_write_str("password", password); 
      finished_password = true; 
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    if(finished_password && finished_ssid)
    {
      request->send(200, "text/html", "Finished Setting Up WiFi Please Restart ESP32");
      finished_ssid = false; 
      finished_password = false; 
    }
    request->send_P(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();

}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void build_html(void) 
{

  /* Introductory HTML */
  const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html><head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
              
  <style>html { font-family: 'didot', serif; display: center; margin: 0px auto; text-align: center;}
              
              body {font-family: 'Lato', sans-serif; text-align: center; background-color: #455a64;}
              h1 {font-family: 'didot', serif; color: white; text-align: center; background-color: #455a64;}
            
              .button { border-radius: 100%; outline: none; margin 20px; box-shadow: 3px 3px 7px #333; background-color: #6699CC; border: none; color: white; padding: 20px; font-size: 2em; font-weight: 600; letter-spacing: 0.1em; position: relative;
              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}

              .button2 { border-radius: 15%; height: 100px; width: 170px; outline: none; margin 35px; box-shadow: 3px 3px 7px #333; background-color: #6699CC; border: none; color: white; padding: 20px; font-size: 2em; font-weight: 600; letter-spacing: 0.1em; position: relative;
              text-decoration: none; font-size: 30px; margin: 20px; cursor: pointer;}

              .button8 { border-radius: 15%; height: 100px; width: 170px; outline: none; margin 35px; box-shadow: 3px 3px 7px #333; background-color: #33cc33; border: none; color: white; padding: 20px; font-size: 2em; font-weight: 600; letter-spacing: 0.1em; position: relative;
              text-decoration: none; font-size: 30px; margin: 20px; cursor: pointer;}
            
              .button3 {cursor: pointer; border-radius: 5%; outline: none; margin-left: 15px; box-shadow: 3px 3px 7px #333; height: 50px; width: 90px; background-color: #5072A7; border: none; color: white; font-size: 1.2em; font-weight: 500; letter-spacing: 0.05em; position: relative; left: 35%;}

              .button7 {cursor: pointer; border-radius: 5%; outline: none; margin-left: 15px; box-shadow: 3px 3px 7px #333; height: 50px; width: 90px; background-color: #33cc33; border: none; color: white; font-size: 1.2em; font-weight: 500; letter-spacing: 0.05em; position: relative; left: 35%;}

              .button4 {cursor: pointer; border-radius: 5%; outline: none; box-shadow: 3px 3px 7px #333; height: 50px; width: 100px; background-color: #5072A7; border: none; display: flex; color: white; position: absolute; margin-left: 40px;}

              .button5 {cursor: pointer; border-radius: 5%; outline: none; margin-left: 55px; margin-right: 55px; box-shadow: 3px 3px 7px #333; height: 50px; width: 100px; background-color: #5072A7; border: none; color: white; position: relative; }

              .button6 {cursor: pointer; border-radius: 5%; outline: none; box-shadow: 3px 3px 7px #333; height: 50px; width: 100px; background-color: #33cc33; border: none; display: flex; color: white; position: absolute; margin-left: 40px;}

  </style></head>

  <body><h1>ESP32 Web Server - TV Remote </h1>)rawliteral";
  strcpy(html, index_html);

  /* Profile HTML */
  strcat(html, "<a href=\"/PROFILE1\">");
  if(p.number == 1)
  {
    strcat(html, "<button class= \"button8\">");
  }
  else
  {
    strcat(html, "<button class= \"button2\">");
  }
  profile p1 = flash_read_profile(1); 
  strcat(html, p1.name); 
  strcat(html, "</button>");
  strcat(html, "<a href=\"/PROFILE2\">");
  if(p.number == 2)
  {
    strcat(html, "<button class= \"button8\">");
  }
  else
  {
    strcat(html, "<button class= \"button2\">");
  }
  profile p2 = flash_read_profile(2); 
  strcat(html, p2.name); 
  strcat(html, "</button>");
  strcat(html, "<a href=\"/PROFILE3\">");
  if(p.number == 3)
  {
    strcat(html, "<button class= \"button8\">");
  }
  else
  {
    strcat(html, "<button class= \"button2\">");
  }
  profile p3 = flash_read_profile(3); 
  strcat(html, p3.name); 
  strcat(html, "</button></a><p>");
  const char profile_html[] PROGMEM = R"rawliteral(
    <form action="/get">
      Change Profile Name: <input type="text" name="input6">
      <input type="submit" value="Submit">
    </form><br>
    </body></html>
  )rawliteral";
  strcat(html, profile_html); 

  /* SAMSUNG HTML */
  strcat(html, "<div class='container'>"); 
  if(!strcmp(p.protocol, "SAMSUNG"))
  {
    strcat(html, "<a href=\"/SAMSUNG\"><p><button class= \"button6\">SAMSUNG</button>");
  }
  else
  {
    strcat(html, "<a href=\"/SAMSUNG\"><p><button class= \"button4\">SAMSUNG</button>");
  }

  /* Power and Volume Buttons */
  const char power_and_volume_html[] PROGMEM = R"rawliteral(
      <a href="/POWERON"><button class="button">ON</button></a>
      <a href="/POWEROFF"><button class="button">OFF</button></a>
      <a href="/VOLUMEUP"><button class="button">VOL UP</button></a>
      <a href="/VOLUMEDOWN"><button class="button">VOL DOWN</button></a>
  )rawliteral";
  strcat(html, power_and_volume_html);

  /* NEC Protocol Button */
  if(!strcmp(p.protocol, "NEC"))
  {
    strcat(html, "<a href=\"/NEC\"><p><button class= \"button6\">NEC</button>");
  }
  else
  {
    strcat(html, "<a href=\"/NEC\"><p><button class= \"button4\">NEC</button>");
  }

  /* Default button Two */
  const char default_two_html[] PROGMEM = R"rawliteral(
              <p><a href="/ONE"><button class="button">1</button></a>
              <a href="/TWO"><button class="button">2</button></a>
              <a href="/THREE"><button class="button">3</button></a></p>
  )rawliteral";
  const char default_three_html[] PROGMEM = R"rawliteral(
              <p><a href="/FOUR"><button class="button">4</button></a>
              <a href="/FIVE"><button class="button">5</button></a>
              <a href="/SIX"><button class="button">6</button></a></p>
              <p><a href="/SEVEN"><button class="button">7</button></a>
              <a href="/EIGHT"><button class="button">8</button></a>
              <a href="/NINE"><button class="button">9</button></a></p>
              <p><a href="/ZERO"><button class="button">0</button></a></p>
    )rawliteral";
    strcat(html, default_two_html);
    if(battery_read())
    {
      strcat(html, "<button class=\"button6\">Battery</button></a>"); 
    }
    else
    {
      strcat(html, "<button class=\"button4\">Battery</button></a>"); 
    }
    strcat(html, default_three_html); 

    /* Prerecorded buttons */
    strcat(html, "<p><a href=\"/CUSTOM1\"><button class=\"button2\">");
    strcat(html, p.buttons[0].name); 
    strcat(html, "</button> ");
    strcat(html, "<a href=\"/CUSTOM2\"><button class=\"button2\">");
    strcat(html, p.buttons[1].name); 
    strcat(html, "</button> ");
    strcat(html, "<a href=\"/CUSTOM3\"><button class=\"button2\">");
    strcat(html, p.buttons[2].name); 
    strcat(html, "</button></a><p>");
    const char remaining_buttons_html[] PROGMEM = R"rawliteral(
        <form action="/get">
          Custom Button 1: <input type="text" name="input3">
          <input type="submit" value="Submit">
        </form><br>
        <form action="/get">
          Custom Button 2: <input type="text" name="input4">
          <input type="submit" value="Submit">
        </form><br>
        <form action="/get">
          Custom Button 3: <input type="text" name="input5">
          <input type="submit" value="Submit">
        </form>
    </body></html>)rawliteral";
  strcat(html, remaining_buttons_html);
}