#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>
const char* ssid = "Redmi4";
const char* password = "";
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
const int API_TIMEOUT = 10000;
SoftwareSerial NodeMCU(D2,D4);
void setup() {
 // Open serial communications and wait for port to open:
 Serial.begin(115200);
 NodeMCU.begin(9600);
 pinMode(D2,INPUT);
 pinMode(D3,OUTPUT);
}
void loop() { // run over and over
 if(NodeMCU.available()>0){
 float val = NodeMCU.parseFloat();
 if(NodeMCU.read()== '\n'){
 Serial.println(val);
 if(val<10)
 {
 Serial.println("activity");
 Serial.println();
 Serial.print("connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 if (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 BearSSL::WiFiClientSecure client;
 client.setInsecure();
 client.setTimeout(API_TIMEOUT);
 Serial.print("connecting to ");
 Serial.println(host);
 if (!client.connect(host, httpsPort)) {
 Serial.println("connection failed");
 return;
 }
String url = "https://maker.ifttt.com/trigger/ESP/with/key/oiUoLXmHdqUEkH25RtbFFZV_-
jHSQcB9LApT5DgYBOP";
 Serial.print("requesting URL: ");
 Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
 "Host: " + host + "\r\n" +
 "User-Agent: BuildFailureDetectorESP8266\r\n" +
 "Connection: close\r\n\r\n");
 Serial.println("request sent");
 while (client.connected()) {
 String line = client.readStringUntil('\n');
 if (line == "\r") {
 Serial.println("headers received");
 break;
 }
 }
 String line = client.readStringUntil('\n');
 Serial.println("reply was:");
 Serial.println("==========");
 Serial.println(line);
 Serial.println("==========");
 Serial.println("closing connection");
 delay(10000);
 }
 }
 }
 delay(1000);
}
