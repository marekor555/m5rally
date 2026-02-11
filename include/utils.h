//
// Created by marekor on 24.01.2026.
//

#ifndef M5RALLY_UTILS_H
#define M5RALLY_UTILS_H
#include <Arduino.h>
#include <vector>

/*
 * wait for all keys to be released
 */
void debounceKeyboard();

/**
 * gui part for scrolling through many string options
 * @param msg array of options
 * @param scrollX choose to enable/disable x axis scroll
 * @param mainColor main text color
 * @param extraColor color of special options, options with | at start and end
 * @return text of the selected option
 */
String scrollTextArrHighlight(const std::vector<String> &msg, bool scrollX, int mainColor, int extraColor);

/**
 * gui part for scrolling through many function options
 * @param descriptions function descriptions
 * @param functions function definitions
 */
void selectFunction(const std::vector<String> &descriptions, void (*functions[])());

/**
 * check if a point is visible on the screen
 * @param x point x coordinate
 * @param y point y coordinate
 * @param camposX camera x coordinate
 * @param camposY camera y coordinate
 * @param margin margin for detection (in pixels)
 * @return
 */
bool isVisible(float x, float y, float camposX, float camposY, float margin);

/**
 * check if a line is visible on the screen
 * @param x1 point 1 x coordinate
 * @param y1 point 1 y coordinate
 * @param x2 point 2 x coordinate
 * @param y2 point 2 y coordinate
 * @param camposX camera x coordinate
 * @param camposY camera y coordinate
 * @return
 */
bool intersectScreen(float x1, float y1, float x2, float y2, float camposX, float camposY);

#endif //M5RALLY_UTILS_H