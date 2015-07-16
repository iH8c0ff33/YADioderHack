/* Libraries */
#include <IRremote.h> //Library for IR recivers and LED for Arduino
/* END Libraries */
/* Variables */
//Header
#include "IRcodes.h" //IR codes for keys
//IRremote
const int RECEIVER_PIN = 11;
/* END Variables */
/* Configuration */
//IRremote
IRrecv irReceiver(RECEIVER_PIN);
decode_results irData;
/* END Configuration */
/* Arduino Setup */
void setup() {
	Serial.begin(9600); //Enstabilishes a serial communication at 9600bps
	irReceiver.enableIRIn(); //Enables IR input on receiver
}
/* END Arduino Setup */
/* Arduino Loop */
void loop() {
	if (irReceiver.decode(&irData)) { //If the receiver succeeds in decoding data (address of variable where to store the received data)
		Serial.println(irData.value, HEX);
		if (irData.value == POWER) {
			Serial.println("POWER");
		}
		irReceiver.resume();
	}
	delay(100);