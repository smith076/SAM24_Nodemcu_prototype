//GPS+Firebase+NodeMCU+rfid
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN D4
#define RST_PIN D3
#define new1 1
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#include <Firebase.h>  
#include <FirebaseArduino.h>  
#include <FirebaseCloudMessaging.h>  
#include <FirebaseError.h>  
#include <FirebaseHttpClient.h>  
#include <FirebaseObject.h>  
#include <TinyGPS++.h>
#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>  
#include <FirebaseArduino.h>  
  
//CREDENTIALS FIREBASE SAMAN  
#define FIREBASE_HOST "speed-analysing-mechanism-24.firebaseio.com"  
#define FIREBASE_AUTH "M2Pru64VI2zffsxzSK7W0TuPEoSLvnu9tRpTUDxy"  
#define WIFI_SSID "Lucifers JioFi4_0246E2"  
#define WIFI_PASSWORD "IAMSHALVIAN1"  
  
SoftwareSerial gpsSerial(4,5);    //RX-D1 TX-D2
TinyGPSPlus gps;    //gps object
void rfidsaman(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  //String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i]);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i],HEX));
  }
  
  content.toUpperCase();
  Firebase.setString("RFID",content);
  Serial.println(); 
 // delay(1000);
}
void setup() {
  Serial.begin(115200);   //begins serial com
  gpsSerial.begin(115200);    //Begins gps
  Serial.println("STARTED");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  //begins wifi and connects 
  Serial.print("connecting");  
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");  
    delay(500);  
  }
  Serial.println();  
  Serial.print("connected: ");  
  Serial.println(WiFi.localIP()); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  //begins firebase
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  rfidsaman();
  
}

void loop() {
  
  if (Firebase.failed()) {  
      Serial.print("setting /number failed:");  
      Serial.println(Firebase.error());    
      return;  
  } 
  
 
  while(gpsSerial.available()){
    if(gps.encode(gpsSerial.read()))// encode gps data 
    {  
    float latitude=(gps.location.lat());
    float longitude=(gps.location.lng());
    Serial.print("LAT: ");
    Serial.println(latitude,6);
    Serial.print("LON: ");
    Serial.println(longitude,6);
    Firebase.setFloat("gps/lat",latitude);
    Firebase.setFloat("gps/lon",longitude);
    }
    
}}
