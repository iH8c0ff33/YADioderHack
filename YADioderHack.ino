// #include <IRremote.h> //Library for IR recivers and LED for Arduino
#include "IRLremote.h"

#include "IRcodes.h" //IR codes for keys

// const uint8_t RECEIVER_PIN = 12;
const uint8_t ledPin = 13;
#define IRL_BLOCKING true
#define pinIR 2
int pressedButton;
uint8_t currentStrip = 1;
uint8_t currentColor = 0;
uint8_t currentDigit = 0;
uint8_t digits[3] = {0, 0, 0};
uint8_t color[3] = {0, 0, 0};
bool ledOn = false;
bool modified = false;
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
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;
uint32_t lastCommand = 0;
unsigned long receivedTime = 0;

// IRrecv irReceiver(RECEIVER_PIN);
// decode_results irData;

void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	pinMode(strip1.REDPIN, OUTPUT);
	pinMode(strip1.GREENPIN, OUTPUT);
	pinMode(strip1.BLUEPIN, OUTPUT);
	pinMode(strip2.REDPIN, OUTPUT);
	pinMode(strip2.GREENPIN, OUTPUT);
	pinMode(strip2.BLUEPIN, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_ALL>, CHANGE);
	// irReceiver.enableIRIn();
}

void loop() {
	uint8_t oldSREG = SREG;
	cli();
	// readIR();
	checkButton();
	blinkFunction();
	SREG = oldSREG;
}

// void readIR() { //Reads IR signals and stores received values
// 	if (irReceiver.decode(&irData)) { //If the IR receiver has received data
// 		pressedButton = irData.value;
// 		irReceiver.resume();
// 	} else {
// 		pressedButton = NULL;
// 	}
// }
void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  if (IRL_BLOCKING && !IRProtocol) {
    // update the values to the newest valid input
    IRProtocol = protocol;
    IRAddress = address;
    IRCommand = command;
  }
}

void checkButton() { //Checks the pressed button and do actions
	if (IRProtocol) {
		if (millis() > receivedTime + 50) {
			Serial.println(IRCommand);
			switch (IRCommand) {
			    case BTNPOWER:
			      Serial.println("power");
			      if (modified) {
			      	applyColor();
			      }
			      currentDigit = 0;
			      if (currentStrip == 1) {
			      	currentStrip = 2;
			      	blinkFor(250);
			      } else {
			      	currentStrip = 1;
			      	blinkFor(100);
			      }
			      break;
			    case BTNRED:
			      Serial.println("red");
			      currentColor = 0;
			      blinkFor(100);
			      break;
			    case BTNGREEN:
			      currentColor = 1;
			      blinkFor(100);
			      break;
			    case BTNBLUE:
			      currentColor = 2;
			      blinkFor(100);
			      break;
			    case BTN1:
			      insert(1);
			      break;
			    case BTN2:
			      insert(2);
			      break;
			    case BTN3:
			      insert(3);
			      break;
			    case BTN4:
			      insert(4);
			      break;
			    case BTN5:
			      insert(5);
			      break;
			    case BTN6:
			      insert(6);
			      break;
			    case BTN7:
			      insert(7);
			      break;
			    case BTN8:
			      insert(8);
			      break;
			    case BTN9:
			      insert(9);
			      break;
			    case BTN0:
			      insert(0);
			      break;
			}
			receivedTime = millis();
		}
		IRProtocol = 0;
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

void insert(uint8_t number) { //Inserts the pressed button number into the current digit and if there are 3 digits stores the color
	blinkFor(100);
	if (currentDigit < 3) {
		digits[currentDigit] = number;
		currentDigit++;
	}
	if (currentDigit == 3) {
		writeColor();
		currentDigit = 0;
	}
}

void writeColor() { //Writes all the digits into one single color 0,255
	if (digits[0] * 100 + digits[1] * 10 + digits[2] < 256) {
		color[currentColor] = digits[0] * 100 + digits[1] * 10 + digits[2];
		blinkFor(250);
		Serial.println(color[currentColor]);
		modified = true;
	} else {
		blinkFor(1000);
	}
}

void applyColor() { //Writes components into the strip
	Serial.print("R: ");
	Serial.print(color[0]);
	Serial.print("\nG: ");
	Serial.print(color[1]);
	Serial.print("\nB: ");
	Serial.print(color[2]);
	Serial.print("\n");
	if (currentStrip == 1) {
		strip1.red = color[0];
		strip1.green = color[1];
		Serial.println(strip1.green);
		strip1.blue = color[2];
	} else {
		strip2.red = color[0];
		strip2.green = color[1];
		strip2.blue = color[2];
	}
	refresh();
	modified = false;
}

void refresh() { //Apply the selected colors to all the strips
	analogWrite(strip1.REDPIN, strip1.red);
	analogWrite(strip1.GREENPIN, strip1.green);
	analogWrite(strip1.BLUEPIN, strip1.blue);
	analogWrite(strip2.REDPIN, strip2.red);
	analogWrite(strip2.GREENPIN, strip2.green);
	analogWrite(strip2.BLUEPIN, strip2.blue);
}