#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include <TimerOne.h>
#include <RBDdimmer.h>
#include <Filters.h>
#include <RunningStatistics.h>

#define USE_SERIAL Serial
#define ldrPin1 A0
#define ldrPin2 A1
#define ldrPin3 A2
#define ACS_Pin A4

const int outputPin = 10;
const int zerocross = 2;
const int trigPin = 7;
const int echoPin = 6;

const unsigned long toggleInterval = 20000;
const unsigned long motionTimeout = 5000;
unsigned long printPeriod = 1000; // in milliseconds
unsigned long previousMillis = 0;
unsigned long lastToggleTime = 0;
unsigned long lastMotionTime = 0;

float ACS_Value;
float testFrequency = 0.5;
float windowLength = 40.0 / testFrequency;
float intercept = 0;
float slope = 0.0752;
float Amps_TRMS;

bool isNightTime = false;
bool isLightsOn = false;

dimmerLamp dimmer(outputPin);
Ultrasonic ultrasonic(trigPin, echoPin);

void setup() {
  USE_SERIAL.begin(9600);
  dimmer.begin(NORMAL_MODE, ON);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(ldrPin1, INPUT);
  pinMode(ldrPin2, INPUT);
  pinMode(ldrPin3, INPUT);
  pinMode(ACS_Pin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // RunningStatistics for AC current measurement
  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);

  // Update LDR values and bulb status
  int ldrValue1 = analogRead(ldrPin1);
  int ldrValue2 = analogRead(ldrPin2);
  int ldrValue3 = analogRead(ldrPin3);
  bool isBulbOn1 = (ldrValue1 < 400);
  bool isBulbOn2 = (ldrValue2 < 400);
  bool isBulbOn3 = (ldrValue3 < 400);

  // Combine motion detection and light control logic
  if (currentMillis - lastToggleTime >= toggleInterval) {
    isNightTime = !isNightTime;
    lastToggleTime = currentMillis;
    isLightsOn = isNightTime; // Turn on lights when it becomes night time
    if (!isLightsOn) {
      dimmer.setPower(0); // Turn off lights if day
    }
  }

  if (isLightsOn) {
    // Read distance from ultrasonic sensor if night
    int distance = ultrasonic.read();
    // Handle motion detection
    if (distance > 0 && distance < 100) { // Adjust the distance threshold as needed
      // Motion detected
      lastMotionTime = currentMillis; // Update last motion time
      dimmer.setPower(80); // Increase intensity of bulb to 80%
    } else {
      // No motion detected
      if (currentMillis - lastMotionTime >= motionTimeout) {
        dimmer.setPower(40); // Decrease intensity of bulb to 40%
      }
    }
  } else {
    // Daytime, turn off the light
    dimmer.setPower(0); // Turn off light
  }

  // Read AC current and calculate TRMS
  int ACS_Value = analogRead(ACS_Pin);
  inputStats.input(ACS_Value);
  if (currentMillis - previousMillis >= printPeriod) {
    previousMillis = currentMillis; // Update time
    Amps_TRMS = 1000 * (intercept + slope * inputStats.sigma());

    // Create and send the data string
    String dataString = createDataString(isBulbOn1, isBulbOn2, isBulbOn3, isLightsOn, dimmer.getPower(), Amps_TRMS);
    USE_SERIAL.println(dataString);
  }

  delay(1000); // Delay for readability
}

String createDataString(bool isBulbOn1, bool isBulbOn2, bool isBulbOn3, bool isLightsOn, int dimmerPower, float ampsTRMS) {
  String dataString = "";
  dataString += "Bulb1:" + String(isBulbOn1 ? "ON" : "OFF") + ",";
  dataString += "Bulb2:" + String(isBulbOn2 ? "ON" : "OFF") + ",";
  dataString += "Bulb3:" + String(isBulbOn3 ? "ON" : "OFF") + ",";
  dataString += "Lights:" + String(isLightsOn ? "ON" : "OFF") + ",";
  dataString += "Dimmer Power:" + String(dimmerPower) + "%,";
  dataString += "Current:" + String(ampsTRMS) + "mA";
  return dataString;
}