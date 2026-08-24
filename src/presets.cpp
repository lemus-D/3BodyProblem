#include "presets.h"
#include "body.h"

/*
 * Every entry below is either an exact periodic solution or a configuration
 * whose velocities are derived from the force balance, not eyeballed.
 *
 * Four of them come from the Suvakov-Dmitrasinovic catalogue of periodic
 * three-body orbits (Phys. Rev. Lett. 110, 114301, 2013). Those all share one
 * form: two unit masses at (-1, 0) and (1, 0) with the same velocity (p1, p2),
 * and a third unit mass at the origin moving at (-2*p1, -2*p2). Net momentum is
 * zero by construction, and the orbit returns to its start after one period.
 *
 * Each was checked by integrating exactly one period and measuring how far the
 * bodies land from where they started. Errors are noted per preset.
 */

const PresetConfig presets[] = {
    {"Figure-8",
     "Three equal masses trace one\nfigure-8 track - an exact\nperiodic solution",
     3,
     1.0,
     // p1 = 0.347111, p2 = 0.532728, period 6.324449, closes to 8e-4
     {{-1.0, 0.0, 0.347111, 0.532728, 1.0},
      {1.0, 0.0, 0.347111, 0.532728, 1.0},
      {0.0, 0.0, -0.694222, -1.065456, 1.0}}},

    {"Butterfly I",
     "Another exact periodic orbit\nfor three equal masses,\nfound in 2013",
     3,
     1.0,
     // p1 = 0.306893, p2 = 0.125507, period 6.235641, closes to 5e-3
     {{-1.0, 0.0, 0.306893, 0.125507, 1.0},
      {1.0, 0.0, 0.306893, 0.125507, 1.0},
      {0.0, 0.0, -0.613786, -0.251014, 1.0}}},

    {"Moth I",
     "Same three masses wound into\na much longer closed path",
     3,
     1.0,
     // p1 = 0.464445, p2 = 0.396060, period 14.893911, closes to 3e-4
     {{-1.0, 0.0, 0.464445, 0.396060, 1.0},
      {1.0, 0.0, 0.464445, 0.396060, 1.0},
      {0.0, 0.0, -0.928890, -0.792120, 1.0}}},

    {"Yin-Yang I",
     "Two bodies spiral around each\nother while the third loops\nthe pair",
     3,
     1.0,
     // p1 = 0.513938, p2 = 0.304736, period 17.328380, closes to 1e-2
     {{-1.0, 0.0, 0.513938, 0.304736, 1.0},
      {1.0, 0.0, 0.513938, 0.304736, 1.0},
      {0.0, 0.0, -1.027876, -0.609472, 1.0}}},

    {"Mini Solar System",
     "One heavy sun, three light\nplanets, each at its exact\ncircular speed",
     4,
     1.0,
     // Circular orbit speed is sqrt(G*M/r) with M = 10:
     //   r=1.5 -> 2.581989, r=2.5 -> 2.0, r=3.5 -> 1.690309
     // The old values were roughly a third of these, so the planets simply
     // fell into the sun instead of orbiting it.
     {
         {0.0, 0.0, 0.0, 0.0, 10.0},           // Sun
         {1.5, 0.0, 0.0, 2.58198890, 0.01},    // Inner planet
         {2.5, 0.0, 0.0, 2.0, 0.01},           // Middle planet
         {-3.5, 0.0, 0.0, -1.69030851, 0.01},  // Outer planet
     }},

    {"Lagrange Triangle",
     "Three equal masses in a\nrotating equilateral triangle.\nEqual masses make it unstable",
     3,
     1.0,
     // For circumradius R=1 the triangle side is sqrt(3), giving a net inward
     // pull of G*m^2/(sqrt(3)*R^2). Setting that equal to m*v^2/R gives
     //   v = sqrt(G*m/(sqrt(3)*R)) = 0.7598357
     // The old table used 1/sqrt(3) = 0.577, which is v^2, not v - so the
     // triangle collapsed inward instead of holding.
     //
     // Note this configuration is only linearly stable when one mass dominates
     // (Gascheau's criterion needs a mass ratio above ~24.96), so with three
     // equal masses it holds for a while and then breaks up. That is correct
     // behaviour, not a bug.
     {
         {1.0, 0.0, 0.0, 0.75983569, 1.0},
         {-0.5, 0.86602540, -0.65817928, -0.37991784, 1.0},
         {-0.5, -0.86602540, 0.65817928, -0.37991784, 1.0},
     }},

    {"Binary + Planet",
     "Two heavy stars in a circular\nbinary with a planet orbiting\nthe pair from outside",
     3,
     1.0,
     // Equal-mass circular binary: each star sits at radius a=1 from the
     // barycentre, separation 2a, so m*v^2/a = G*m^2/(2a)^2 gives
     //   v = sqrt(G*m/(4a)) = sqrt(8/4) = 1.4142136
     // The planet orbits the combined mass 16 at r=6: sqrt(16/6) = 1.6329932.
     // At the old r=3 the planet was inside the zone where a binary of
     // separation 2 ejects it, so it was thrown out almost immediately.
     {
         {-1.0, 0.0, 0.0, 1.41421356, 8.0},   // Star 1
         {1.0, 0.0, 0.0, -1.41421356, 8.0},   // Star 2
         {0.0, 6.0, 1.63299316, 0.0, 1.0},    // Planet
     }},
};

const int NUM_PRESETS = 7;

void loadPreset(int presetIndex, Body bodies[], int &numBodies, double &G)
{
    const PresetConfig &preset = presets[presetIndex];

    numBodies = preset.numBodies;
    G = preset.G;

    // Shift to the centre-of-mass frame. Without this any preset carrying net
    // momentum slowly walks off screen, which looks like a simulation bug.
    double totalMass = 0.0, cx = 0.0, cy = 0.0, px = 0.0, py = 0.0;
    for (int i = 0; i < numBodies; i++)
    {
        const PresetConfig::BodyInit &b = preset.bodies[i];
        totalMass += b.mass;
        cx += b.mass * b.x;
        cy += b.mass * b.y;
        px += b.mass * b.vx;
        py += b.mass * b.vy;
    }
    if (totalMass > 0.0)
    {
        cx /= totalMass;
        cy /= totalMass;
        px /= totalMass;
        py /= totalMass;
    }

    for (int i = 0; i < numBodies; i++)
    {
        const PresetConfig::BodyInit &b = preset.bodies[i];

        bodies[i].x = b.x - cx;
        bodies[i].y = b.y - cy;
        bodies[i].mass = b.mass;

        // Encode the initial velocity as a previous position. This must use the
        // same timestep the main loop integrates with, hence the shared SIM_DT.
        bodies[i].x_prev = bodies[i].x - (b.vx - px) * SIM_DT;
        bodies[i].y_prev = bodies[i].y - (b.vy - py) * SIM_DT;
    }
}
