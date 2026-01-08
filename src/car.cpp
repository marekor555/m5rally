//
// Created by marekor on 12.12.2025.
//

#include "car.h"

#include <M5Cardputer.h>

#include "box.h"
#include "collision.h"

#define ACCEL 80
#define FRICTION 20
#define MAX_VELOCITY 200.0

#define WHEEL_FORCE 1600.0
#define WHEEL_BACKFORCE 600.0
#define MAX_STEER 200.0
#define HAND_EXT 10.0

#define CAR_WIDTH 7
#define CAR_HEIGHT 14

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

bool Car::tick(const std::vector<Box> &colliders, const std::vector<NGon> &barriers, const std::vector<Line> &lines,
				const Line &finishLine, const float delta) {
	runTime += delta;
	const float xvel = -cos((angle + 90) * DEG_TO_RAD), yvel = -sin((angle + 90) * DEG_TO_RAD);

	const float lastPosX = posX;
	const float lastPosY = posY;
	const float lastAngle = angle;

	posX += velocity * xvel * delta;
	posY += velocity * yvel * delta;

	if (velocity != 0) {
		steer = damp(steer, WHEEL_BACKFORCE * delta);
		const float realMaxSteer = MAX_STEER + (handbrake ? HAND_EXT : 0);
		steer = constrain(steer, -realMaxSteer, realMaxSteer);

		const float rotationDir = (velocity > 0 ? 1.0f : -1.0f);
		angle += steer * rotationDir * delta;

		if (angle > 360) angle -= 360;
		if (angle < 0) angle += 360;
	}
	if (fabs(velocity) > 0) {
		const float appliedFriction = handbrake ? (FRICTION * 3.0f) : FRICTION;
		velocity = damp(velocity, appliedFriction * delta);
	}

	bool collided = false;
	for (const Box &box: colliders)
		if (collision(posX, posY, CAR_WIDTH, CAR_HEIGHT, angle,
					box.posX, box.posY, box.width, box.height, box.angle))
			collided = true;

	NGon carNGon;
	carNGon.fromRectangle(posX, posY, CAR_WIDTH, CAR_HEIGHT, angle);
	if (NGonLineCollision(carNGon, finishLine)) return false;

	for (const NGon &barrier: barriers) {
		if (NGonCollision(carNGon, barrier))
			collided = true;
	}
	for (const Line &line: lines) {
		if (NGonLineCollision(carNGon, line))
			collided = true;
	}
	if (collided) {
		velocity = -velocity * 0.5f;
		steer = 0;
		posX = lastPosX;
		posY = lastPosY;
		angle = lastAngle;
	}
	return true;
}

void Car::gas(const float delta) {
	velocity = moveToward(velocity, MAX_VELOCITY, ACCEL * delta);
}

void Car::backward(const float delta) {
	velocity = moveToward(velocity, -MAX_VELOCITY / 2, ACCEL * delta);
}

void Car::brake(const float delta) {
	velocity = moveToward(velocity, 0, ACCEL* delta);
}

void Car::steerLeft(const float delta) {
	steer = moveToward(steer, -MAX_STEER, WHEEL_FORCE * delta);
}

void Car::steerRight(const float delta) {
	steer = moveToward(steer, MAX_STEER, WHEEL_FORCE * delta);
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

void Car::drawUI(M5Canvas *display, const float delta) const {
	display->drawWideLine(
		2, M5Cardputer.Display.height(),
		2, static_cast<int>(M5Cardputer.Display.height() - M5Cardputer.Display.height() * (
								fabs(velocity) / MAX_VELOCITY)),
		4.0, handbrake ? TFT_RED : TFT_GREENYELLOW);
	display->setTextSize(1);
	display->setTextColor(TFT_WHITE, TFT_BLACK);
	display->drawRightString("T: " + String(runTime, 3), display->width(), 0);
}
