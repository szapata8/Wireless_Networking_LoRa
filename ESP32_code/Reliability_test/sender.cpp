/*
RELIABILITY TEST - SENDER CODE
*/

#include <SPI.h>
#include <LoRa.h>
#include <messages.h>
#include <esp_log.h>
using namespace std;

#define WAIT_BEFORE_TRANSMISSION 1000 // miliseconds
#define WAIT_BETWEEN_TRANSMISSION 2000 // miliseconds
// #define TRANSMISSION_TIME 10000 // miliseconds
// #define MAX_PACKET_LENGTH_BYTES 255


// char* message_to_send = message_3;
// char* packet_data;
bool first_loop_iteration;
int ms_at_setup;
int current_transmission_time;

char* substr(char* arr, int begin, int len) {
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

void setup() {
	Serial.begin(9600);
	while (!Serial);

	Serial.println("LoRa Sender");
	
	LoRa.setPins(15, 16, 22); // LoRa.setPins(ss, reset, dio0);

	if (!LoRa.begin(915E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
	LoRa.beginPacket();
	LoRa.print(START_OF_TRANSMISSION_FRAME);
	LoRa.endPacket();
	delay(WAIT_BEFORE_TRANSMISSION); // Wait WAIT_BEFORE_TRANSMISSION milliseconds before starting to transmit -> allows the receiver to prepare to record data
	ms_at_setup = esp_log_timestamp();
	Serial.print("ms at end of setup: ");
	Serial.println(ms_at_setup);
	first_loop_iteration = true;
}

void loop() {
	/*
	// // Extract key information about the message to send
	// int char_amount = strlen(message_to_send);
	// int fragments = char_amount / MAX_PACKET_LENGTH_BYTES;
	// int remainder = char_amount % MAX_PACKET_LENGTH_BYTES;
	// int lower_index = 0;

	// // Break message into packets of size MAX_PACKET_LENGTH_BYTES
	// for (int fragment = 0; fragment <= fragments; fragment++) {
	// 	lower_index = MAX_PACKET_LENGTH_BYTES * fragment;

		
	// 	// Check if transmission has been going for longer than TRANSMISSION_TIME
	// 	current_transmission_time = esp_log_timestamp() - ms_at_setup;
	// 	if (current_transmission_time > TRANSMISSION_TIME){
	// 		// Send END_OF_TRANSMISSION_FRAME
	// 		Serial.println("TRANSMISSION TIME COMPLETED!");
	// 		LoRa.beginPacket();
	// 		LoRa.print(END_OF_TRANSMISSION_FRAME);
	// 		LoRa.endPacket();
	// 		delay(1000); // Send the END_OF_TRANSMISSION_FRAME every 1 second
	// 		return;
	// 	}

	// 	// If it's the last packet, fill it up with padding to still be of size MAX_PACKET_LENGTH_BYTES
	// 	if (fragment == fragments){
	// 		int padding_length = MAX_PACKET_LENGTH_BYTES - remainder;
	// 		char padding[padding_length];
	// 		memset(padding, '-', padding_length); // Create a padding for the packet to be 255 bytes total
	// 		padding[padding_length] = '\0';
	// 		packet_data = substr(message_to_send, lower_index, remainder); // Grab last chunk of data

	// 		char last_packet[MAX_PACKET_LENGTH_BYTES] = "";
	// 		strcpy(last_packet, packet_data);
	// 		strcat(last_packet, padding);
	// 		packet_data = last_packet;
	// 	}
	// 	// The packet takes up all 255 bytes with data
	// 	else {
	// 		packet_data = substr(message_to_send, lower_index, MAX_PACKET_LENGTH_BYTES);
	// 	}

		// Print what's being sent to the serial port
		Serial.print("Fragment #");
		Serial.print(fragment);
		Serial.println();
		Serial.println(packet_data);

		// // Send LoRa packet
		LoRa.beginPacket();
		LoRa.print(packet_data);
		LoRa.endPacket();
		delay(95);
	}
	*/
	if (first_loop_iteration){
		// Wait WAIT_BETWEEN_TRANSMISSION
		while ( (esp_log_timestamp() - ms_at_setup) < WAIT_BETWEEN_TRANSMISSION){;}

		// Send message_1
		LoRa.beginPacket();
		LoRa.print(message_1);
		LoRa.endPacket();
		Serial.println("message_1 sent");

		// Wait WAIT_BETWEEN_TRANSMISSION
		while ( (esp_log_timestamp() - ms_at_setup) < (2 * WAIT_BETWEEN_TRANSMISSION) ){;}
		
		// Send message_2
		LoRa.beginPacket();
		LoRa.print(message_2);
		LoRa.endPacket();
		Serial.println("message_2 sent");

		// Wait WAIT_BETWEEN_TRANSMISSION
		while ( (esp_log_timestamp() - ms_at_setup) < (3 * WAIT_BETWEEN_TRANSMISSION) ){;}
		
		// Send message_3
		LoRa.beginPacket();
		LoRa.print(message_3);
		LoRa.endPacket();
		Serial.println("message_3 sent");

		// Wait WAIT_BETWEEN_TRANSMISSION
		while ( (esp_log_timestamp() - ms_at_setup) < (4 * WAIT_BETWEEN_TRANSMISSION) ){;}
		
		// Send message_4
		LoRa.beginPacket();
		LoRa.print(message_4);
		LoRa.endPacket();
		Serial.println("message_4 sent");

		// Wait WAIT_BETWEEN_TRANSMISSION
		while ( (esp_log_timestamp() - ms_at_setup) < (5 * WAIT_BETWEEN_TRANSMISSION) ){;}
		
		// Send message_5
		LoRa.beginPacket();
		LoRa.print(message_5);
		LoRa.endPacket();
		Serial.println("message_5 sent");

		first_loop_iteration = false; // Code executes only once and then board needs to be reset to run another round of transmissions
	}
	delay(1000);
}


