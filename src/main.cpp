// m5rally Copyright (C) 2025  MAREKOR555 licensed under GPLv3
// This program comes with ABSOLUTELY NO WARRANTY;

#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

#include "levels.h"

bool justPressed = false;

std::vector<String> options = {
	"t - test level",
	"s - enter sleep",
};

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
				case 't':
					testLevel();
			}
		}
	}
	M5Canvas canvas(&M5Cardputer.Lcd);
	canvas.createSprite(240, 135);
	canvas.fillScreen(TFT_BLACK);
	canvas.setTextColor(TFT_WHITE, TFT_BLACK);
	canvas.setTextSize(1);

	for (const String &option : options) {
		canvas.println(option);
	}

	canvas.pushSprite(0, 0);
	canvas.deleteSprite();
	delay(17);  // ~60fps
}
