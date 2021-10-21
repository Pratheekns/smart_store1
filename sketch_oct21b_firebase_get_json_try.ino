#include "ESP8266WiFi.h"
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#define FIREBASE_HOST "smart-retail-store-cb900-default-rtdb.asia-southeast1.firebasedatabase.app"//--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "qodymnKWObtZEuYJft0VTqxtNmk56AoqHQWC2a2q" //--> Your firebase database secret code.

#define WIFI_SSID "OP"
#define WIFI_PASSWORD "123456789"


void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
   // Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("connected: ");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {
Serial.println("Incomplete Order key-->" + getIncompleteOrderFromFB("12345"));
}

String getIncompleteOrderFromFB(String cartID){
  String path = "carts/"+cartID;
JsonVariant variant = Firebase.get(path).getJsonVariant("orders"); 
JsonObject& obj = variant.as<JsonObject>();
String reqKey = "Not Found";
for (auto kv : obj) {
    JsonVariant reqVariant = obj.get<JsonVariant>(kv.key);
    JsonObject& reqObj = reqVariant.as<JsonObject>();
    if(!reqObj.containsKey("orderID")){
      reqKey = kv.key;
      }
   }
   return reqKey;
}
