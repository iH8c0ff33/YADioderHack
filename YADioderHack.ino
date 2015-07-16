#include <IRremote.h> //Library for IR recivers and LED for Arduino

#include "IRcodes.h" //IR codes for keys

const uint8_t RECEIVER_PIN = 12;
const uint8_t ledPin = 13;
int pressedButton;
bool ledOn = false;
struct strip {
	const uint8_t REDPIN;
	const uint8_t GREENPIN;
	const uint8_t BLUEPIN;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};
strip strip1 = {3, 5, 6, 0, 0, 0};
strip strip2 = {9, 10, 11, 0, 0, 0};
struct blink_t {
	bool active;
	unsigned long time;
	int duration;
};
blink_t blinkVar = {0, 0, 0};

IRrecv irReceiver(RECEIVER_PIN);
decode_results irData;

void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	irReceiver.enableIRIn();
}

void loop() {
	readIR();
	checkButton();
	blinkFunction();
	delay(100);
}

void readIR() { //Reads IR signals and stores received values
	if (irReceiver.decode(&irData)) { //If the IR receiver has received data
		pressedButton = irData.value;
		irReceiver.resume();
	} else {
		pressedButton = NULL;
	}
}

void checkButton() { //Checks the pressed button and do actions
	switch (pressedButton) {
	    case BTNPOWER:
	      Serial.println("power");
	      break;
	    case BTNRED:
	      Serial.println("red");
	      blinkFor(1000);
	      break;
	    case BTNGREEN:
	      break;
	    case BTNBLUE:
	      break;
	    case BTN1:
	      break;
	    case BTN2:
	      break;
	    case BTN3:
	      break;
	    case BTN4:
	      break;
	    case BTN5:
	      break;
	    case BTN6:
	      break;
	    case BTN7:
	      break;
	    case BTN8:
	      break;
	    case BTN9:
	      break;
	    case BTN0:
	      break;
	}
}

void blinkFunction() { //Turns on an LED and turns it off after duration
	if (blinkVar.active) {
		if (millis() < blinkVar.time + blinkVar.duration && !ledOn) { //Turns LED on if it is off
			Serial.println("led on");
			digitalWrite(ledPin, HIGH);
			ledOn = true;
			blinkVar.time = millis();
		} else if (millis() > blinkVar.time + blinkVar.duration && ledOn) { //If duration is passed (and if the LED is on) turns the LED off
			Serial.println("led off");
			digitalWrite(ledPin, LOW);
			ledOn = false;
			blinkVar.active = false;
		}
	}
}

void blinkFor(int duration) { //Sets the variables for turning on the LED
	blinkVar.active = true;
	blinkVar.duration = duration;
	blinkVar.time = millis();
}