/*
RELIABILITY TEST - RECEIVER CODE
*/

#include <iostream>
#include <string.h>
#include <SPI.h>
#include <LoRa.h>
#include <esp_log.h>
using namespace std;

# define RSSI_LENGTH 5 // bytes

bool transmission_happening;

int rssi_padding_slots(int rssi) {
	// Returns the padding slots available after writing rssi to have a standard length in the packet for 
	// displaying rssi (python always reads 255 bytes at a time form serial port)
	if (rssi > 0) {
		return RSSI_LENGTH - 1;

	}
	else {
		// rssi is negative
		if (rssi > -10) {
			return RSSI_LENGTH - 2; // To display numbers from -1 to -9
		}
		if (rssi > -100) {
			return RSSI_LENGTH - 3; // To display numbers from -10 to -99
		}
		return RSSI_LENGTH - 4; // To display numbers from -100 to below (they will use 4 bytes, one for each character)
	}
		int padding_length = RSSI_LENGTH;
		char padding[padding_length];
		memset(padding, '-', padding_length); // Create a padding for the packet to be 255 bytes total
		padding[padding_length] = '\0';
}

void setup() {
	Serial.begin(9600);
	while (!Serial);

	Serial.println("---> ESP32 LoRa Receiver initalized <---");

	LoRa.setPins(15, 16, 22); // LoRa.setPins(ss, reset, dio0);

	if (!LoRa.begin(915E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
	transmission_happening = false;
}

void loop() {
	// try to parse packet
	int packetSize = LoRa.parsePacket(); // Returns amount of bytes in the buffer of the LoRa module
	if (packetSize) {
		// print RSSI of packet
		int rssi = LoRa.packetRssi();

		int padding_slots = rssi_padding_slots(rssi);

		Serial.print("@");
		Serial.print(rssi);
		for (int i=1; i<padding_slots; i++) {
			Serial.print("=");
		}
		Serial.print("@");

		// read packet
		while (LoRa.available()) {
			// Serial.print((char)LoRa.read());
			Serial.print((char)LoRa.read());
		}
	}
}

