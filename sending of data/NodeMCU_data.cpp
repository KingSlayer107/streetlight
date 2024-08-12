#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "Template Id"
#define BLYNK_TEMPLATE_NAME "Template Name"
#define BLYNK_AUTH_TOKEN "Authentication Token"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

char auth[] = "Authentication Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your WiFi Name";
char pass[] = "Your WiFi Password";

String receivedString = "";
BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  // Sync the states of the virtual pins with the server
  Blynk.syncVirtual(V0, V1, V2, V3, V4, V5);

  // Set up a timer to call the sendToBlynk function every 10 seconds (10000 milliseconds)
  timer.setInterval(1000L, sendToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
  if (Serial.available()) {
    receivedString = Serial.readStringUntil('\n');
    Serial.println("Received from Arduino: " + receivedString); // Print received data
    parseAndStoreData(receivedString);
  }
}

void parseAndStoreData(String data) {
  // Split the string into individual components
  int bulb1State = (data.indexOf("Bulb1:") != -1) ? 1 : 0;
  int bulb2State = (data.indexOf("Bulb2:") != -1) ? 1 : 0;
  int bulb3State = (data.indexOf("Bulb3:") != -1) ? 1 : 0;
  int lightsState = (data.indexOf("Lights:") != -1) ? 1 : 0;
  
  int dimmerPowerIndex = data.indexOf("Dimmer Power:") + String("Dimmer Power:").length();
  int dimmerPowerEnd = data.indexOf('%', dimmerPowerIndex);
  String dimmerPowerStr = data.substring(dimmerPowerIndex, dimmerPowerEnd);
  int dimmerPower = dimmerPowerStr.toInt();
  
  int currentIndex = data.indexOf("Current:") + String("Current:").length();
  int currentEnd = data.indexOf("mA", currentIndex);
  String currentStr = data.substring(currentIndex, currentEnd);
  float current = currentStr.toFloat();
  // Print parsed data
  // Store data in global variables to be sent to Blynk later
  storeData(bulb1State, bulb2State, bulb3State, lightsState, dimmerPower, current);
}

// Global variables to hold the data
int g_bulb1State, g_bulb2State, g_bulb3State, g_lightsState, g_dimmerPower;
float g_current;

void storeData(int bulb1State, int bulb2State, int bulb3State, int lightsState, int dimmerPower, float current) {
  g_bulb1State = bulb1State;
  g_bulb2State = bulb2State;
  g_bulb3State = bulb3State;
  g_lightsState = lightsState;
  g_dimmerPower = dimmerPower;
  g_current = current;
}

void sendToBlynk() {
  // Send stored data to Blynk
  Blynk.virtualWrite(V0, g_bulb1State);
  Blynk.virtualWrite(V1, g_bulb2State);
  Blynk.virtualWrite(V2, g_bulb3State);
  Blynk.virtualWrite(V3, g_lightsState);
  Blynk.virtualWrite(V4, g_dimmerPower);
  Blynk.virtualWrite(V5, g_current);
}
