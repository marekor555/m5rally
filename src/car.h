//
// Created by marekor on 12.12.2025.
//

#ifndef M5RALLY_CAR_H
#define M5RALLY_CAR_H

#include <M5Cardputer.h>

#include "box.h"

class Car {
public:
	float posX = 0, posY = 0,
			angle = 90, velocity = 0, steer = 0, appliedForce = 0;
	bool handbrake = false;

	void tick(const std::vector<Box> &colliders);
	void gas();
	void brake();
	void steerLeft();
	void steerRight();
	void init(float x, float y);
	void draw(M5Canvas *display, float camposX, float camposY) const;
};
#endif //M5RALLY_CAR_H