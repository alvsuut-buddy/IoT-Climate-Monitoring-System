#define BLYNK_TEMPLATE_ID "[BLYNK_TEMPLATE_ID]"
#define BLYNK_TEMPLATE_NAME "[BLYNK_TEMPLATE_NAME]"
#define BLYNK_AUTH_TOKEN "[BLYNK_AUTH_TOKEN]"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h> 
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>

#define FIREBASE_HOST "[FIREBASE_HOST]"
#define FIREBASE_AUTH "[FIREBASE_AUTH]"

const char* ssid = "[SSID_WIFI]";
const char* password = "[PASSWORD_WIFI]";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000)

#define DHTPIN 2        // D4 on NodeMCU (GPIO 2)
#define DHTTYPE DHT11   // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

const int buttonPin = D3; // Button connected to D3

// LED configuration
const int yellowLedPin = D7;  // Yellow LED pin
const int greenLedPin = D8;   // Green LED pin

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Firebase setup
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long previousMillis = 0;
const long interval = 60000; // 1 minute in milliseconds

float maxTemperature = -1000.0; 
float maxHumidity = -1000.0;     
float minHumidity = 1000.0;    
int previousDay = -1;
bool showMaxMin = false; 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("DHT11 Sensor Test");

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

  // Initialize LED pins as output
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  }

lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  timeClient.begin();

  // Initialize Blynk
  delay(200);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to");
  lcd.setCursor(0,1);
  lcd.print("blynk");
  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected to");
    lcd.setCursor(0, 1);
    lcd.print("blynk");
    Serial.println("Connected to blynk");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed connecting");
    lcd.setCursor(0, 1);
    lcd.print("to blynk");
    Serial.println("Failed connecting to blynk");
  }
  
  if (Firebase.getFloat(firebaseData, "/minMax/maxValues/temperature")) {
    maxTemperature = firebaseData.floatData();
  }
  if (Firebase.getFloat(firebaseData, "/minMax/minValues/temperature")) {
    minTemperature = firebaseData.floatData();
  }
  if (Firebase.getFloat(firebaseData, "/minMax/maxValues/humidity")) {
    maxHumidity = firebaseData.floatData();
  }
  if (Firebase.getFloat(firebaseData, "/minMax/minValues/humidity")) {
    minHumidity = firebaseData.floatData();
  }

  // Fetch the last saved day from Firebase to check if a new day starts
  if (Firebase.getInt(firebaseData, "/minMax/day")) {
    previousDay = firebaseData.intData();
  }
  timeClient.update();
  int currentDay = timeClient.getDay();  // You can use getDay to get the day of the year

  // Check if the day has changed
  if (currentDay != previousDay) {
    // Reset max and min values for a new day
    maxTemperature = -1000.0;
    minTemperature = 1000.0;
    maxHumidity = -1000.0;
    minHumidity = 1000.0;
    
    // Save the new day to Firebase
    Firebase.setInt(firebaseData, "/minMax/day", currentDay);
    previousDay = currentDay;
    Serial.println("New day detected, max/min values reset.");
  }

void loop() {
  // Run Blynk
  Blynk.run();

  // Delay for sensor readings
  delay(2000);

  // Reading temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error    ");
    return;
  }

 lcd.clear();
  if (digitalRead(buttonPin)) {
    // Show current temperature and humidity
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
    
  } else {
    // Show max/min values
    lcd.setCursor(0, 0);
    lcd.print("Max Temp: ");
    lcd.print(maxTemperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Max Humi: ");
    lcd.print(maxHumidity);
    lcd.print("%");

    delay(2000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Min Temp: ");
    lcd.print(minTemperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Min Humi: ");
    lcd.print(minHumidity);
    lcd.print("%");
  }

  if (temperature > 28) {
    Blynk.logEvent("suhu", "Suhu ruangan melebihi 28 C");
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
  }

  if (temperature > maxTemperature) {
    Firebase.setFloat(firebaseData, "/minMax/maxValues/temperature", temperature);
    maxTemperature = temperature;
    Serial.println("New max temp has been updated");
  }
  if (temperature < minTemperature) {
    Firebase.setFloat(firebaseData, "/minMax/minValues/temperature", temperature);
    minTemperature = temperature;
    Serial.println("New min temp has been updated");
  }
  if (humidity > maxHumidity) {
    Firebase.setFloat(firebaseData, "/minMax/maxValues/humidity", humidity);
    maxHumidity = humidity;
    Serial.println("New max humidity has been updated");
  }
  if (humidity < minHumidity) {
    Firebase.setFloat(firebaseData, "/minMax/minValues/humidity", humidity);
    minHumidity = humidity;
    Serial.println("New min humidity has been updated");
  }
  
  // Get current time
  unsigned long currentMillis = millis();

  // If 1 minute has passed, upload data to Firebase
  if (currentMillis - previousMillis >= interval) {
    timeClient.update();
    unsigned long timestamp = timeClient.getEpochTime();
    previousMillis = currentMillis;

    // Create a JSON object with temperature, humidity, and timestamp
    FirebaseJson json;
    json.set("temperature", temperature);
    json.set("humidity", humidity);
    json.set("timestamp", timestamp);  // Add timestamp

    // Upload to Firebase
    String path = "/sensorData";
    if (Firebase.pushJSON(firebaseData, path, json)) {
      Serial.println("Data uploaded successfully.");
    } else {
      Serial.print("Failed to upload data: ");
      Serial.println(firebaseData.errorReason());
    }
  }

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, maxTemperature);
  Blynk.virtualWrite(V3, maxHumidity);
  Blynk.virtualWrite(V4, minTemperature);
  Blynk.virtualWrite(V5, minHumidity);


