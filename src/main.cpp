// m5rally Copyright (C) 2025  MAREKOR555 licensed under GPLv3
// This program comes with ABSOLUTELY NO WARRANTY;

#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "levels.h"
#include "utils.h"

void listLevels();
void enterSleep();

std::vector<String> options = {
	"Load level",
	"Levels",
	"Enter sleep",
};

void (*results[])() = {
	loadLevel,
	listLevels,
	enterSleep,
};

std::vector<String> levelNames = {
	"exit",
	"Green hell (GT)",
	"Grabowka Poland",
	"Italian chicane (GT)",
	"Thread the needle",
	"The loop",
	"Test level"
};

void (*levels[])() = {
	[]{},
	greenHell,
	grabowkaPL,
	italianChicane,
	threadTheNeedle,
	theLoop,
	testLevel,
};

void enterSleep() {
	M5.Display.fillScreen(TFT_BLACK);
	while (M5Cardputer.Keyboard.isPressed())
		M5Cardputer.update();
	while (!M5Cardputer.Keyboard.isPressed())
		M5Cardputer.update();
}

void listLevels() {
	debounceKeyboard();
	selectFunction(levelNames, levels);
}

void setup() {
	M5Cardputer.begin();
	Serial.begin(9600);
}

void loop() {
	selectFunction(options, results);
	debounceKeyboard();
}
