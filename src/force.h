#pragma once

struct Body;
struct Force
{
    double fx, fy;
};

Force calculateForce(const Body &b1, const Body &b2, double G);