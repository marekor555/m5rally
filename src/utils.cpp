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

String scrollTextArrHighlight(const std::vector<String> msg, bool scrollX, int mainColor, int extraColor) {
	debounceKeyboard();
	M5Cardputer.Lcd.setTextColor(mainColor);
	int posx = 1, posy = 0;
	int highlight = 0;
	bool change = true;
	while (true) {
		M5Cardputer.update();
		if (M5Cardputer.Keyboard.isPressed()) {
			const Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
			if (status.word.size() > 0) {
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
