#include "ESP8266WiFi.h"
#include <FirebaseArduino.h>


String skus[]= {"ab" , "hb" , "hs" , "iuw" , "iw" , "uays" , "uhh" , "us" , "uyd" , "yn"};
typedef struct { 
  String skus_1;
  char* title;
  int id;
  int mrp;
  int sp;
} langDictionary;
const langDictionary myDictionaryArr[] {
    {"ab", "Magic Mouse", 1,250,245},
    {"hb", "Magic Mouse", 1,250,245},
    {"hs", "iphone", 2,1000,950},
    {"iuw", "ipad", 3,800,780},
    {"iw", "apple watch", 4,350,335},
    {"uays", "apple tv", 5,1000,950},
    {"uhh", "air tag", 6,150,145},
    {"us", "air pods", 7,445,430},
    { "uyd", "Mac", 8,1500,1450},
    {"yn", "Mac", 8,1500,1450},
};
String cart[] = {"none","none","none","none","none","none","none","none","none","none"};
int m=10;
#define FIREBASE_HOST "smart-retail-store-cb900-default-rtdb.asia-southeast1.firebasedatabase.app"//--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "qodymnKWObtZEuYJft0VTqxtNmk56AoqHQWC2a2q" //--> Your firebase database secret code.

#define WIFI_SSID "OP"
#define WIFI_PASSWORD "123456789"
 
#define SCAN_PERIOD 500
void setup()
 {
  Serial.begin(115200);


  
}
void set_firebase(){
// connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("connected: ");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void addtocart(String skuid){
 
 
 if (!existingincart(skuid)){
  int uni =0;
   for (int j = 0; j < m; j++){
      if (cart[j] == "none"){
        uni = j;
        break;
        }
     }cart[uni]= skuid;
     Serial.println("added");
     Serial.println(skuid);
     set_firebase();
      String temp = "";
      temp+="carts/42424/skus/"+skuid+"/reciever1";
       Firebase.setInt(temp, 1);
      Serial.println("SET RECEIVER AS 1");
  
  }
     
}

void deletetocart(String skuid){
 
  if (existingincart(skuid)){
   for (int j = 0; j < m; j++){
      if (cart[j] == skuid){
        cart[j]= "none";
        }
     }
     Serial.println("deleted");
     Serial.println(skuid);
     set_firebase();
     String temp = "";
      temp+="carts/42424/skus/"+skuid+"/reciever1";
       Firebase.setInt(temp, 0);
       Serial.println("SET RECEIVER AS 0");
     
}
}

bool existingincart(String sku){
  bool existing = false;
  for (int i = 0; i < m; i++){
    if (cart[i]==sku){
      Serial.println("existing");
      Serial.println(sku);
     existing = true;
     
    }
  }return existing;
}
  

void scan_networks(){
  WiFi.scanNetworks();
    delay(100);
  int n = WiFi.scanComplete();
  if(n >= 0)
  {
    Serial.printf("%d network(s) found\n", n);
    for (int i = 0; i < n; i++)
    {
      for(int j = 0; j < 10; j++){
      if ((WiFi.SSID(i)== skus[j] )&& (WiFi.RSSI(i)>-40)){
          Serial.print(  WiFi.SSID(i));
          Serial.println(WiFi.RSSI(i));
          addtocart(skus[j]);
    }
      if ((WiFi.SSID(i)== skus[j] )&& (WiFi.RSSI(i)<-40)){
          Serial.print( WiFi.SSID(i));
          Serial.println(WiFi.RSSI(i));
          deletetocart(skus[j]);
    }
   
  } 
}WiFi.scanDelete();
}
}

void check(){
  for (int k = 0; k < 10; k++)  {
    String skus__="";
    if (cart[k]!="none"){
      skus__= cart[k];
     int v;
    for(int i = 0; i < 10; i++) {
      if (skus__ == myDictionaryArr[i].skus_1){
       v=i;
    }
  }
  String temp = "";
  temp+="carts/42424/skus/"+skus__;
  //set_firebase();
  Firebase.setInt(temp+"/id",myDictionaryArr[v].id);
  Firebase.setInt(temp+"/sp",myDictionaryArr[v].sp);
  Firebase.setInt(temp+"/mrp",myDictionaryArr[v].mrp );
  Serial.println("firebase updated");
 }
  }return;
}



void loop()
{
   scan_networks();
   for (int z = 0; z < m; z++){
    Serial.print(cart[z]);
   }
    Serial.println("");
   check();
    
}


  
