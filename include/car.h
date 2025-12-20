//
// Created by marekor on 12.12.2025.
//

#ifndef M5RALLY_CAR_H
#define M5RALLY_CAR_H

#include <M5Cardputer.h>

#include "box.h"
#include "collision.h"

class Car {
   public:
    float posX = 0, posY = 0,
          angle = 90, velocity = 0, steer = 0;
    bool handbrake = false;

    void tick(const std::vector<Box>& colliders, const std::vector<NGon>& barriers, const std::vector<Line>& lines);
    void gas();
    void backward();
    void brake();
    void steerLeft();
    void steerRight();
    void init(float x, float y, float a);
    void draw(M5Canvas* display, float camposX, float camposY) const;
	void drawUI(M5Canvas* display) const;
};
#endif  // M5RALLY_CAR_H