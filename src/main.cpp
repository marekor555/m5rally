// m5rally Copyright (C) 2025  MAREKOR555 licensed under GPLv3
// This program comes with ABSOLUTELY NO WARRANTY;

#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "car.h"

Car car;

std::vector<Box> boxes;

bool justPressed = false;

void setup() {
	M5Cardputer.begin();
	Serial.begin(9600);
	car.init(-40, -40);
	boxes.push_back(Box().init(0, 0, 10, 10, 180, TFT_GREEN));
	boxes.push_back(Box().init(-100, 0, 20, 200, 0, TFT_DARKGREY)); // left
	boxes.push_back(Box().init(100, 0, 20, 200, 0, TFT_DARKGREY)); // right
	boxes.push_back(Box().init(0, -100, 20, 200, 90, TFT_DARKGREY)); // top
	boxes.push_back(Box().init(0, 100, 20, 200, 90, TFT_DARKGREY)); // down
}

void loop() {
	M5Cardputer.update();
	car.tick(boxes);
	M5Canvas canvas(&M5.Lcd);

	if (M5Cardputer.Keyboard.isPressed()) {
		auto state = M5Cardputer.Keyboard.keysState();
		for (int i = 0; i < state.word.size(); i++) {
			switch (state.word[i]) {
				case 'w': car.gas(); break;
				case ',': car.steerLeft(); break;
				case 'a': car.brake(); break;
				case '.': car.steerRight(); break;
				case '/': car.handbrake = justPressed ? !car.handbrake : car.handbrake; break;
			}
		}
	}

	if (M5Cardputer.Keyboard.isPressed() && !justPressed)
		justPressed = true;
	else if ((M5Cardputer.Keyboard.isPressed() && justPressed) || !M5Cardputer.Keyboard.isPressed())
		justPressed = false;


	canvas.createSprite(240, 135);
	canvas.fillScreen(TFT_BLACK);
	canvas.setTextColor(TFT_WHITE, TFT_BLACK);
	canvas.setTextSize(1);

	car.draw(&canvas, car.posX-M5Cardputer.Lcd.width()/2, car.posY-M5Cardputer.Lcd.height()/2);
	for (Box box : boxes) {
		box.draw(&canvas, car.posX-M5Cardputer.Lcd.width()/2, car.posY-M5Cardputer.Lcd.height()/2);
	}

	canvas.printf("x: %.2f y: %.2f\n", car.posX, car.posY);
	canvas.printf("a: %.2f v: %.2f\n", car.angle, car.velocity);
	canvas.printf("s: %.2f\nf: %.2f\n", car.steer, car.appliedForce);
	canvas.printf("hb: %s\n", car.handbrake ? "ON" : "OFF");

	canvas.pushSprite(0, 0);
	canvas.deleteSprite();
	delay(17); // ~60fps
}
