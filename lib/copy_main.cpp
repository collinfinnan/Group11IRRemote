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
void build_html(void);

/* Define the servers (one for WiFi one for ESP32 as access point) */
AsyncWebServer server(80);

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
char html[30000];

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

const char html_style[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>

    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">


    <style>

      h1 {font-family: 'didot', serif; color: white; text-align: center; background-color: #455a64;}

      h2 {font-family: 'didot', serif; font-size: 1.5em; color: white; text-align: center; background-color: #455a64;}

      body {font-family: 'Lato', sans-serif; text-align: center; background-color: #455a64;}

        .wrapper{
            display: grid;
            grid-template-columns: 1fr 2fr 1fr;
            grid-auto-rows:minmax(100px, auto);
            justify-items:stretch;
            align-items: stretch;
        }

        .wrapper > div{
            background: #455a64;;
            padding: 1em;
        }

        .wrapper > div:nth-child(odd){
            background:  #455a64;;
        }

        .box1{
            grid-column: 1;
            grid-row: 1/7;
        }


        .box3{
            grid-column: 2/3;
            grid-row: 1/7;
        }

        .box4{
            grid-column: 3;
            grid-row: 1/4;
        }

        .box5{
            grid-column: 2/3;
            grid-row: 4/7;
        }

        .box7{
            grid-column: 3;
        }

        .box8{
            grid-column: 2/3;
            grid-row: 5/10;
        }

        .grid1 {
            border: 5px solid rgb(111,41,97);
            display: grid;
            grid-template-columns: 100px 100px 100px;
            column-gap: 5px;
            row-gap: 10px;
            width: 350px;
            height: 450px;
            margin-left: auto;
            margin-right: auto;
            align-content: center;
        }

        .grid1 > * {
            background-color: rgba(111,41,97,.4);
            width: 100px;
            height: 100px;
            margin: auto;
            margin-left: 12.5px;
            border-radius: 100%;
            padding: 30px;
        }
        .button1 { border-radius: 100%; outline: none; margin: 20px; box-shadow: 3px 3px 7px #333; background-color: #6699CC; border: none; color: white; padding: 20px 30px; font-size: 3em; font-weight: 700; letter-spacing: 0.1em; position: relative;
            text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;
        }
        .button2 { border-radius: 15%; height: 116px; width: 200px; outline: none; margin: 35px; box-shadow: 3px 3px 7px #333; background-color: #6699CC; border: none; color: white; padding: 20px; font-size: 2em; font-weight: 600; letter-spacing: 0.1em; position: relative;
            text-decoration: none; font-size: 30px; margin: 20px; cursor: pointer;}

        .button3 {border-radius: 5%; outline: none; margin-left: 15px; margin-right: 55px; box-shadow: 3px 3px 7px #333; height: 50px; width: 100px; background-color: #5072A7; border: none; color: white; position: relative; }

        .button4 {border-radius: 5%;  outline: none; box-shadow: 3px 3px 7px #333; height: 75px; width: 150px; background-color: #5072A7; border: none; color: white; position: relative; }

        .button5 {border-radius: 5%;  outline: none; box-shadow: 3px 3px 7px #333; height: 75px; width: 150px; background-color: rgb(86, 152, 188); border: none; color: white; position: relative;}

        .button6 { border-radius: 100%; outline: none; margin: 20px; box-shadow: 3px 3px 7px #333; background-color: #1fcd00; border: none; color: white; padding: 20px 30px; font-size: 3em; font-weight: 700; letter-spacing: 0.1em; position: relative;
            text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}

        .button7 { border-radius: 5%;  outline: none; box-shadow: 3px 3px 7px #333; height: 75px; width: 150px; background-color: #1fcd00; border: none; color: white; position: relative;}

        .button8 {border-radius: 5%;  outline: none; box-shadow: 3px 3px 7px #333; height: 75px; width: 150px; background-color: rgb(255, 0, 0); border: none; color: white; position: relative;}

        .button9 {border-radius: 5%;  outline: none; box-shadow: 3px 3px 7px #333; height: 75px; width: 150px; background-color: rgb(0, 255, 0); border: none; color: white; position: relative;}
      
        .directional-buttons {
          width: 16rem;
          height: 16rem;
          display: grid;
          grid-gap: 0.5rem;
          grid-template-columns: 45fr 60fr 45fr;
          grid-template-rows: 45fr 60fr 45fr;
          grid-template-areas: "....  up  ....."
                              "left  ..  right"
                              ".... down .....";    
        }
        .visually-hidden {
          position: absolute !important;
          height: 1px; 
          width: 1px;
          overflow: hidden;
          clip: rect(1px 1px 1px 1px);
          clip: rect(1px, 1px, 1px, 1px);
          white-space: nowrap;
        }
        .direction-button {
          color: rgb(86, 152, 188);
          background: currentcolor;
          border: none;
          position: relative;
        }
        .direction-button:before {
          display: block;
          content: "";
          position: absolute;
          width: 4.25rem;
          height: 4.25rem;
          transform: rotate(45deg);
          background: currentcolor;
        }
        .direction-button:after {
          display: block;
          content: "";
          position: absolute;
          border: 2rem solid transparent;
          color: rgba(255, 250, 250, 0.6);
        }
        .direction-button:hover {
          color: rgb(118, 137, 170);
        }
        .direction-button:active:after {
          color: rgb(255, 152, 152);
        }
        .up {
          grid-area: up;
          border-radius: 0.5rem 0.5rem 0 0;
        }
        .up:before {
          left: calc(50% - 2.125rem);
          bottom: -2.125rem;
        }
        .up:after {
          border-bottom-color: currentcolor;
          left: calc(50% - 2rem);
          top: -1rem;
        }
        .left {
          grid-area: left;
          border-radius: 0.5rem 0 0 0.5rem;
        }
        .left:before {
          right: -2.125rem;
          top: calc(50% - 2.125rem);
        }
        .left:after {
          border-right-color: currentcolor;
          top: calc(50% - 2rem);
          left: -1rem;
        }
        .right {
          grid-area: right;
          border-radius: 0 0.5rem 0.5rem 0;
        }
        .right:before {
          left: -2.125rem;
          top: calc(50% - 2.125rem);
        }
        .right:after {
          border-left-color: currentcolor;
          top: calc(50% - 2rem);
          right: -1rem;
        }
        .down {
          grid-area: down;
          border-radius: 0 0 0.5rem 0.5rem;
        }
        .down:before {
          left: calc(50% - 2.125rem);
          top: -2.125rem;
        }
        .down:after {
          border-top-color: currentcolor;
          left: calc(50% - 2rem);
          bottom: -1rem;
        }
)rawliteral";

const char html_header[] PROGMEM = R"rawliteral(
    </style>
</head>
<body>
    <P>
)rawliteral";

const char html_profile_header[] PROGMEM = R"rawliteral(
    <h1> ESP32 Web Server - TV Remote </h1></P>
    <div class= "wrapper">
        <div class= "box box1">
  )rawliteral";

const char html_profile_1[] PROGMEM = R"rawliteral(
            <p><a href="/PROFILE1"><button class=   )rawliteral";
const char html_profile_2[] PROGMEM = R"rawliteral(</button></a></p>
            <p><a href="/PROFILE2"><button class= )rawliteral";
const char html_profile_3[] PROGMEM = R"rawliteral(</button></a></p>
            <a href="/PROFILE3"><button class= )rawliteral";

const char html_defaults[] PROGMEM = R"rawliteral(
            <form action="/get">
                <p></p><p></p><p></p><p> <h2>Edit Profile Name: </h2></p><p></p>
                <input type="text" name="input3">
                <input type="submit" class = "button3" value="Submit">
              </form><br>
        </div>
        <div class= "box box3"> 
                <a href="/POWERON"><button class="button1">ON</button></a>
                <a href="/POWEROFF"><button class="button1">OFF</button></a>
                <a href="/VOLUMEUP"><button class="button1">VOL UP</button></a>
                <a href="/VOLUMEDOWN"><button class="button1">VOL DOWN</button></a>
                <p><a href="/ONE"><button class="button1">1</button></a>
                <a href="/TWO"><button class="button1">2</button></a>
                <a href="/THREE"><button class="button1">3</button></a></p>
                <p><a href="/FOUR"><button class="button1">4</button></a>
                <a href="/FIVE"><button class="button1">5</button></a>
                <a href="/SIX"><button class="button1">6</button></a></p>
                <p><a href="/SEVEN"><button class="button1">7</button></a>
                <a href="/EIGHT"><button class="button1">8</button></a>
                <a href="/NINE"><button class="button1">9</button></a></p>
                <p><a href="/ZERO"><button class="button1">0</button></a></p>
        </div>
        <div class= "box box4"> 
            <a href="/SAMSUNG"><button class= 
)rawliteral";

const char html_samsung[] PROGMEM = R"rawliteral(>SAMSUNG</button></a>
            <p><a href="/NEC"><button class= )rawliteral";


const char html_nav[] PROGMEM = R"rawliteral(>NEC</button></a></p>
  
        </div>
        <div class= "box box5">
        <div class="directional-buttons">
            <button class="direction-button up" onclick="location.href='/UP'">
              <span class="visually-hidden">up</span>
            </button>
            <button class="direction-button left" onclick="location.href='/LEFT'">
              <span class="visually-hidden">left</span>
            </button>
            <button class="direction-button right" onclick="location.href='/RIGHT'">
              <span class="visually-hidden">right</span>
            </button>
            <button class="direction-button down" onclick="location.href='/DOWN'">
              <span class="visually-hidden">down</span>
            </button>
          </div>
          <a href="/ENTER"><button class= "button5">Enter</button></a>
          <a href="/HOME"><button class= "button5">Home</button></a>)rawliteral";

const char html_battery_on[] PROGMEM = R"rawliteral(
          <button class="button9">Battery</button>
        </div>

)rawliteral";

