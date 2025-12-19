#pragma once

struct Body;

struct PresetConfig
{
    const char *name;
    const char *description;
    int numBodies;
    double G;
    // Store initial conditions directly
    struct BodyInit
    {
        double x, y, vx, vy, mass;
    } bodies[10];
};
extern const PresetConfig presets[];
extern const int NUM_PRESETS;
void loadPreset(int presetIndex, Body bodies[], int &numBodies, double &G);
