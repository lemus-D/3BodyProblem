#include "presets.h"
#include "body.h"

// Presets
const PresetConfig presets[] = {
    {"Figure-8 Orbit",
     "Three equal masses trace a\nfigure-8 pattern - a rare\nstable periodic solution",
     3,
     1.0,
     {{-0.97000436, 0.24308753, 0.466203685, 0.43236573, 1.0},
      {0.0, 0.0, -0.93240737, -0.86473146, 1.0},
      {0.97000436, -0.24308753, 0.466203685, 0.43236573, 1.0}}},
      
    {"Binary Star + Planet",
     "Two heavy stars orbit each\nother while a light planet\ndances chaotically around them",
     3,
     1.0,
     {
         {-1.0, 0.0, 0.0, 0.5, 3.0}, // Star 1
         {1.0, 0.0, 0.0, -0.5, 3.0}, // Star 2
         {0.0, 3.0, 1.2, 0.0, 0.2}   // Planet
     }},
     
    {"Mini Solar System",
     "One massive sun with three\nplanets in stable orbits\nat different distances",
     4,
     1.0,
     {
         {0.0, 0.0, 0.0, 0.0, 10.0}, // Sun (massive, stationary)
         {1.5, 0.0, 0.0, 0.85, 0.5}, // Inner planet
         {2.5, 0.0, 0.0, 0.65, 0.5}, // Middle planet
         {3.5, 0.0, 0.0, 0.52, 0.5}  // Outer planet
     }},
     
    {"Chaotic Butterfly",
     "Four bodies in a high-energy\nconfiguration that creates\nbeautiful chaotic patterns",
     4,
     1.0,
     {{-1.0, -1.0, 0.3, 0.3, 1.0},
      {1.0, -1.0, -0.3, 0.3, 1.0},
      {-1.0, 1.0, 0.3, -0.3, 1.0},
      {1.0, 1.0, -0.3, -0.3, 1.0}}},
      
    {"Lagrange Triangle",
     "Three equal masses form a\nrotating equilateral triangle\n- perfectly stable!",
     3,
     1.0,
     {
         {1.0, 0.0, 0.0, 0.577, 1.0},      // Body at 0°
         {-0.5, 0.866, -0.5, -0.289, 1.0}, // Body at 120°
         {-0.5, -0.866, 0.5, -0.289, 1.0}  // Body at 240°
     }},
     
    {"Pyth Three-Body",
     "Three unequal masses (3-4-5\nratio) in a figure-8 like\npattern",
     3,
     1.0,
     {{-1.0, 0.0, 0.35, 0.4, 3.0},
      {0.5, 0.0, 0.25, 0.5, 4.0},
      {1.5, 0.0, -0.6, -0.9, 5.0}}},};
    
      

const int NUM_PRESETS = 6;

void loadPreset(int presetIndex, Body bodies[], int &numBodies, double &G)
{
    const PresetConfig &preset = presets[presetIndex];

    numBodies = preset.numBodies;
    G = preset.G;

    double dt = 0.001;
    for (int i = 0; i < numBodies; i++)
    {
        bodies[i].x = preset.bodies[i].x;
        bodies[i].y = preset.bodies[i].y;
        bodies[i].mass = preset.bodies[i].mass;

        // Calculate x_prev, y_prev from velocity
        bodies[i].x_prev = bodies[i].x - preset.bodies[i].vx * dt;
        bodies[i].y_prev = bodies[i].y - preset.bodies[i].vy * dt;
    }
}