#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
  
const char* ssid = "POCO X3 NFC";
const char* password = "RomMedAli";
const int DS18B20 = 2; //D4
 
WiFiServer server(80);
  
void setup() {
  Serial.begin(9600);
  delay(10);
  
  // Connect to WiFi network
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}
  
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  OneWire oneWire(DS18B20);
    
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);
  sensors.begin();
  sensors.requestTemperatures(); // Send the command to get temperatures
 
  Serial.print("Temp : ");
  Serial.println(sensors.getTempCByIndex(0));
   
  client.print("Temp : ");
  client.print(sensors.getTempCByIndex(0));
  client.println(" °C");
   
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
  
}
