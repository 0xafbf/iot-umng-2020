

/*
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt

   Copyright (c) 2020 mobizt

   This example is for FirebaseESP8266 Arduino library v 2.8.9 or later

*/

//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://iot-2020umng.firebaseio.com/"
#define FIREBASE_AUTH "TPAksSAwOnNnGAD445el8lQDGpglK7Ntr204QOtu"

#define WIFI_SSID "BoteroAguilar"
#define WIFI_PASSWORD "ejecutivo2020"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

FirebaseJson json;
String path = "/Goldberg";

void printResult(FirebaseData &data);

char moduleToListen = 'A';
char moduleToWrite = 'B';

void setup()
{

  Serial.begin(9600);

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

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  /*
    This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
    Firewall that allows only GET and POST requests.

    Firebase.enableClassicRequest(firebaseData, true);
  */

  /*
  for (char car = 'A'; car <= 'D'; car++) {
    if (Firebase.setInt(firebaseData, path + "/modulo" + car, 0)) { //Path, and value to write. 
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.print("VALUE: ");
      Serial.println("------------------------------------");
      Serial.println();
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  }
  printResult(firebaseData);
  */
}

void printResult(FirebaseData &data)
{
  for (char car = 'A'; car <= 'D'; car++) {
    Serial.print("Módulo ");
    Serial.print(car);
    if (Firebase.getInt(data, path + "/modulo" + car) ) {
      if (firebaseData.dataType() == "int") {
        int dataO = (firebaseData.intData()); //Read value
        Serial.println(dataO);
      }
    }
  }
}

void do_main_action() {
	Serial.print("CHANGED, doing stuff");
	for (int idx = 0; idx < 10; ++idx) {
		delay(500);
		Serial.print('.');
	}
	
	Serial.println("\nWriting firebase");

	String writePath = path + "/modulo" + moduleToWrite;
	if (Firebase.setInt(firebaseData, writePath, 1)) {
		Serial.println("written");
	}
}

int prevResult = -1;
void loop()
{
	delay(2000);
	String readPath = path + "/modulo" + moduleToListen;
	int result = -1;
	Serial.println("reading");
	if (Firebase.getInt(firebaseData, readPath, result)) {
		Serial.printf("got %d\n", result);
		if (result == 1 && prevResult == 0) {
			do_main_action();
		}
		prevResult = result;
	}
}
