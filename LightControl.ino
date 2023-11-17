/*
*  Security Alarm
*  CGS Semester 2
*  Task 2
*  Author: Phoenix, ObjectivistApe
*/
// Libraries if we need them in a (theoretical) later stage of development
#include <math.h>
#include <WiFi.h>
#include <aREST.h>
#include <future>
#include <string>

// Lights
#define Lights 12
#define Inbuilt 2
// Brightness sensors
#define bright0 14
#define bright1 27
#define bright2 26
#define bright3 25



// WiFi parameters
const char* ssid = "C001R@V3G@m35";

// An array to handle the different brightnesses better
int Brights[4] = { bright0, bright1, bright2, bright3 };

void setup(void)
{
  pinMode(Lights, OUTPUT);
  pinMode(Inbuilt, OUTPUT);
  analogWrite(Inbuilt, 255);
  for (int i = 0; i < 4; i++) {
    pinMode(Brights[i], INPUT);
  }
  
  // Start Serial
  Serial.begin(2000000);

  // Create WiFI to connect audio player website through
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, "", 1, 0, 1, 2);
}

void loop() {
  int lowest = 255;
  for (int i = 0; i < 4; i++) {
    if (map(analogRead(Brights[i]), 0, 4095, 0, 255) < lowest) {
      lowest = map(analogRead(Brights[i]), 0, 4095, 0, 255);
    }
  }
  analogWrite(Lights, lowest);
}
