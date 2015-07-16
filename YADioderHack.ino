#include <IRremote.h> //Library for IR recivers and LED for Arduino
#include "IRcodes.h" //IR codes for keys
const int RECEIVER_PIN = 11;
IRrecv irReceiver(RECEIVER_PIN);
decode_results irData;

void setup() {
	Serial.begin(9600); //Enstabilishes a serial communication at 9600bps
	irReceiver.enableIRIn(); //Enables IR input on receiver
}

void loop() {
	if (irReceiver.decode(&irData)) { //If the receiver succeeds in decoding data (address of variable where to store the received data)
		Serial.println(irData.value, HEX);
		if (irData.value == POWER) {
			Serial.println("POWER");
		}
		irReceiver.resume();
	}
	delay(100);