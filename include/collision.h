//
// Created by marekor on 12.12.2025.
//

#ifndef M5RALLY_COLLISION_H
#define M5RALLY_COLLISION_H

#include <M5Cardputer.h>

#include <cmath>

#include "vector"

class Pos2D {
public:
	float x;
	float y;

	Pos2D(const float ix = 0, const float iy = 0) : x(ix), y(iy) {
	}
};

class NGon {
public:
	uint16_t color;
	uint16_t infillColor{};
	std::vector<Pos2D> corners;

	NGon(const uint16_t color) : color(color) {
	}
	NGon(const uint16_t color, const uint16_t infillColor) : color(color), infillColor(infillColor) {
	}

	NGon() : color(0) {
	}

	/**
	 * convert rectangle into NGon format
	 * @param centerX center x coordinate of the rectangle
	 * @param centerY center y coordinate of the rectangle
	 * @param width width of the rectangle
	 * @param height height of the rectangle
	 * @param angle angle of rotation (clockwise from up)
	 */
	void fromRectangle(float centerX, float centerY, float width, float height, float angle);

	void drawOutline(M5Canvas *display, float camposX, float camposY) const;
	void drawInfill(M5Canvas *display, float camposX, float camposY) const;
};

class Line {
public:
	uint16_t color;
	std::vector<Pos2D> points;

	Line(const uint16_t color) : color(color) {
	}

	Line() : color(0) {
	}

	void drawOutline(M5Canvas *display, float camposX, float camposY) const;
};

int orientation(const Pos2D &p, const Pos2D &q, const Pos2D &r);

bool NGonCollision(const NGon &n1, const NGon &n2);

bool NGonLineCollision(const NGon &n, const Line &l);

/**
 * check if rectangles collide (line intersection)
 */
bool collision(float x1, float y1, float w1, float h1, float a1,
				float x2, float y2, float w2, float h2, float a2);
#endif  // M5RALLY_COLLISION_H
