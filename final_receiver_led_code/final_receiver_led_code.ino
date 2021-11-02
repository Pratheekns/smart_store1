#include "ESP8266WiFi.h"
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

String cartID = "12345";
String skus[]= {"IPH1" , "IPH2" , "IPA1" , "MMO1" , "MKB1" , "ASW1" , "ATV1" , "AMA1" , "AAP1" , "AAP2"};
typedef struct {
  String skus_1;
  String title;
  int id;
  int mrp;
  int sp;
} langDictionary;
const langDictionary myDictionaryArr[] {
    {"IPH1","iphone", 1,250,245},
    {"IPH2","iphone", 1,250,245},
    {"IPA1","ipad", 2,1000,950},
    {"MMO1","Magic Mouse", 3,800,780},
    {"MKB1","Magic Keyboard", 4,350,335},
    {"ASW1","apple watch", 5,1000,950},
    {"ATV1","apple tv", 6,150,145},
    {"AMA1","Macbook", 7,445,430},
    { "AAP1","air pods", 8,1500,1450},
    {"AAP2","air pods", 8,1500,1450},
};
String cart[] = {"none","none","none","none","none","none","none","none","none","none"};
int m=10;
int n=0;
int redLED=15;//D8
int greenLED=4;//D2
String randomorder ="0";
char str[15];
String order="";
 int old_scanned;
   int scanned;
   int range;
#define FIREBASE_HOST "smart-retail-store-cb900-default-rtdb.asia-southeast1.firebasedatabase.app"//--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "qodymnKWObtZEuYJft0VTqxtNmk56AoqHQWC2a2q" //--> Your firebase database secret code.

#define WIFI_SSID "OP"
#define WIFI_PASSWORD "123456789"
 
#define SCAN_PERIOD 500
void setup(){
  Serial.begin(115200);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  setup_firebase();
  range = Firebase.getInt("carts/"+cartID+"/range");
  Serial.print(range);
  }
 

void setup_firebase(){
// connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
   
      Serial.println();
      Serial.print("cart ready for use ");
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
     
  digitalWrite(redLED, LOW);
 digitalWrite(greenLED, HIGH);
}

void addtocart(String skuid){
 
 
 if (!existingincart(skuid)){
  int uni =0;
   for (int j = 0; j < m; j++){
      if (cart[j] == "none"){
        uni = j;
        break;
        }
     }
     cart[uni]= skuid;
     Serial.println("added");
     Serial.println(skuid);
     if(WiFi.status() != WL_CONNECTED)
        setup_firebase();
      String temp = "";
      temp+="carts/"+cartID+"/orders/"+order+"/skus/"+skuid+"/receiver1";
       Firebase.setInt(temp, 1);
       Firebase.setString("carts/"+cartID+"/currentOrderKey", order);
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
     if(WiFi.status() != WL_CONNECTED)
        setup_firebase();
//     setup_firebase();
     String temp = "";
      temp+="carts/"+cartID+"/orders/"+order+"/skus/"+skuid+"/receiver1";
       Firebase.setInt(temp, 0);
       Firebase.setString("carts/"+cartID+"/currentOrderKey", order);
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
      if ((WiFi.SSID(i)== skus[j] )&& (WiFi.RSSI(i)>-range)){
          Serial.print(  WiFi.SSID(i));
          Serial.println(WiFi.RSSI(i));
          addtocart(skus[j]);
    }
      if ((WiFi.SSID(i)== skus[j] )&& (WiFi.RSSI(i)<-range)){
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
  temp+="carts/"+cartID+"/orders/"+order+"/skus/"+skus__;
  Firebase.setInt(temp+"/id",myDictionaryArr[v].id);
  Firebase.setInt(temp+"/sp",myDictionaryArr[v].sp);
  Firebase.setInt(temp+"/mrp",myDictionaryArr[v].mrp );
  Serial.println("firebase updated");
 }
  }return;
}

void LED(int scanned){
   //scanned = Firebase.getInt("carts/12345/scanned");
  if (scanned==0){//Ready to pick
    digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  Serial.println("SCANNED 0");
  }
  else
  if (scanned == 1){//Shopping in progress
    digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  Serial.println("SCANNED 1");
 
}else
  if (scanned == 2){//Paid up cart
    digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, HIGH);
  Serial.println("SCANNED 2");}
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

String getIncompleteOrderFromFB(String cartID){
  String path = "carts/"+cartID;
  String currentIncompleteOrder  = Firebase.getString(path+"/currentOrderKey");
  if(currentIncompleteOrder!="noOrder") return currentIncompleteOrder;
  else return GenTxt();
}
 
  
void loop(){
  if(WiFi.status() != WL_CONNECTED){
    setup_firebase();
  }
   scanned = Firebase.getInt("carts/"+cartID+"/scanned");
  Serial.println(scanned);
  if (scanned == 1){
    String key = getIncompleteOrderFromFB(cartID);
    if(key != "Not Found" ){
      order = key;
      }
    else{
    order = GenTxt();}

     scan_networks();
   }
   else if(scanned==2){
Firebase.setString("carts/"+cartID+"/currentOrderKey", "noOrder");} 
   LED(scanned);

 check();
   
}
