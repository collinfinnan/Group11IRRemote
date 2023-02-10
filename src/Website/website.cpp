#include "website.h"

// Connect to Wi-Fi network with SSID and password
bool start_WiFi() {
  int count = 0; 
  int time_out = 10; 
  Serial.print("Connecting to ");
  Serial.println(NETWORK_NAME);
  WiFi.begin(NETWORK_NAME, NETWORK_PW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count == time_out) {
      return(false); 
    }
  }
  return(true);
}