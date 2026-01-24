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
                        if (posy<=-1) {
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
            for (int i = 0; i<msg.size(); i++) {
                if (i == highlight) {
                    M5Cardputer.Lcd.fillRect(10*posx, 10 * posy + 10 * (i + 1), 10*msg[i].length(), 10, TFT_DARKGRAY);
                }
                if (msg[i].startsWith("|") && msg[i].endsWith("|")) {
                    M5Cardputer.Lcd.setTextColor(extraColor);
                    M5Cardputer.Lcd.drawString(msg[i].substring(1, msg[i].length()-1), 10*posx, 10 * posy + 10 * (i + 1));
                    M5Cardputer.Lcd.setTextColor(mainColor);
                } else {
                    M5Cardputer.Lcd.drawString(msg[i], 10*posx, 10 * posy + 10 * (i + 1));
                }
            }
            change = false;
        }
        delay(1);
    }
    return msg[highlight];
}