//
// Created by marekor on 12.12.2025.
//

#include "collison.h"

#include <cmath>

struct Vec {
    float x, y;
};

static Vec rotatePoint(float px, float py, float cx, float cy, float angleRad) {
    float s = std::sin(angleRad);
    float c = std::cos(angleRad);
    px -= cx;
    py -= cy;
    float rx = px * c - py * s;
    float ry = px * s + py * c;
    return {rx + cx, ry + cy};
}

static void project(const Vec corners[4], const Vec& axis, float &minP, float &maxP) {
    minP = maxP = corners[0].x * axis.x + corners[0].y * axis.y;
    for (int i = 1; i < 4; i++) {
        float p = corners[i].x * axis.x + corners[i].y * axis.y;
        if (p < minP) minP = p;
        if (p > maxP) maxP = p;
    }
}

static bool overlap(const Vec A[4], const Vec B[4], const Vec& axis) {
    float minA, maxA, minB, maxB;
    project(A, axis, minA, maxA);
    project(B, axis, minB, maxB);
    return !(maxA < minB || maxB < minA);
}

static Vec normalize(const Vec& v) {
    float len = std::sqrt(v.x*v.x + v.y*v.y);
    if (len == 0) return {0,0};
    return {v.x / len, v.y / len};
}


bool collision(float x1, float y1, float w1, float h1, int a1,
               float x2, float y2, float w2, float h2, int a2)
{
    float r1 = a1 * (3.14159265f / 180.0f);
    float r2 = a2 * (3.14159265f / 180.0f);

    float cx1 = x1 + w1 * 0.5f;
    float cy1 = y1 + h1 * 0.5f;
    float cx2 = x2 + w2 * 0.5f;
    float cy2 = y2 + h2 * 0.5f;

    Vec base1[4] = {
        {x1, y1},
        {x1 + w1, y1},
        {x1 + w1, y1 + h1},
        {x1, y1 + h1}
    };

    Vec base2[4] = {
        {x2, y2},
        {x2 + w2, y2},
        {x2 + w2, y2 + h2},
        {x2, y2 + h2}
    };

    Vec A[4], B[4];
    for (int i = 0; i < 4; i++) {
        A[i] = rotatePoint(base1[i].x, base1[i].y, cx1, cy1, r1);
        B[i] = rotatePoint(base2[i].x, base2[i].y, cx2, cy2, r2);
    }

    Vec axes[4] = {
        { A[1].x - A[0].x, A[1].y - A[0].y },
        { A[3].x - A[0].x, A[3].y - A[0].y },
        { B[1].x - B[0].x, B[1].y - B[0].y },
        { B[3].x - B[0].x, B[3].y - B[0].y },
    };

    for (int i = 0; i < 4; i++) {
        Vec axis = {-axes[i].y, axes[i].x};
        axis = normalize(axis);
        if (!overlap(A, B, axis)) return false;
    }


    return true;
}
