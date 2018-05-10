
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>


MDNSResponder mdns;

// Replace with your network credentials
//const char* ssid = "Connection1";
const char* ssid = "Connection1";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 14;
int gpio2_pin = 0;

unsigned long  tassen=0;
int Single;
int Double; 
int taster= 1;
int trigger=0;
int enabel=0;

void setup(void){
  EEPROM.begin(512);
  /*
  webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
*/
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, INPUT_PULLUP);
  
  
  delay(1000);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection

  /*
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
  */
  server.on("/", [](){
    Single = EEPROM.read(0);
    Double= EEPROM.read(1);
  webPage ="";
  webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
    server.send(200, "text/html", webPage);
  });
 
  
  server.on("/socket1On", [](){
    
    Single = EEPROM.read(0);
    Single++;
    EEPROM.write(0,Single);
    EEPROM.commit();
webPage ="";
    webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
    server.send(200, "text/html", webPage);
    delay(100);
  });
  server.on("/socket1Off", [](){
    
    Single = EEPROM.read(0);
    Single--;
    EEPROM.write(0,Single);
    EEPROM.commit();
    webPage ="";
    webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
    server.send(200, "text/html", webPage);
    delay(100); 
  });


  
  server.on("/socket2On", [](){
    
    Double= EEPROM.read(1);
    Double++;
    EEPROM.write(1,Double);
    EEPROM.commit();
    webPage ="";
    webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
    server.send(200, "text/html", webPage);
    delay(100);
  });
  server.on("/socket2Off", [](){
    
    Double= EEPROM.read(1);
    Double--;
    EEPROM.write(1,Double);
    EEPROM.commit();
    webPage ="";
    webPage += "<h1>Espresso Muehle</h1><p>1 Tasse Espresso <a href=\"socket1On\"><button>+</button></a>&nbsp;<a href=\"socket1Off\"><button>-</button></a> ";
  webPage += String((int)Single*100);
  webPage += " ms</p>";
  webPage += "<p>2 Tasse Espresso <a href=\"socket2On\"><button>+</button></a>&nbsp;<a href=\"socket2Off\"><button>-</button></a> ";
  webPage += String((int)Double*100);
  webPage += " ms</p>";
    server.send(200, "text/html", webPage);
    delay(100); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
  

if (digitalRead(gpio2_pin) == 0){
  delay(20);
  tassen ++ ;
  trigger = 1;
  }
if (trigger == 1 && digitalRead(gpio2_pin) == 1){ enabel = 1;}


if ( tassen >= 1 && tassen <= 9 && enabel == 1 ){
  tassen = 0;
  trigger = 0;
  enabel = 0;
}
if ( tassen >= 10 && tassen <= 120 && enabel == 1 ){ 
  digitalWrite(gpio0_pin,HIGH) ;
  Serial.println ("Single");
  delay ( EEPROM.read(0) * 100 );
  digitalWrite(gpio0_pin,LOW) ;
  tassen = 0;
  trigger = 0;
  enabel = 0;
  
}

if ( tassen > 120 && enabel == 1)
{
  digitalWrite(gpio0_pin,HIGH) ;
  Serial.println ("Double");
  delay ( EEPROM.read(1) * 100 );
  digitalWrite(gpio0_pin,LOW) ;
  tassen = 0;
   trigger = 0;
   enabel = 0;
}

}
