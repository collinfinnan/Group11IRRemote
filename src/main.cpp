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
#include <../lib/SamsungCodes.h>
#include <Wifi.h>

/* Wifi Credentials */
const char* ssid = "SDNet";
const char* password = "CapstoneProject";

/* Define the server */
WiFiServer server(80);

/* HTTP header for storage and time keeping for timeouts */
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

/* Pin Definitions for ESP32 */
const int transmitter_pin = 26;
const int receiver_pin = 5;

/* Create the send and receive Objects */
IRsend irsend(transmitter_pin);
IRrecv irrecv(receiver_pin);
decode_results results;
uint32_t command;
uint8_t len = 32; 

/* Output Button Status */
String output4State = "off";
String output26State = "off";
String output27State = "off";
const int output4 = 4;
const int output26 = 26;
const int output27 = 27;

void setup() {

  /* Establish serial connection */
  Serial.begin(115200);

  /* Begin the WiFi server and connect to WiFi */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting ...");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  irrecv.enableIRIn();
}

void loop() {
WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /POWERON") >= 0) {
              command = PowerOn; 
              irsend.sendSAMSUNG(command, len);
            } 

            if (header.indexOf("GET /POWEROFF") >= 0) {
              command = PowerOff; 
              irsend.sendSAMSUNG(command, len);
            } 

            if (header.indexOf("GET /VOLUMEUP") >= 0) {
              command = VolumeUp; 
              irsend.sendSAMSUNG(command, len);
            } 

            if (header.indexOf("GET /VOLUMEDOWN") >= 0) {
              command = VolumeDown; 
              irsend.sendSAMSUNG(command, len);
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server - TV Remote </h1>");
            client.println("</body></html>");

            // Buttons
            client.println("<p><a href=\"/POWERON\"><button class=\"button\">POWER ON</button></a></p>");
            client.println("<p><a href=\"/POWEROFF\"><button class=\"button\">POWER OFF</button></a></p>");
            client.println("<p><a href=\"/VOLUMEUP\"><button class=\"button\">VOLUME UP</button></a></p>");
            client.println("<p><a href=\"/VOLUMEDOWN\"><button class=\"button\">VOLUME DOWN</button></a></p>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  /* Send command to IRED via ESP32 
  uint8_t len = 32; 
  uint32_t command = PowerOn;
  irsend.sendSAMSUNG(command, len); */

  /* Receive any incoming IR 
  if (irrecv.decode(&results)){
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
  } */
}