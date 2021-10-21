#include "ESP8266WiFi.h"
#include <FirebaseArduino.h>
#include <ArduinoJson.h>


String skus[]= {"ab" , "hb" , "hs" , "iuw" , "iw" , "uays" , "uhh" , "us" , "uyd" , "yn"};
typedef struct { 
  String skus_1;
  int id;
  int mrp;
  int sp;
} langDictionary;
const langDictionary myDictionaryArr[] {
    {"ab", 1,250,245},
    {"hb", 1,250,245},
    {"hs", 2,1000,950},
    {"iuw", 3,800,780},
    {"iw", 4,350,335},
    {"uays", 5,1000,950},
    {"uhh", 6,150,145},
    {"us", 7,445,430},
    { "uyd", 8,1500,1450},
    {"yn", 8,1500,1450},
};
String cart[] = {"none","none","none","none","none","none","none","none","none","none"};
int m=10;
int n=0;
int led1=15;
int led2=4;
String randomorder ="0";
char str[15];
String order="";
 int old_scanned;
#define FIREBASE_HOST "smart-retail-store-cb900-default-rtdb.asia-southeast1.firebasedatabase.app"//--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "qodymnKWObtZEuYJft0VTqxtNmk56AoqHQWC2a2q" //--> Your firebase database secret code.

#define WIFI_SSID "OP"
#define WIFI_PASSWORD "123456789"
 
#define SCAN_PERIOD 500
void setup()
 {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  
if( setup_firebase()==1){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  }
  else{ digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}
 }
void set_firebase(){
// connect to wifi.
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

int setup_firebase(){
// connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
    n=0;
  }
  Serial.println();
  Serial.print("connected: ");
  n=1;
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  return n;
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
      temp+="carts/123995/orders/"+order+"/skus/"+skuid+"/receiver1";
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
      temp+="carts/123995/orders/"+order+"/skus/"+skuid+"/receiver1";
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
  temp+="carts/123995/orders/"+order+"/skus/"+skus__;
  Firebase.setInt(temp+"/id",myDictionaryArr[v].id);
  Firebase.setInt(temp+"/sp",myDictionaryArr[v].sp);
  Firebase.setInt(temp+"/mrp",myDictionaryArr[v].mrp );
  Serial.println("firebase updated");
 }
  }return;
}

void LED(){
  int scanned = Firebase.getInt("carts/123995/scanned");
  if (scanned==0){
    digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  }
  if (scanned == 1){digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  
}
  if (scanned == 2){digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);}
}
String GenTxt(){

  int generated=0;
  
  
memset(str, '\0', sizeof(str));

uint8_t cnt = 0;
  while (cnt != sizeof(str) - 1)
  {
    str[cnt] = random(0, 0x7F);
    if (str[cnt] == 0)
    {
      break;
    }
    if (isAlphaNumeric(str[cnt]) == true)
    {
      cnt++;
    }
    else
    {
      str[cnt] = '\0';
    }
}
randomorder=str;
return randomorder;

}

bool search_firebase(String id);
{

void loop()
{ set_firebase();
  int scanned = Firebase.getInt("carts/123995/scanned");
  if (scanned == 1){
    if(search_firebase){}
    else{
    order = GenTxt();} 
    // create set of firebase
     
  } 

if (scanned == 1){
  scan_networks();
   }
   LED();
 for (int z = 0; z < m; z++){
  Serial.print(cart[z]);
 }
// 
//   Serial.println("");

 check();
    
}


  
