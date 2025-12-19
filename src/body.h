#pragma once
#include <cmath>

struct Force;

struct Body
{
    double x, y;
    double vx, vy;
    double x_prev, y_prev;
    double mass;
};

void updateBody(Body &body, const Force force, double dt);
double distance(const Body &b1, const Body &b2);