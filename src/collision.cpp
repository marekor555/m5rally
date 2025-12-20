//
// Created by marekor on 12.12.2025.
//

#include "collision.h"

void NGon::fromRectangle(float cx, float cy, float w, float h, float angle) {
    float rad = angle * (M_PI / 180.0f);
    float cos0 = cos(rad);
    float sin0 = sin(rad);

    corners.clear();

    corners.push_back(Pos2D(cx - w / 2 * cos0 + h / 2 * sin0, cy - w / 2 * sin0 - h / 2 * cos0));
    corners.push_back(Pos2D(cx + w / 2 * cos0 + h / 2 * sin0, cy + w / 2 * sin0 - h / 2 * cos0));
    corners.push_back(Pos2D(cx + w / 2 * cos0 - h / 2 * sin0, cy + w / 2 * sin0 + h / 2 * cos0));
    corners.push_back(Pos2D(cx - w / 2 * cos0 - h / 2 * sin0, cy - w / 2 * sin0 + h / 2 * cos0));
}

void NGon::drawOutline(M5Canvas* display, const float camposX, const float camposY) const {
    display->drawSmoothLine(corners[0].x - camposX, corners[0].y - camposY, corners[corners.size() - 1].x - camposX, corners[corners.size() - 1].y - camposY,
                      color);

    for (int i = 1; i < corners.size(); ++i) {
        display->drawSmoothLine(corners[i - 1].x - camposX, corners[i - 1].y - camposY, corners[i].x - camposX, corners[i].y - camposY, color);
    }
}

void Line::drawOutline(M5Canvas* display, const float camposX, const float camposY) const {
    for (int i = 1; i < points.size(); ++i) {
        display->drawSmoothLine(points[i - 1].x - camposX, points[i - 1].y - camposY, points[i].x - camposX, points[i].y - camposY, color);
    }
}

int orientation(const Pos2D& p, const Pos2D& q, const Pos2D& r) {
    const float val = (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
    if (fabs(val) < 1e-6f) return 0;
    return (val >= 0) ? 1 : -1;
}

bool NGonCollision(const NGon &n1, const NGon &n2) {
    for (int i = 0; i < n1.corners.size(); i++) {
        Pos2D p1 = n1.corners[i];
        Pos2D p2 = n1.corners[(i + 1) % n1.corners.size()];

        for (int j = 0; j < n2.corners.size(); j++) {
            Pos2D q1 = n2.corners[j];
            Pos2D q2 = n2.corners[(j + 1) % n2.corners.size()];

            const int
                o1 = orientation(p1, p2, q1),
                o2 = orientation(p1, p2, q2),
                o3 = orientation(q1, q2, p1),
                o4 = orientation(q1, q2, p2);

            if (o1 * o2 < 0 && o3 * o4 < 0) return true;
        }
    }
    return false;
}

bool NGonLineCollision(const NGon &n, const Line &l) {
    for (int i = 0; i < n.corners.size(); i++) {
        Pos2D p1 = n.corners[i];
        Pos2D p2 = n.corners[(i + 1) % n.corners.size()];

        for (int j = 0; j < l.points.size()-1; j++) {
            Pos2D q1 = l.points[j];
            Pos2D q2 = l.points[(j + 1) % l.points.size()];

            const int
                o1 = orientation(p1, p2, q1),
                o2 = orientation(p1, p2, q2),
                o3 = orientation(q1, q2, p1),
                o4 = orientation(q1, q2, p2);

            if (o1 * o2 < 0 && o3 * o4 < 0) return true;
        }
    }
    return false;
}

bool collision(float x1, float y1, float w1, float h1, float a1,
               float x2, float y2, float w2, float h2, float a2) {
    NGon nGon1;
    nGon1.fromRectangle(x1, y1, w1, h1, a1);
    NGon nGon2;
    nGon2.fromRectangle(x2, y2, w2, h2, a2);
    return NGonCollision(nGon1, nGon2);
}
