#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// Replace with your network credentials
const char* ssid     = ""; // Enter your WIFI name
const char* password = ""; // Enter your WIFI password
const char* apiUrl = "http://192.168.1.106:5000/atyourservice/switch";
const unsigned long apiCallInterval = 100; // 60 seconds
unsigned long lastApiCallTime = 0;

WiFiClient wifiClient;
 
// Auxiliar variables to store the current output state
String output1State = "off";
 
// Assign output variables to GPIO pins
const int relay1 = D0;
// Current time
unsigned long currentTime = millis();
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
 
void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT); // Set relay pin as output
  digitalWrite(relay1, HIGH); // Initialize relay off (HIGH for inactive state)


 WiFi.begin(ssid, password); // Connect to Wi-Fi

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to make the next API call
  if (currentTime - lastApiCallTime >= apiCallInterval) {
    lastApiCallTime = currentTime;
    makeApiCall();
  }
}
 


void makeApiCall() {
  if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection
    HTTPClient http; // Create HTTP client

    Serial.println("Sending HTTP GET request...");
    http.begin(wifiClient, apiUrl); // Specify API endpoint
    int httpResponseCode = http.GET(); // Send GET request
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String payload = http.getString(); // Get the response payload
      Serial.println("Response:");
      Serial.println(payload);

      StaticJsonDocument<2048> doc; // Create a JSON document with a sufficient buffer size
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
         const char* message = doc["message"]; // Extract "message"
         Serial.println("Parsed JSON:");
         Serial.print("Message: ");
         Serial.println(message);
        
         JsonArray nestedField = doc["data"].as<JsonArray>();

         if (!nestedField.isNull()) {
          Serial.println("Nested Field:");
          for (JsonObject obj : nestedField) {
            int switch_id = obj["switch_id"];
            int switch_status = obj["switch_status"];
            const char* switch_name = obj["switch_name"];

            Serial.print("Switch Id: ");
            Serial.println(switch_id);
            Serial.print("Switch Name: ");
            Serial.println(switch_name);
            Serial.print("Switch Status: ");
            Serial.println(switch_status);
            Serial.println("-------------------");

            if (switch_status > 0) {
              Serial.println("GPIO 0 on");
              output1State = "on";
              digitalWrite(relay1, LOW);
            } else if (switch_status == 0) {
              Serial.println("GPIO 0 off");
              output1State = "off";
              digitalWrite(relay1, HIGH);
            }


          }
        } else {
          Serial.println("Nested Field is missing");
        }

      }else {
      Serial.print("Error in HTTP request: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
      }

    } else {
      Serial.print("Error in HTTP request1: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end(); // Free resources
  } else {
    Serial.println("Wi-Fi not connected");
  }
}
