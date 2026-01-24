//
// Created by marekor on 24.01.2026.
//

#ifndef M5RALLY_UTILS_H
#define M5RALLY_UTILS_H
#include <Arduino.h>
#include <vector>

void debounceKeyboard();

String scrollTextArrHighlight(std::vector<String> msg, bool scrollX, int mainColor, int extraColor);

bool isVisible(float x, float y, float camposX, float camposY, float margin);

bool intersectScreen(float x1, float y1, float x2, float y2, float camposX, float camposY);

#endif //M5RALLY_UTILS_H