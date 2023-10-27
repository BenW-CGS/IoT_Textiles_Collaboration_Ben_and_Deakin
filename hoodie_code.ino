/*
*  Weather Station App
*  CGS Semester 2
*  Task 2
*  Author: Ben W
*/

//Access libraries
#include <math.h>
#include <WiFi.h>
#include <aREST.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// DHT11 sensor pins
#define DHTPIN 26
#define DHTTYPE DHT11

//pins for lights
int redPin= 12;
int greenPin = 14;
int bluePin = 33;

// Create aREST instance
aREST rest = aREST();

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// WiFi parameters. Get network and password to access internet.
const char* ssid = "Proxima";
const char* password = "centauri";
//Static IP address configuration
// P connections 
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float temperature;
float humidity;
char* location = "ACT";
int timer = 72000;
float fireindex;

// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  //setup lights
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  // Init DHT 
  dht.begin();
  
  // Init variables and expose them to REST API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("location",&location);
  rest.variable("fireindex",&fireindex);
    
  // Give name and ID to device
  rest.set_id("141");
  rest.set_name("KingOfThePirates");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);//, password);
  IPAddress ip(192, 168, 1, 141); //set static ip
  IPAddress gateway(192, 168, 1, 1); //set getteway
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);
  IPAddress subnet(255, 255, 255, 0);//set subnet
  WiFi.config(ip, gateway, subnet);

  //Tell me wifi status.
  while (WiFi.status() != WL_CONNECTED) {
    for(int i=0;i<20;i++){
      delay(500);
      Serial.print(".");
    }
    if(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
}

void loop() {
  
  // Reading temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  fireindex = 1 + sqrt(100 - humidity) + sqrt(temperature);

  // Prints data to serial.
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  //Fire index is my idea. See, I Do understand!
  Serial.print("Benjamin's Fire Index: ");
  Serial.println(fireindex);
  Serial.print("Timer: ");
  Serial.println(timer);
  if (temperature < 5){
    setColor(0, 0, 255);
  } else if(temperature < 10){
    setColor(33, 0, 166);
  } else if (temperature < 15){
    setColor(66, 0, 133);
  } else if(temperature < 20){
    setColor(100, 0, 100);
  } else if (temperature < 25){
    setColor(133, 0, 66);
  } else if (temperature < 30){
    setColor(166, 0, 33);
  } else {
    setColor(255, 0, 0);
  }
  delay(5000);
  timer--;

  //Check running time and reset if expired
  if (timer == 0 ) {
    delay(3000);
    Serial.println("Resetting..");
    ESP.restart();
  }
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  //Serial.println("called");
}

// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6,state);
  return 1;
}

void setColor(int redValue, int greenValue, int blueValue) {
  //Takes RGB, and sets each colour to right amount. Analog write sets colour. Simplifies colours.
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
