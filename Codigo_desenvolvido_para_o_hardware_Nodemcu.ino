#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "password"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;


Servo servo1;
Servo servo2;

ESP8266WebServer server(8080);

String moveServo( int sr1, int sr2, int peso){
  
  Serial.println(sr1);
  Serial.println(sr2);
  Serial.println(peso);

  int peso2 = peso * 10 ;

  Serial.println(peso2);
 
  {if(sr1) {
    for (int angulo = 0; angulo < 30; angulo += 1) {
      servo1.write(angulo);
      delay(20); 
     }
  }
  
  if(sr2) {
    for (int angulo = 0; angulo < 30; angulo += 1) {
      servo2.write(angulo); 
      delay(20); 
     }
  } 
}delay(peso2 * 1000);

  if(sr1) {
    for (int angulo = 30; angulo >= 0; angulo -= 1) {
      servo1.write(angulo);
      delay(20); 
     }
  }
  if(sr2) {
    for (int angulo = 30; angulo >= 0; angulo -= 1) {
      servo2.write(angulo); 
      delay(20); 
     }
  }

  return "Feito";
}

void infoServo() {
  if (server.method() != HTTP_POST) {
    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(405, "application/json", "Method Not Allowed");
  } else {

    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(200, "application/json", "ok");
    
    String srv1;
    String srv2;
    String pesoS;
    String valores;
    
    valores = server.arg(0);
    srv1 = valores[0];
    srv2 = valores[2];
    pesoS = valores[4];
    
    moveServo(srv1.toInt(), srv2.toInt(), pesoS.toInt());
    Serial.println(srv1);
    Serial.println(srv2);
    Serial.println(pesoS);
  }
}



void handleNotFound()
{
    if (server.method() == HTTP_OPTIONS)
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
    }
    else
    {
        server.send(404, "text/plain", "");
    }
}

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  servo1.attach(2); //D4
  servo2.attach(5); //D1

  servo1.write(0);
  servo2.write(0);

  delay(2000);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/infoservo", infoServo);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}



void loop(void) {
  server.handleClient();
}
