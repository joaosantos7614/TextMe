#include <Wire.h>
#include <ESP8266WiFi.h>

const char* ssid = "Ludwig WAN Beethoven";       // Replace with your WiFi network SSID
const char* password = "AnanasBanana1234"; // Replace with your WiFi network password
WiFiServer server(80);

//const int ledPin = 14; // Onboard LED pin
//int ledStatus = 0;



void setup() {
  
  WiFi.hostname("WeMos D1 R1");
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C communication
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Device name: ");
  Serial.println(WiFi.hostname());
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Initialize the LED pin as an output
  //pinMode(ledPin, OUTPUT);
}

void loop() {
  
  String message = "";
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);

    String message = "";
    while (client.available()) {
      String headerLine = client.readStringUntil('\r');
      if (headerLine == "\n") {
        break; // Empty line marks the end of headers
      }
      Serial.println(headerLine);
      if (headerLine.indexOf("Message:")!=-1) {
        // Get the part of the string after "Message: "
        message = headerLine.substring(10); // Length of "Message: "
      }
      
    }
    
    

    
  
    // Respond to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    //client.println("Content-Length: 500");
    client.println("Connection: close");
    client.println("");
    client.println("<!DOCTYPE html><html><body>");
    client.println("<h1>Wemos D1 R1</h1>");
    client.println("<p>ESP8266 short message server</p>");
    client.println("</body></html>");
    client.println();

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
    Serial.println("");
    Serial.println(message);

    // Convert String to char array
    char messageArray[message.length() + 1];
    message.toCharArray(messageArray, sizeof(messageArray));
    
    // Send message to Arduino UNO 
    Wire.beginTransmission(9); // 9 is the address of the receiving Arduino
    Wire.write(messageArrayjps1234
    );
    Wire.endTransmission();
    //delay(1000);
  }
}
