/*********REPLACE_WITH_YOUR_FIREBASE_DATABASE_URL
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <Adafruit_BMP280.h>

// Network and Firebase credentials
#define WIFI_SSID "Robotic WIFI"
#define WIFI_PASSWORD "rbtWIFI@2025"

#define Web_API_KEY ""
#define DATABASE_URL ""
#define USER_EMAIL ""
#define USER_PASS ""

Adafruit_BMP280 bmp;
// User function
void processData(AsyncResult &aResult);

// Authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Timer variables for sending data every 10 seconds
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds


void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  if (!bmp.begin(0x77)) { // Try 0x77 if this fails
    Serial.println("Could not find BMP280 sensor!");
    while (1);
  }
  
  // Configure SSL client
  ssl_client.setInsecure();
  ssl_client.setConnectionTimeout(1000);
  ssl_client.setHandshakeTimeout(5);
  
  // Initialize Firebase
  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void loop(){
  // Maintain authentication and async tasks
  app.loop();
  // Check if authentication is ready
  if (app.ready()){ 
    // Periodic data sending every 10 seconds
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval){
      // Update the last send time
      lastSendTime = currentTime;

      float temp = bmp.readTemperature();
      float pressure = bmp.readPressure();
      float altitude = bmp.readAltitude(1013.25); // You can adjust sea-level pressure
      Serial.print("Real Temperature: "); Serial.println(temp);
      Serial.print("Real Pressure: "); Serial.println(pressure);
      Serial.print("Real Altitude: "); Serial.println(altitude);

      // send a string
    //   stringValue = "value_" + String(currentTime);
    //   Database.set<String>(aClient, "/test/string", stringValue, processData, "RTDB_Send_String");
    //   // send an int
    //   Database.set<int>(aClient, "/test/int", intValue, processData, "RTDB_Send_Int");
    //   intValue++; //increment intValue in every loop

    //   // send a string
    //   floatValue = 0.01 + random (0,100);
      
      Database.set<float>(aClient, "/test/temperature", temp, processData, "RTDB_Send_Float");
      Database.set<float>(aClient, "/test/pressure", pressure, processData, "RTDB_Send_Float");
      Database.set<float>(aClient, "/test/altitude", altitude, processData, "RTDB_Send_Float");
    }
  }
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}