//
// Created by marekor on 12.12.2025.
//

#include "car.h"

#include <M5Cardputer.h>

#include "box.h"
#include "collision.h"

#define ACCEL 0.1
#define FRICTION 0.05
#define MAX_FORCE 0.5
#define MAX_VELOCITY 7

#define WHEEL_FORCE 5.0
#define WHEEL_BACKFORCE 0.5
#define MAX_STEER 5.0
#define HAND_EXT 10.0

#define CAR_WIDTH 10
#define CAR_HEIGHT 20

float moveToward(const float value, const float target, const float step) {
	if (value < target) return value + step;
	if (value > target) return value - step;
	return value;
}

float damp(const float value, const float step) {
	if (abs(value) < step) return 0;
	if (value < 0) return value + step;
	if (value > 0) return value - step;
	return value;
}

void Car::tick(const std::vector<Box> &colliders, const std::vector<NGon> &barriers, const std::vector<Line> &lines) {
	const float xvel = -cos((angle + 90) * DEG_TO_RAD), yvel = -sin((angle + 90) * DEG_TO_RAD);

	const float lastPosX = posX;
	const float lastPosY = posY;
	const float lastAngle = angle;

	posX += velocity * xvel;
	posY += velocity * yvel;

	if (velocity != 0) {
		steer = damp(steer, WHEEL_BACKFORCE);
		steer = constrain(steer, -MAX_STEER - handbrake * HAND_EXT, MAX_STEER + handbrake * HAND_EXT);
		if (steer > 0)
			angle += min(steer * (velocity > 0 ? 1.0 : -1.0), MAX_STEER + handbrake * HAND_EXT);
		if (steer < 0)
			angle += max(steer * (velocity > 0 ? 1.0 : -1.0), -MAX_STEER - handbrake * HAND_EXT);
		angle = angle > 360 || angle < -360? 0 : angle;
	}
	if (handbrake) {
		velocity = damp(velocity, FRICTION);
	}

	bool collided = false;
	for (const Box &box: colliders)
		if (collision(posX, posY, CAR_WIDTH, CAR_HEIGHT, angle,
					box.posX, box.posY, box.width, box.height, box.angle))
			collided = true;

	NGon carNGon;
	carNGon.fromRectangle(posX, posY, CAR_WIDTH, CAR_HEIGHT, angle);
	for (const NGon &barrier: barriers) {
		if (NGonCollision(carNGon, barrier))
			collided = true;
	}
	for (const Line &line : lines) {
		if (NGonLineCollision(carNGon, line))
			collided = true;
	}
	if (collided) {
		velocity = -velocity/4;
		steer = -steer/4;
		posX = lastPosX;
		posY = lastPosY;
		angle = lastAngle;
	}
}

void Car::gas() {
	velocity = moveToward(velocity, MAX_VELOCITY, ACCEL);
}

void Car::backward() {
	velocity = moveToward(velocity, -MAX_VELOCITY / 2, ACCEL);
}

void Car::brake() {
	velocity = moveToward(velocity, 0, ACCEL);
}

void Car::steerLeft() {
	steer = moveToward(steer, -MAX_STEER, WHEEL_FORCE);
}

void Car::steerRight() {
	steer = moveToward(steer, MAX_STEER, WHEEL_FORCE);
}

void Car::init(const float x, const float y, const float a) {
	posX = x;
	posY = y;
	angle = a;
}

void Car::draw(M5Canvas *display, const float camposX, const float camposY) const {
	M5Canvas canvas(display);
	canvas.createSprite(CAR_WIDTH, CAR_HEIGHT);
	canvas.fillScreen(TFT_BLACK);
	canvas.fillRoundRect(0, 0, CAR_WIDTH, CAR_HEIGHT, CAR_WIDTH / 10, TFT_NAVY);
	canvas.fillRoundRect(0, 0, CAR_WIDTH, CAR_HEIGHT / 4, CAR_WIDTH / 20, TFT_BLUE);
	canvas.drawWideLine(CAR_WIDTH / 2, 0, CAR_WIDTH / 2, CAR_HEIGHT, CAR_HEIGHT / 6, TFT_NAVY);
	canvas.drawWideLine(0, CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT / 2, CAR_HEIGHT / 10, TFT_YELLOW);
	canvas.drawLine(0, CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT / 2, TFT_NAVY);
	canvas.pushRotateZoomWithAA(display, posX - camposX, posY - camposY, angle, 1.0, 1.0);
	canvas.deleteSprite();
}

void Car::drawUI(M5Canvas *display) const {
	display->drawWideLine(
		2, M5Cardputer.Display.height(),
		2, static_cast<int>(M5Cardputer.Display.height() - M5Cardputer.Display.height() * (fabs(velocity) / MAX_VELOCITY)),
		4.0, handbrake ? TFT_RED : TFT_GREENYELLOW);

}
