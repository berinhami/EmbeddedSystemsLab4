#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "MotoModem";
const char* password = "beriGoodPassword21";

const char* mqtt_server = "192.168.0.21";
const char* mqtt_topic = "stoplight";
const char* clientID = "lightserver";

//variables
int distance = 0;
int red = D5;
int yellow = D6;
int green = D7;
bool turniton = false;
bool doorOpen = false;
String distanceColor = "-";
const unsigned long eventInterval = 400;
unsigned long previousTime = 0;
int countme = 0;

//WiFiServer server(80);
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void ReceivedMessage(char* topic, byte* payload, unsigned int length) {

  if ((char)payload[1] == 'R') {
      distanceColor = "redLight";
  }
  if ((char)payload[1] == 'Y') {
      distanceColor = "yellowLight";
  }
  if ((char)payload[1] == 'G') {
      distanceColor = "greenLight";
  }
  if ((char)payload[1] == 'O') {
      distanceColor = "none";
  }
  if ((char)payload[1] == 'C') {
      distanceColor = "all";
  }
  if ((char)payload[1] == 'p') {
    doorOpen = true;
    Serial.println(doorOpen);
  }
  if ((char)payload[1] == 'l') {
    doorOpen = false;
    Serial.println(doorOpen);
  }
}

bool Connect() {
  // Connect to MQTT Server and subscribe to the topic
  if (client.connect(clientID)) {
      client.subscribe(mqtt_topic);
      return true;
    }
    else {
      return false;
  }
}

void setup() {
  Serial.begin(115200);
  
  //set up pins
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
 
  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  client.setCallback(ReceivedMessage);
  if (Connect()) {
    Serial.println("Connected Successfully to MQTT Broker!");  
  }
  else {
    Serial.println("Connection Failed!");
  }
 
}
 
void loop() {

    if (!client.connected()) {
    Connect();
  }
  client.loop();

  if(doorOpen){
  //cycle code
   unsigned long currentTime = millis();
   if(turniton == true){
      if(currentTime - previousTime >= eventInterval){
        if(countme % 2 == 0){
     
          digitalWrite(red, HIGH);
          digitalWrite(yellow, LOW);
          digitalWrite(green, LOW);
        }
        if(countme % 2 == 1){
  
          digitalWrite(red, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(green, LOW);
        }
        countme++;
        previousTime = currentTime;
      }
    }

    if (distanceColor == "redLight"){
      turniton = false;
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
    //  Serial.println("Im red");
    } 
    if (distanceColor == "yellowLight"){
      turniton = false;
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
   //   Serial.println("Im yellow");
    }
    if (distanceColor == "greenLight"){
      turniton = false;
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, HIGH);
   //   Serial.println("Im green");
    }
    if (distanceColor == "none"){
      turniton = false;
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
     // Serial.println("Im Off");
    }
    if (distanceColor == "all"){  
      turniton = true;
    }
  }
  else{
    turniton = false;
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
  }
}
