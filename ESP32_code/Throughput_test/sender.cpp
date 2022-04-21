/*
THROUGHPUT TEST - SENDER CODE
*/

#include <SPI.h>
#include <LoRa.h>
#include <esp_log.h>
using namespace std;

#define WAIT_BEFORE_TRANSMISSION 1000 // milliseconds

char* message_to_send = "===============================================================================================================================================================================================================================================================";
bool first_pass;

void setup() {
	Serial.begin(9600);
	while (!Serial);

	Serial.println("LoRa Sender");
	
	LoRa.setPins(15, 16, 22); // LoRa.setPins(ss, reset, dio0);

	if (!LoRa.begin(915E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
}

void loop() {
	// Send LoRa packet
	LoRa.beginPacket();
	LoRa.print(message_to_send);
	LoRa.endPacket();
}


