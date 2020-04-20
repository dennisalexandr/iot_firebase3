#include <WiFi.h>
#include <HCSR04.h>

#include "FirebaseESP32.h"

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID "asdasd"
#define WIFI_PASSWORD "asdasd"



#define led1 4
#define led2 15

#define ledB 23

#define button 19
#define ldr A0

int oldAdcLdr;
int newAdcLdr;

FirebaseData firebaseData;

String path = "/Node1";

int stateMotion = LOW;             // default tidak ada gerakan
int valMotion = 0;   

int buttonState =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);

  pinMode(ledB,OUTPUT);

  pinMode(ldr,INPUT);
  pinMode(button,INPUT);

  
  initWifi();
  oldAdcLdr = analogRead(ldr);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);


  //push button
  buttonState=digitalRead(button);
  Serial.println("status button: "+buttonState);
  if (buttonState == HIGH) {           // check if the sensor is HIGH
      digitalWrite(ledB,HIGH);
      Firebase.setInt(firebaseData, path + "/button", 1);
  } 
  else {  
      digitalWrite(ledB,LOW);  
      Firebase.setInt(firebaseData, path + "/button", 0);
  }


  //ldr
  newAdcLdr = analogRead(ldr);
  Serial.println("asd"+newAdcLdr);
  if(newAdcLdr != oldAdcLdr){
    Firebase.setDouble(firebaseData, path + "/ldr", newAdcLdr);
    oldAdcLdr = newAdcLdr;
  }


  
  
  //get value /Node1/lampu1
  if(Firebase.getInt(firebaseData, path + "/lampu1")){
    /*Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.print("VALUE: ");
    Serial.println(firebaseData.intData());
    */
    if(firebaseData.intData() == 0)
      digitalWrite(led1,0);
    else
      digitalWrite(led1,1);    
  }
 

  //get value /Node1/lampu2
  if(Firebase.getInt(firebaseData, path + "/lampu2")){
    /*Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.print("VALUE: ");
    Serial.println(firebaseData.intData());
    */
    if(firebaseData.intData() == 0)
      digitalWrite(led2,0);
    else
      digitalWrite(led2,1);
  }


   
}

void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
