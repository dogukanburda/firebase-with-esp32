#include <WiFi.h>
#include "FirebaseESP32.h"


#define FIREBASE_HOST "https://nosqldata-43a40-default-rtdb.firebaseio.com/" //Change to your Firebase RTDB project ID e.g. Your_Project_ID.firebaseio.com
#define FIREBASE_AUTH "**************" //Change to your Firebase RTDB secret password
#define WIFI_SSID "*****"
#define WIFI_PASSWORD "****"


//Define Firebase Data object
FirebaseData fbdo;
FirebaseData fbdo2;

const int ledPin =  19; //GPIO19 for LED
String path = "/";
String node = "node1";
unsigned long eskiZaman=0;
unsigned long yeniZaman;

//Global function that handles stream data
void streamCallback(StreamData data) // Stream's value
{

  if (data.dataType() == "int") {
    digitalWrite(ledPin, data.intData());
  }


}


void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    Serial.println();
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }
}


void setup()
{

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  Serial.println();
  Serial.println();

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



  if (!Firebase.beginStream(fbdo, path + "HV" +"/"+ node)) // to subscribe to the stream changes at a defined database path
  {
    Serial.println("Could not begin stream");
    Serial.println("REASON: " + fbdo.errorReason());
    Serial.println();
  }

  Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback); //streamCallback is doer

}

void loop()
{
  yeniZaman = millis();
  
  if(yeniZaman-eskiZaman > 1000){
    
    int adc = analogRead(36);  //Reads pin 36
    if (Firebase.pushInt(fbdo2, path +"ADC/"+node, adc)) {
      Serial.println("pushed ADC value is "+adc);
    } else {
      Serial.println("Failed because; REASON: " + fbdo2.errorReason());
    }
    
    eskiZaman = yeniZaman;

}
}
