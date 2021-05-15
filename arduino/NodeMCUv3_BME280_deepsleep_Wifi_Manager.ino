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
#include <WiFiManager.h>

//const char* ssid = "WIFI-SSID";
//const char* password = "WIFI-PASSWORD";

const char* serverName = "http://tonofarm.herokuapp.com/esp-post-data.php";

String apiKeyValue = "API-KEY";
//String sensorName = "BME280";
String sensorLocation = "12.7581423,102.1468503";

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

#define uS_TO_S_FACTOR 1000000     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */

// Chip ID
unsigned long chipId = ESP.getChipId();

float Temp;
float Humidity;
float Pressure;

// VBatt
ADC_MODE(ADC_VCC);
float vbatt;

// RSSI
long rssi;

void connect() {
   // We start by connecting to a WiFi network
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
  
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    //while (1);
    // Restart ESP
    ESP.restart();
  }

  // BME280 Sensor
  Temp = bme.readTemperature();
  Humidity = bme.readHumidity();
  Pressure = bme.readPressure() / 100.0F;

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
//      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + chipId
//                            + "&location=" + sensorLocation + "&value1=" + String(bme.readTemperature())
//                            + "&value2=" + String(bme.readHumidity()) + "&value3=" + String(bme.readPressure()/100.0F) + "&value4=" + rssi +  "&value5=" + vbatt + "";
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

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

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
  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("AutoConnectAP");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
  
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
  // Deep Sleep 
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
}

void loop() {
}