
/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com 
  look at ESP8266WiFiSTA.cpp 
  use \" when compiler dos not accept " in the HTML code
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


MDNSResponder mdns;

// Replace with your network credentials

const char* ssid = "PanFeelOff";
const char* password = "Ghbdtn2402";

// config static IP
//IPAddress ip(192, 168, 0, 15); // ip of the WiFiPP
IPAddress ip(192, 168, 0, 14); // where xx is the desired IP Address
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
bool autoReconnect = true; //enables autoconnect

ESP8266WebServer server(614);
//ESP8266WebServer::ESP8266WebServer(IPAddress addr, int port)
//: _server(addr, port)

//String webPage = "";

const char * webPage = " <!DOCTYPE html> "

"<head><meta name=viewport content='width=device-width'></head>"
"<html>"
"<head>"
"<link rel=\"shortcut icon\" href=\"#\">"
"</head>"
"<body onload=\"body()\">"

"<h1>Garage</h1>"
"<p>Door <button onclick=\"s1()\">UP/DOWN</button></p>"
"<p>Light <button onclick=\"s2()\">ON/OFF</button></p>"

"<br/>"
"<p id=\"demo\"></p>"
"<script>"
"function s1() {"
"window.location.href = \"socket1On\";"
"}"

"function s2() {"
"window.location.href = \"socket2On\";"
"}"

"function body() {"
"var pathname = window.location.href;"
"document.getElementById(\"demo\").innerHTML = pathname;"
"if(pathname==\"http://72.141.74.133:614/socket1On\"||pathname==\"http://72.141.74.133:614/socket2On\"){"
"window.setTimeout('window.open(\"http://72.141.74.133:614\",\"http://72.141.74.133:614/socket1On\")',1000);"
"clearTimeout();}"
"else if(pathname==\"http://192.168.0.14:614/socket1On\"||pathname==\"http://192.168.0.14:614/socket2On\"){"
"window.setTimeout('window.open(\"http://192.168.0.14:614\",\"http://192.168.0.14:614/socket1On\")',1000);"
"clearTimeout();}"

"}"
"</script>"

"</body>" 
"</html>";


int gpio2_pin = 2;
int gpio4_pin = 4;
int gpio5_pin = 5;


void setup(void)
{
//  webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
//  webPage += "<head><meta name=viewport content='width=device-width'></head>";
//  webPage +="<h1>ESP8266 Web Server</h1>";
//  webPage += "<p>Socket #1 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";

  
  // preparing GPIOs
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW); //was high
  pinMode(gpio4_pin, OUTPUT);
  digitalWrite(gpio4_pin, HIGH);
  pinMode(gpio5_pin, OUTPUT);
  digitalWrite(gpio5_pin, HIGH);  


  //setup server
  WiFi.setAutoReconnect(autoReconnect);

  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  WiFi.config(ip, gateway, subnet);
  
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
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    }
  
  server.on("/", [](){ //server.on("received URL", {what to do})
    server.send(200, "text/html", webPage);  //server.send(200, "type ot the info", content)
  });


  //interpret data from server
//    server.on("1", [](){
//    server.send(200, "text/html", "Fine");
//    //Serial.println("socket2Off");
//    delay(1000);
//  });

 
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio5_pin, LOW);
    delay(1000);
    digitalWrite(gpio5_pin, HIGH);
    delay(1000);
    Serial.println("Button toggled");
  });
  

    server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio4_pin, LOW);
    delay(1000);
    digitalWrite(gpio4_pin, HIGH);
    delay(1000);
  });



  server.begin();
  Serial.println("HTTP server started");
 // digitalWrite(gpio2_pin, LOW);
}
 
void loop(void){
  //LED indicates that wifi is connected
   if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(gpio2_pin, LOW);
  }
  
  //If wifi is disconnected, reconnect
   if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(gpio2_pin, LOW);
    WiFi.begin(ssid, password);
    delay(1000);
    }
   
  server.handleClient();
} 
