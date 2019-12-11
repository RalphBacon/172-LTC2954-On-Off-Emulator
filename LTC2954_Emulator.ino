/*
 * Press to start, press again to shutdown. Long press to
 * override µC and shutdown (eg if frozen state).
 *
 */
#include "Arduino.h"
#define ENABLE 14 // aka A0
#define PWRINT 2

// Shutdown flag
bool shutDownRequest = false;
bool shutDown = false;

void pwrIntISR();

//==================================
//SETUP    SETUP     SETUP     SETUP
//==================================
void setup()
{
	// Keep this unit running
	pinMode(ENABLE, OUTPUT);
	digitalWrite(ENABLE, HIGH);

	Serial.begin(9600);
	Serial.println("Setup starts");
	Serial.println("Power established.");
	Serial.println("Shut down requests not yet accepted.");

	// Set up the interrupt to service shut down request
	pinMode(PWRINT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PWRINT), pwrIntISR, LOW);
}

//==============================
// LOOP    LOOP    LOOP     LOOP
//==============================
void loop()
{
	//
	if (millis() > 5000 && !shutDownRequest) {
		shutDownRequest = true;
		Serial.println("Shutdown requests now accepted.");

		// Take your finger off that button!
		while (digitalRead(PWRINT) == LOW);
	}

	// Represents the Arduino doing some housekeeping shutdown work
	if (shutDownRequest && shutDown) {
		Serial.println("Shut Down Request Received.");
		delay(1000);
		Serial.println("Finalising SD card data.");
		delay(1000);
		Serial.print("Shutting down...");
		delay(1000);
		Serial.flush();
		digitalWrite(ENABLE, LOW);
	}
	else {
		// A ShutDown signal has potentially come too early; ignore.
		shutDown = false;
	}

	delay(100);
}

// This routine will set the flag to shut down the µC
void pwrIntISR() {
	// Set the flag
	shutDown = true;
}
