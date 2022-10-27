#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Set GPIOs for LED and reedswitch
const int reedSwitch = D2;

// Detects whenever the door changed state
bool changeState = false;

// Holds reedswitch state (1=opened, 0=close)
bool state;
String doorState;

unsigned long previousMillis = 0; 
const long interval = 1500;

const char* ssid = "MotoModem";
const char* password = "beriGoodPassword21";

const char* mqtt_server = "192.168.0.21";
const char* mqtt_topic = "stoplight";
const char* clientID = "magSensor";

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

int count = 0;

// Runs whenever the reedswitch changes state
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State changed");
  changeState = true;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Read the current door state
  pinMode(reedSwitch, INPUT_PULLUP);
  state = digitalRead(reedSwitch);

  // Set the reedswitch pin as interrupt, assign interrupt function and set CHANGE mode
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE);

  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  if (client.connect(clientID)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

}

void loop() {
  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // If a state has occured, invert the current door state   
      state = !state;
      if(state) {
        doorState = "closed";
          if (client.publish(mqtt_topic, "closed")) {
              Serial.println("doorstate sent");
          }
      }
      else{
        doorState = "open";
        if (client.publish(mqtt_topic, "open")) {
              Serial.println("doorstate sent");
        }
      }
      //digitalWrite(led, state);
      changeState = false;
      Serial.println(state);
      Serial.println(doorState);
    }  
    delay(1000);
  }
}
