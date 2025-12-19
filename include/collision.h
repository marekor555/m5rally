//
// Created by marekor on 12.12.2025.
//

#ifndef M5RALLY_COLLISON_H
#define M5RALLY_COLLISON_H

#include <M5Cardputer.h>

#include <cmath>

#include "vector"

class Pos2D {
   public:
    float x;
    float y;

    Pos2D(float ix = 0, float iy = 0) : x(ix), y(iy) {}
};

class NGon {
   public:
    std::vector<Pos2D> corners;

    void fromRectangle(float centerX, float centerY, float width, float height, float angle);

    void drawOutline(M5Canvas* display, float camposX, float camposY) const;
};

int orientation(const Pos2D& p, const Pos2D& q, const Pos2D& r);

bool NGonCollision(NGon n1, NGon n2);

bool collision(float x1, float y1, float w1, float h1, float a1,
               float x2, float y2, float w2, float h2, float a2);
#endif  // M5RALLY_COLLISON_H