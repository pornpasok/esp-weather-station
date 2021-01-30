#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59        /* Time ESP will go to sleep (in seconds) */
#define SEALEVELPRESSURE_HPA (1013.25)
long timeout; // Time to WiFi Connect

// WiFi Connect
const char* ssid = "YOUR-WIFI-SSID";
const char* password = "YOUR-WIFI-PASSWORD";

// Heroku App URL
const char* serverName = "http://app-name.herokuapp.com/esp-post-data.php";

String apiKeyValue = "API-KEY";
//String sensorName = "BME280";
String sensorLocation = "37.8718992,-122.2585399";

// Chip ID
unsigned long chipId = ESP.getChipId();

float Temp;
float Humidity;
float Pressure;

// VBatt
ADC_MODE(ADC_VCC); // ESP8266 Only
float vbatt;

// RSSI (WiFi Signal)
long rssi;

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

void connect() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    timeout = millis();
    Serial.println(timeout);

    // Time Out got deep_sleep_mode save battery
    if (timeout > 30000) {
      Serial.print("TIME_OUT: ");
      Serial.println(timeout);
      Serial.println("Sleeping 10 minutes ..");
      ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");

    // Random BME280 Sensor Value for Testing
    Temp = random(20, 30);
    Humidity = random(50, 100);
    Pressure = random(1000, 1100);
    
    // Restart ESP
    //ESP.restart();
  }
  else {
    // BME280 Sensor
    Temp = bme.readTemperature();
    Humidity = bme.readHumidity();
    Pressure = bme.readPressure() / 100.0F;
  }

  // Chip ID
  Serial.print("Chip ID: ");
  Serial.println(chipId);

  Serial.print("Temperature: ");
  Serial.print(Temp);
  Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" %\t");
  Serial.print("Pressure: ");
  Serial.print(Pressure);
  Serial.println(" hPa");

  // VCC
  vbatt = ESP.getVcc() / 1000.0F;
  Serial.print("VCC: ");
  Serial.println(vbatt);

  // Wi-Fi Signal
  rssi = WiFi.RSSI();
  Serial.print("WiFi Signal: ");
  Serial.println(rssi);
  
  // Use WiFiClient class to create TCP connections
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + chipId
                            + "&location=" + sensorLocation + "&value1=" + Temp
                            + "&value2=" + Humidity + "&value3=" + Pressure + "&value4=" + rssi +  "&value5=" + vbatt + "";

      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      // Show Connect Status
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(1000);

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
      delay(1000);
   }
   else {
    Serial.println("WiFi Disconnected");
   }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.setTimeout(2000);
  // Wait for serial to initialize.
  while (!Serial) { }

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
