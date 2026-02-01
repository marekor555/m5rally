#include <utils.h>
#include <M5Cardputer.h>

void debounceKeyboard() {
	M5Cardputer.update();
	while (M5Cardputer.Keyboard.isPressed()) {
		M5Cardputer.update();
		const auto status = M5Cardputer.Keyboard.keysState();
		if (status.shift || status.del) {
			delay(100);
			break;
		}
		delay(10);
	}
}

bool isVisible(const float x, const float y, const float camposX, const float camposY, const float margin) {
	const float screenX = x - camposX, screenY = y - camposY;

	return
			screenX >= -margin && screenX < M5Cardputer.Display.width() + margin &&
			screenY >= -margin && screenY < M5Cardputer.Display.height() + margin;
}

bool intersectScreen(const float x1, const float y1, const float x2, const float y2, const float camposX,
					const float camposY) {
	const float
			right = camposX + M5Cardputer.Display.width(),
			bottom = camposY + M5Cardputer.Display.height(),
			left = camposX,
			top = camposY;

	if (max(x1, x2) < left) return false;
	if (min(x1, x2) > right) return false;
	if (max(y1, y2) < top) return false;
	if (min(y1, y2) > bottom) return false;

	return true;
}

void selectFunction(const std::vector<String> &descriptions, void (*functions[])()) {
	int option = 0;
	while (true) {
		M5Cardputer.update();
		if (M5Cardputer.Keyboard.isPressed()) {
			const auto status = M5Cardputer.Keyboard.keysState();
			for (const char key: status.word) {
				switch (key) {
					case ';':
						option--;
						if (option < 0)
							option = descriptions.size() - 1;
						break;
					case '.':
						option++;
						if (option >= descriptions.size())
							option = 0;
						break;
					default: ;
				}
			}
			if (status.enter) {
				functions[option]();
				return;
			}
			while (M5Cardputer.Keyboard.isPressed())
				M5Cardputer.update();
		}
		M5Canvas canvas(&M5.Lcd);
		canvas.createSprite(240, 135);
		canvas.fillScreen(TFT_BLACK);
		canvas.setTextColor(TFT_WHITE, TFT_BLACK);
		int previous = option - 1;
		if (previous < 0)
			previous = descriptions.size() - 1;
		if (previous >= descriptions.size())
			previous = 0;
		int next = option + 1;
		if (next < 0)
			next = descriptions.size() - 1;
		if (next >= descriptions.size())
			next = 0;

		canvas.setTextSize(2);
		canvas.drawCenterString(descriptions[option], M5Cardputer.Display.width() / 2,
								M5Cardputer.Lcd.height() / 2);

		canvas.setTextSize(1);
		canvas.setTextColor(TFT_DARKGREY, TFT_BLACK);
		canvas.drawCenterString(descriptions[previous], M5Cardputer.Lcd.width() / 2,
								M5Cardputer.Lcd.height() * 0.25);
		canvas.drawCenterString(descriptions[next], M5Cardputer.Lcd.width() / 2,
								M5Cardputer.Lcd.height() * 0.75);

		canvas.pushSprite(0, 0);
		canvas.deleteSprite();
	}
}

String scrollTextArrHighlight(const std::vector<String> &msg, const bool scrollX, const int mainColor, const int extraColor) {
	debounceKeyboard();
	M5Cardputer.Lcd.setTextColor(mainColor);
	int posx = 1, posy = 0;
	int highlight = 0;
	bool change = true;
	while (true) {
		M5Cardputer.update();
		if (M5Cardputer.Keyboard.isPressed()) {
			const Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
			if (!status.word.empty()) {
				switch (status.word[0]) {
					case ';':
						if (posy <= -1) {
							posy++;
							highlight--;
							change = true;
						}
						break;
					case '.':
						posy--;
						highlight++;
						change = true;
						break;
					case '/':
						if (scrollX) {
							posx--;
							change = true;
						}
						break;
					case ',':
						if (scrollX && posx < 1) {
							posx++;
							change = true;
						}
						break;
					default: ;
				}
			}
			if (status.opt || status.enter) {
				break;
			}
			debounceKeyboard();
		}
		if (change) {
			M5Cardputer.Lcd.setTextColor(TFT_WHITE);
			M5Cardputer.Lcd.setTextSize(1);
			M5Cardputer.Lcd.fillScreen(TFT_BLACK);
			for (int i = 0; i < msg.size(); i++) {
				if (i == highlight) {
					M5Cardputer.Lcd.fillRect(10 * posx, 10 * posy + 10 * (i + 1), 10 * msg[i].length(), 10,
											TFT_DARKGRAY);
				}
				if (msg[i].startsWith("|") && msg[i].endsWith("|")) {
					M5Cardputer.Lcd.setTextColor(extraColor);
					M5Cardputer.Lcd.drawString(msg[i].substring(1, msg[i].length() - 1), 10 * posx,
												10 * posy + 10 * (i + 1));
					M5Cardputer.Lcd.setTextColor(mainColor);
				} else {
					M5Cardputer.Lcd.drawString(msg[i], 10 * posx, 10 * posy + 10 * (i + 1));
				}
			}
			change = false;
		}
		delay(1);
	}
	return msg[highlight];
}
