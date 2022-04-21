/*
THROUGHPUT TEST - RECEIVER CODE
*/

#include <iostream>
#include <string.h>
#include <SPI.h>
#include <LoRa.h>
#include <esp_log.h>
using namespace std;

# define RSSI_LENGTH 5 // bytes
# define BYTES_LENGTH 4 // bytes

bool transmission_happening;

int get_rssi_padding_slots(int rssi) {
	// Returns the padding slots available after writing rssi to have a standard length in the packet for 
	// displaying rssi
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
}

int get_bytes_padding_slots(int bytes_received) {
	// Returns the padding slots available after writing amount of bytes to have a standard length in the packet for 
	// displaying amount of bytes received
	if (bytes_received > 99) {
		return BYTES_LENGTH - 3; // Amount of bytes needs 3 digits (100-255)
	}
	if (bytes_received > 9) {
		return BYTES_LENGTH - 2; // Amount of bytes needs 2 digits (10-99)
	}
	return BYTES_LENGTH - 1; // Amount of bytes needs 1 digit (1-9)
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
		int rssi_padding_slots = get_rssi_padding_slots(rssi);

		Serial.print("@");
		Serial.print(rssi);
		for (int i=1; i<rssi_padding_slots; i++) {
			Serial.print("=");
		}
		Serial.print("@");

		// print amount of bytes received
		int bytes_padding_slots = get_bytes_padding_slots(packetSize);
		Serial.print(packetSize);
		for (int i=1; i<bytes_padding_slots; i++) {
			Serial.print("=");
		}
	}
}