const char html_battery_off[] PROGMEM = R"rawliteral(
          <button class="button8">Battery</button>
        </div>

)rawliteral";

const char html_custom_input[] PROGMEM = R"rawliteral(<div class= "box box7">

            <form action="/get">
                <h2> Custom App 1: </h2>
                <input type="text" name="input4">
                <input type="submit" class = "button3" value="Submit">
            </form><br>
        
              <form action="/get">
                <h2> Custom App 2: </h2>
                <input type="text" name="input5">
                <input type="submit" class = "button3" value="Submit">
            </form><br>
        
              <form action="/get">
                <h2> Custom App 3: </h2>
                <input type="text" name="input6">
                <input type="submit" class = "button3" value="Submit">
              </form><br>

        </div>

        <div class= "box box8"> 
            <a href="/CUSTOM1"><button class="button2">)rawliteral";
const char html_custom_1[] PROGMEM = R"rawliteral(</button></a>  
            <a href="/CUSTOM2"><button class="button2">)rawliteral";
const char html_custom_2[] PROGMEM = R"rawliteral(</button></a>
            <a href="/CUSTOM3"><button class="button2">)rawliteral";
const char html_custom_3[] PROGMEM = R"rawliteral(</button></a>
    
    </div>

</body>
  )rawliteral";


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
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  /* Begin collecting IR data from the pin */
  irrecv.enableIRIn();

  /* Set battery read pinmode */
  pinMode(PINBATW, OUTPUT);
  pinMode(PINBATR, INPUT);

  /* Reset flash, create some profiles, and load profile 1 by default */
  p = flash_read_profile(1); 

  // HOME PAGE
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    build_html(); 
    request->send_P(200, "text/html", html); });

  // TEXT BASED ENTRIES
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage;
    String inputParam;
    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    int count = 0; 
    /* For Custom Button 1 */
    if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
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
    else if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage = request->getParam(PARAM_INPUT_5)->value();
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
    else if (request->hasParam(PARAM_INPUT_6)) {
      inputMessage = request->getParam(PARAM_INPUT_6)->value();
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
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      strcpy(p.name, inputMessage.c_str());  
    }
    flash_write_profile(p);
    build_html(); 
    request->send_P(200, "text/html", html); });

  /* NUMBER BUTTONS */
  server.on("/ZERO", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    button_send_default(p.protocol, ZERO);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/ONE", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, ONE);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/TWO", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, TWO);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/THREE", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, THREE);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/FOUR", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, FOUR);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/FIVE", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, FIVE);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/SIX", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, SIX);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/SEVEN", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, SEVEN);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/EIGHT", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, EIGHT);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/NINE", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, NINE);
    build_html(); 
    request->send_P(200, "text/html", html); });

  /* POWER AND VOLUME BUTTONS */
  server.on("/VOLUMEUP", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    button_send_default(p.protocol, VOLUMEUP); 
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/VOLUMEDOWN", HTTP_GET, [](AsyncWebServerRequest *request)
            {  
    //irsend.sendSAMSUNG(0xE0E0D02F, 32);
    button_send_default(p.protocol, VOLUMEDOWN);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/POWERON", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, POWERON);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/POWEROFF", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, POWEROFF);
    build_html(); 
    request->send_P(200, "text/html", html); });

  /* CUSTOM BUTTONS */
  server.on("/CUSTOM1", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_custom(p.buttons[0]);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/CUSTOM2", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_custom(p.buttons[1]);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/CUSTOM3", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_custom(p.buttons[2]);
    build_html(); 
    request->send_P(200, "text/html", html); });

  /* PROFILE BUTTONS */
  server.on("/PROFILE1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    p = flash_read_profile(1); 
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/PROFILE2", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    p = flash_read_profile(2);
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/PROFILE3", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    p = flash_read_profile(3);
    build_html(); 
    request->send_P(200, "text/html", html); });

  /* PROTOCOL BUTTONS */
  server.on("/SAMSUNG", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    strcpy(p.protocol, "SAMSUNG"); 
    flash_write_profile(p); 
    build_html(); 
    request->send_P(200, "text/html", html); });
  server.on("/NEC", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    strcpy(p.protocol, "NEC"); 
    flash_write_profile(p); 
    build_html(); 
    request->send_P(200, "text/html", html); });

    /* NAV BUTTONS */
    server.on("/ENTER", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, ENTER);
    build_html(); 
    request->send_P(200, "text/html", html); });
    server.on("/UP", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, UP);
    build_html(); 
    request->send_P(200, "text/html", html); });
    server.on("/DOWN", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, DOWN);
    build_html(); 
    request->send_P(200, "text/html", html); });
    server.on("/LEFT", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, LEFT);
    build_html(); 
    request->send_P(200, "text/html", html); });
    server.on("/RIGHT", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, RIGHT);
    build_html(); 
    request->send_P(200, "text/html", html); });
    server.on("/HOME", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
    button_send_default(p.protocol, HOME);
    build_html(); 
    request->send_P(200, "text/html", html); });

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
            { request->send_P(200, "text/html", index_html); Serial.println("MADE IT HERE");});

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
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
    request->send_P(200, "text/html", index_html); });
  server.onNotFound(notFound);
  server.begin();
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

