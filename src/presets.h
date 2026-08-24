#pragma once

struct Body;

/*
 * Shared integrator settings.
 *
 * These live here rather than in main.cpp because loadPreset() has to convert
 * an initial velocity into a Verlet x_prev, and that conversion is only correct
 * if it uses the exact timestep the main loop integrates with. They used to be
 * declared separately in two files, so changing one silently corrupted every
 * preset's starting velocity.
 *
 * dt was 1e-3 at 10 substeps. That is too coarse to resolve the close
 * approaches in the periodic orbits below: Butterfly I missed closing by 16
 * units per period at 1e-3 and closes to within 5e-3 at 1e-4. The substep count
 * is raised in step so the simulated time per frame is unchanged.
 */
constexpr double SIM_DT = 0.0001;
constexpr int SIM_STEPS_PER_FRAME = 100;

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
