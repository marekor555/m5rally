//
// Created by marekor on 12.12.2025.
//

#ifndef M5RALLY_BOX_H
#define M5RALLY_BOX_H
#include "M5GFX.h"


class Box {
public:
	float posX, posY, angle = 0;
	int width, height;
	mutable uint16_t colorFill;

	Box init(float x, float y, int w, int h, float a, uint16_t color);
	void draw(M5Canvas *display, float camposX, float camposY) const;
};


#endif //M5RALLY_BOX_H