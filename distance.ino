#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "MotoModem";
const char* password = "beriGoodPassword21";

const char* mqtt_server = "192.168.0.21";
const char* mqtt_topic = "stoplight";
const char* clientID = "distancelights";

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);


const int echo = D5;
const int trig = D6;

int duration = 0;
int distance = 0;
bool doorOpen = false;
String send = "-";

void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  Serial.begin(115200);
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
  // put your main code here, to run repeatedly:
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration/2) /28.5;
  Serial.println(distance);

  if(distance > 30){
    if(client.publish(mqtt_topic, "/OFF")){
      Serial.print("off");
    }
  }
  else if(distance > 20){
    if(client.publish(mqtt_topic, "/GREEN")){
      Serial.print("green");
    }
  }
  else if(distance > 10){
    if(client.publish(mqtt_topic, "/YELLOW")){
      Serial.print("yellow");
    }
  }
  else if(distance > 5){
    if(client.publish(mqtt_topic, "/RED")){
      Serial.print("red");
    }
  }
  else{
      if(client.publish(mqtt_topic, "/CYCLE")){
          Serial.print("cycle");
      }
  }
  delay(1000);
  
}