/**
 * FUNCTION:    void build_html(void)
 * PURPOSE:     Creates the HTML for remote control webpage
 * ARGUMENTS:   none
 * RETURNS:     none
 * DESCRIPTION: Uses the global "profile" to customize the appearance
 *              and functionality of the webpage.
 * BUTTONS:
 *              1.) On/Off/VolUp/VolDown/Numbers/Profiles Buttons Styling
 *              2.) Hot Key Buttons Styling
 *              3.) Submit and Record Buttons Styling
 *              4.) Samsung and NEC Buttons Styling
 *              5.) Enter Button Styling
 *              6.) Profile Button Styling (after Pressed)
 *              7.) NEC and Samsung Button Styling (after Pressed)
 *              8.) Battery (RED) Button Styling
 *              9.) Battery (Green) Button Styling
 **/
void build_html(void)
{

  /* Introductory HTML that defines styling*/
  /* See GRID.html file for comments on styling parameters */
  strcpy(html, html_style);

  /* Rest of the code is actually adding styled buttons */

  /* Title */
  strcat(html, html_header); 

  /* Profiles */
  profile p1 = flash_read_profile(1); 
  profile p2 = flash_read_profile(2); 
  profile p3 = flash_read_profile(3); 
  
  strcat(html, html_profile_header); 
  strcat(html, html_profile_1); 
  if(p.number == 1) {
    strcat(html, "\"button6\">");
  } else {
    strcat(html, "\"button1\">");
  }
  strcat(html, p1.name);
  strcat(html, html_profile_2);
  if(p.number == 2) {
    strcat(html, "\"button6\">");
  } else {
    strcat(html, "\"button1\">");
  }
  strcat(html, p2.name); 
  strcat(html, html_profile_2);
  if(p.number == 3) {
    strcat(html, "\"button6\">");
  } else {
    strcat(html, "\"button1\">");
  }
  strcat(html, p3.name);
  strcat(html, "</button></a>\n");

  strcat(html, html_defaults);

  /* Protocol Buttons */
  if(!strcmp(p.protocol, "SAMSUNG")) {
    strcat(html, "\"button7\"");
  } else {
    strcat(html, "\"button4\"");
  }
  strcat(html, html_samsung);
  if(!strcmp(p.protocol, "NEC")) {
    strcat(html, "\"button7\"");
  } else {
    strcat(html, "\"button4\"");
  }

  /* Naviation Buttons */
  strcat(html, html_nav);

  /* Battery Status */
  if(battery_read()) {
    strcat(html, html_battery_on);
  } else {
    strcat(html, html_battery_off);
  }

  /* Prompts for Custom Buttons */
  strcat(html, html_custom_input);
  strcat(html, p.buttons[0].name);
  strcat(html, html_custom_1);
  strcat(html, p.buttons[1].name); 
  strcat(html, html_custom_2);
  strcat(html, p.buttons[2].name);
  strcat(html, html_custom_3);

}
