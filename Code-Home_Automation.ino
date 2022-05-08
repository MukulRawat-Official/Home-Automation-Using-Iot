// sending led data from node mcu to server
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;
WiFiServer server(80);

#define relaylow HIGH
#define relayhigh LOW
#define led D6

long myChannelNumber = CONFIDENTIAL;
const char myWriteAPIKey[] = "CONFIDENTIAL";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("NodeMCU Started!");
  WiFi.begin("sunny", "12345670");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.print("To wifi ip =  ");
  Serial.println(WiFi.localIP());
  server.begin();
  ThingSpeak.begin(client);
  pinMode(led,OUTPUT);
  digitalWrite(led,relaylow);
  
}

void loop() {

  client = server.available();  //Gets a client that is connected to the server and has data available for reading.    
  if (client == 1)
  {  
    String request =  client.readStringUntil('\n');
  // Serial.println(request);
    request.trim();
    if(request == "GET /ledon HTTP/1.1")
    {
     Serial.println("BULB ON");
      digitalWrite(led, relayhigh);
      ThingSpeak.writeField(myChannelNumber, 2, 1, myWriteAPIKey); // 2 == graph 1 = value
    }
    if(request == "GET /ledoff HTTP/1.1")
    {
      Serial.println("BULB OFF");
      digitalWrite(led, relaylow);
      ThingSpeak.writeField(myChannelNumber, 2, 0, myWriteAPIKey);
    }
      //client.println("Content-Type: text/html");
  //client.println("");
  //client.println("<!DOCTYPE HTML>");

  }
  client.println("<html>");
  client.println("<h1>Welcome to the Webpage!</h1>");
  client.println("<h3>LED Controls<h3>");
  client.println("<br>");
  client.println("<a href=\"/ledon\"\"><button style='background-color:green;border-color:green;color:white'>LED ON</button></a>");
  client.println("<a href=\"/ledoff\"\"><button style='background-color:red;border-color:red;color:white'>LED  OFF</button></a><br/>");
  client.println("</html>");
}
