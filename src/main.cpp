// m5rally Copyright (C) 2025  MAREKOR555 licensed under GPLv3
// This program comes with ABSOLUTELY NO WARRANTY;

#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "levels.h"

bool justPressed = false;

std::vector<String> options = {
	"Test level",
	"First level",
	"Second level",
	"Enter sleep",
};

void enterSleep() {
	M5Cardputer.Lcd.fillScreen(TFT_BLACK);
	while (M5Cardputer.Keyboard.isPressed()) M5Cardputer.update();
	while (!M5Cardputer.Keyboard.isPressed()) M5Cardputer.update();
}

void (*results[])() = {
	testLevel,
	first,
	second,
	enterSleep,
};

int option = 0;
void setup() {
	M5Cardputer.begin();
	Serial.begin(9600);
}

void loop() {
	M5Cardputer.update();
	if (M5Cardputer.Keyboard.isPressed()) {
		const auto status = M5Cardputer.Keyboard.keysState();
		for (const char key: status.word) {
			switch (key) {
				case ';':
					option--;
					if (option < 0) option = options.size() - 1;
					break;
				case '.':
					option++;
					if (option >= options.size()) option = 0;
					break;
				case '/':
					results[option]();
			}
		}
		while (M5Cardputer.Keyboard.isPressed()) M5Cardputer.update();
	}
	M5Canvas canvas(&M5Cardputer.Lcd);
	canvas.createSprite(240, 135);
	canvas.fillScreen(TFT_BLACK);
	canvas.setTextColor(TFT_WHITE, TFT_BLACK);
	int previous = option - 1;
	if (previous < 0) previous = options.size() - 1;
	if (previous >= options.size()) previous = 0;
	int next = option + 1;
	if (next < 0) next = options.size() - 1;
	if (next >= options.size()) next = 0;

	canvas.setTextSize(2);
	canvas.drawCenterString(options[option], M5Cardputer.Lcd.width()/2, M5Cardputer.Lcd.height()/2);

	canvas.setTextSize(1);
	canvas.drawCenterString(options[previous], M5Cardputer.Lcd.width()/2, M5Cardputer.Lcd.height()*0.25);
	canvas.drawCenterString(options[next], M5Cardputer.Lcd.width()/2, M5Cardputer.Lcd.height()*0.75);

	canvas.pushSprite(0, 0);
	canvas.deleteSprite();
}
