#include "force.h"
#include "body.h"
#include <cmath>

Force calculateForce(const Body &b1, const Body &b2, double G)
{
    Force force = {0.0, 0.0};
    double dist = sqrt((b2.x - b1.x) * (b2.x - b1.x) + (b2.y - b1.y) * (b2.y - b1.y));
    if (dist < 0.0001)
    {
        dist = 0.0001;
    }
    double dx = b2.x - b1.x;
    double dy = b2.y - b1.y;

    double F = G * (b1.mass * b2.mass) / (dist * dist);

    force.fx = F * (dx / dist);
    force.fy = F * (dy / dist);
    return force;
}