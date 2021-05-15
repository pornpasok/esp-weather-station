#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Sodaq_SHT2x.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59        /* Time ESP will go to sleep (in seconds) */

long timeout; // Time to WiFi Connect

const char* ssid = "YOUR-WIFI-SSID";
const char* password = "YOUR-WIFI-PASSWORD";
const char* host = "http://app-name.herokuapp.com/esp-post-data.php";
const char* api   = "API-KEY";  //API Key
String sensorLocation = "12.7581423,102.1468503";

// VBatt
ADC_MODE(ADC_VCC); // ESP8266 Only
float vbatt;

// Chip ID
unsigned long chipId = ESP.getChipId();

// RSSI (WiFi Signal)
long rssi;

void connect() {
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    // LED Status
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    Serial.print(".");
    timeout = millis();
    Serial.println(timeout);

    // Time Out got deep_sleep_mode save battery
    if (timeout > 30000) {
      Serial.print("TIME_OUT: ");
      Serial.println(timeout);
      Serial.print("Sleeping ");
      Serial.print(TIME_TO_SLEEP);
      Serial.println(" seconds ..");
      // Deep Sleep TIME_TO_SLEEP seconds
      ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Temp
  Serial.print("Temperature(C): ");
  Serial.println(SHT2x.GetTemperature());
  Serial.print("Humidity(%RH): ");
  Serial.println(SHT2x.GetHumidity());
  Serial.print("Dewpoint(C): ");
  Serial.println(SHT2x.GetDewPoint());
  Serial.println("--------------------");
  
  float t = SHT2x.GetTemperature();
  float h = SHT2x.GetHumidity();
  
  // Chip ID
  Serial.print("Chip ID: ");
  Serial.println(chipId);
  
  // VCC
  vbatt = ESP.getVcc() / 1000.0F;
  Serial.print("VCC: ");
  Serial.println(vbatt);

  // Wi-Fi Signal
  rssi = WiFi.RSSI();
  Serial.print("WiFi Signal: ");
  Serial.println(rssi);
  
  Serial.print("connecting to ");
  Serial.println(host);

  // Show Connect Status
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);
  Serial.println("LED ON");

  HTTPClient http;
  http.begin(host);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // We now create a URI for the request
  String httpRequestData = "api_key=";
  httpRequestData += api;
  httpRequestData += "&sensor=";
  httpRequestData += chipId;
  httpRequestData += "&location=";  
  httpRequestData += sensorLocation;
  httpRequestData += "&value1=";  
  httpRequestData += t;
  httpRequestData += "&value2=";  
  httpRequestData += h;
  httpRequestData += "&value3=";  
  httpRequestData += 0;
  httpRequestData += "&value4=";  
  httpRequestData += rssi;
  httpRequestData += "&value5=";  
  httpRequestData += vbatt;
  
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);
  Serial.println("LED OFF");
}

void setup(void) {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  delay(10);
  pinMode(D4, OUTPUT);
  Wire.begin();
  // Wait for serial to initialize.
  //while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  connect();

  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");
  // Deep Sleep TIME_TO_SLEEP seconds
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
}

void loop() {
}
