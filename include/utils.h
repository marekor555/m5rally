//
// Created by marekor on 24.01.2026.
//

#ifndef M5RALLY_UTILS_H
#define M5RALLY_UTILS_H
#include <Arduino.h>
#include <vector>

void debounceKeyboard();

String scrollTextArrHighlight(std::vector<String> msg, bool scrollX, int mainColor, int extraColor);

#endif //M5RALLY_UTILS_H