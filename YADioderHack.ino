/* Libraries */
#include <IRremote.h> //Library for IR recivers and LED for Arduino
/* END Libraries */
/* Variables */
//IRremote
const int RECEIVER_PIN = 11; //IR receiver pin
/* END Variables */
/* Configuration */
//IRremote
IRrecv irReceiver(RECEIVER_PIN); //IR receiver object
decode_results irData; //Define custom type for storing IR receiver data
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
		Serial.println(irData.value, HEX); //Print the value stored in irData treating it as HEX
		irReceiver.resume(); //Tell the IR receiver to resume receiving data
	}
	delay(100); //Wait 100ms not to flood the serial line
}