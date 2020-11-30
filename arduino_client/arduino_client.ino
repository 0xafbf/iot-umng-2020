#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String api_key = "7IK9TWDYE2FXC9JG";
const char* ssid = "BoteroAguilar";
const char* pass = "ejecutivo2020";

const char* server = "api.thingspeak.com";

#define READ A0


void setup() {
	Serial.begin(9600);
	delay(10);
	Serial.print("Connecting ");
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("\nConnected");
}

#define NUM_MEASUREMENTS 10
#define TIME_BETWEEN_MEASUREMENTS 500

int samples[NUM_MEASUREMENTS];

void loop() {
	// wait 5 seconds before trying
	int total = 0;
	bool shakes = false;
	for (int it = 0; it < NUM_MEASUREMENTS; ++it) {
		delay(TIME_BETWEEN_MEASUREMENTS);
		int input = analogRead(READ);
		samples[it] = input;
		total += input;
		if (input < 100 || input > 900) {
			shakes = true;
			Serial.println("Shakes!");
		}
	}

	total /= NUM_MEASUREMENTS;
	
	WiFiClient client;
	HTTPClient http;
	String prefix = "http://api.thingspeak.com/update?api_key=WBC391M34BG4I612&field1=";
		
	Serial.println(total);
	prefix += total;
	Serial.print("sending: ");
	Serial.println(prefix);
	bool result = http.begin(client, prefix);
	if (!result) {
		return;
	}
	int httpCode = http.GET();
	Serial.printf("HTTP: %d\n", httpCode);
	Serial.println(http.getString());
	http.end();

	if (shakes) {
		char* path = "http://api.thingspeak.com/apps/thingtweet/1/statuses/update";
		String payload = "api_key=7IK9TWDYE2FXC9JG";
		payload += "&status=This message was sent from an ESP8266 device";
		HTTPClient http2;
		http2.begin(client, path);
		int tweetResult = http2.POST(payload);
		Serial.printf("Tweet: %d\n", tweetResult);
		Serial.println(http2.getString());
		http2.end();
	}
}



