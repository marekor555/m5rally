//
// Created by marekor on 12.12.2025.
//

#include "box.h"

#include "collision.h"

Box Box::init(const float x, const float y, const int w, const int h, const float a, const uint16_t c) {
    posX = x;
    posY = y;
    width = w;
    height = h;
    angle = a;
    colorFill = c;
    return *this;
}

void Box::draw(M5Canvas* display, const float camposX, const float camposY) const {
    // NGon ngon;
    // ngon.fromRectangle(posX, posY, width, height, angle);
    M5Canvas canvas(display);
    canvas.createSprite(width, height);
    canvas.fillScreen(colorFill);
    canvas.pushRotateZoomWithAA(display, posX - camposX, posY - camposY, angle, 1.0, 1.0);
    canvas.deleteSprite();
    // ngon.drawOutline(display, camposX, camposY);
}
