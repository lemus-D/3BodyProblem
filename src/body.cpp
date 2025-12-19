#include "body.h"
#include "force.h"
#include <cmath>

void updateBody(Body &body, const Force force, double dt)
{
    double ax = force.fx / body.mass;
    double ay = force.fy / body.mass;

    double x_new = 2 * body.x - body.x_prev + ax * dt * dt;
    double y_new = 2 * body.y - body.y_prev + ay * dt * dt;

    body.vx = (x_new - body.x_prev) / (2 * dt);
    body.vy = (y_new - body.y_prev) / (2 * dt);

    body.x_prev = body.x;
    body.y_prev = body.y;
    body.x = x_new;
    body.y = y_new;
}

double distance(const Body &b1, const Body &b2)
{
    return sqrt((b2.x - b1.x) * (b2.x - b1.x) + (b2.y - b1.y) * (b2.y - b1.y));
}