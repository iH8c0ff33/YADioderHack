#include <IRremote.h> //Library for IR recivers and LED for Arduino

#include "IRcodes.h" //IR codes for keys

const int RECEIVER_PIN = 11;
unsigned long now = 0;

IRrecv irReceiver(RECEIVER_PIN);
decode_results irData;

void setup() {
	Serial.begin(9600); //Enstabilishes a serial communication at 9600bps
	irReceiver.enableIRIn(); //Enables IR input on receiver
}

void loop() {
	now = millis();
	readIR();
}

void readIR() { //Reads IR signals and stores received values
	if (irReceiver.decode(&irData)) { //If the IR receiver has received data
		pressedButton = irData.value;
		irReceiver.resume();
	} else {
		pressedButton = NULL;
	}
}