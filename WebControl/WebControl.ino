#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "YONKOVI";
const char *password = "0887694113";

const int motorA = D1;//left
const int motorB = D2;//right
const int revMotorA = D3;//left reverce
const int revMotorB = D4;//right reverce

const int motorSpeed = 255;

int WhiteLed = D7;
int WhiteLedStatus = 0;
int RedLed = D8;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Define server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/turnOn", HTTP_GET, handleTurnOn);
  server.on("/forward", HTTP_GET, handelForward);
  server.on("/backwards", HTTP_GET, handelBackwards);
  server.on("/left", HTTP_GET, handelLeft);
  server.on("/right", HTTP_GET, handelRight);
  server.on("/breaks", HTTP_GET, handelBreaks);

  //set pins
  pinMode(motorA , OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(revMotorA, OUTPUT);
  pinMode(revMotorB, OUTPUT);
  pinMode(WhiteLed, OUTPUT);
  pinMode(RedLed, OUTPUT);
  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}


void handleRoot() {
  // Read HTML file from SPIFFS
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  // Send HTML content as the response
  server.streamFile(file, "text/html");
  file.close();
}

void handleTurnOn() {
  // Add code to turn on your device or perform a specific action
  server.send(200, "text/plain", "Turned on!");

  if(WhiteLedStatus == 0){
    digitalWrite(WhiteLed, HIGH);
    WhiteLedStatus = 1;
  }
  else {
    digitalWrite(WhiteLed, LOW);
    WhiteLedStatus = 0;
  }
  

  Serial.println("Turn ON");
}

void handelForward() {
  server.send(200, "text/plain", "forward");
  Serial.println("Moving forward!");

  analogWrite(motorA, motorSpeed);
  digitalWrite(revMotorA, HIGH);

  analogWrite(motorB, motorSpeed);
  digitalWrite(revMotorB, LOW);
}

void handelBackwards() {
  server.send(200, "text/plain", "backwards");
  Serial.println("Moving Backwards!");

  analogWrite(motorA, motorSpeed);
  digitalWrite(revMotorA, LOW);

  analogWrite(motorB, motorSpeed);
  digitalWrite(revMotorB, HIGH);
}

void handelLeft() {
  server.send(200, "text/plain", "left");
  Serial.println("Moving left!");

  analogWrite(motorA, motorSpeed);
  digitalWrite(revMotorA, LOW);

  analogWrite(motorB, motorSpeed);
  digitalWrite(revMotorB, LOW);
}

void handelRight() {
  server.send(200, "text/plain", "right");
  Serial.println("Moving right!");

  analogWrite(motorA, motorSpeed);
  digitalWrite(revMotorA, HIGH);

  analogWrite(motorB, motorSpeed);
  digitalWrite(revMotorB, HIGH);
}


void handelBreaks() {
  server.send(200, "text/plain", "breaks");
  Serial.println("breaks!");
  
  digitalWrite(RedLed, HIGH);
  
  analogWrite(motorA, LOW);
  digitalWrite(revMotorA, LOW);

  analogWrite(motorB, LOW);
  digitalWrite(revMotorB, LOW);

  delay(200);
  digitalWrite(RedLed, LOW);
}
