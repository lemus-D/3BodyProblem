#include <iostream>
#include "raylib.h"


#include "raygui.h"

// Custom headers
#include "body.h"
#include "presets.h"
#include "force.h"
#include "view.h"

#include <cmath>

const double minDist = 0.0001;
bool firstFrame = true;

// Game states


int main()
{
    // Window setup
    int screenWidth = 1200;
    int screenHeight = 1000;
    const char *title = "Three Body Problem";
    setup(screenWidth, screenHeight, title, 60);

    // Simulation parameters
    const double dt = 0.001;      // Small timestep
    const int stepsPerFrame = 10; // Multiple physics steps per frame
    double G = 1.0;               // Gravitational constant
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    const double scale = 100.0; // Physics units to pixels

    // Bodies initialization
    Body bodies[MAX_BODIES];
    Force forces[MAX_BODIES];
    int numBodies = 3;

    // Configuration
    int currentBodyIndex = 0;


    // Config for main settings
    char numBodiesText[16] = "3";
    char GText[16] = "1.0";
    bool editingNumBodies = false;
    bool editingG = false;

    GameState gameState = TITLE_SCREEN;

    // Main game loop
    while (!WindowShouldClose())
    {

        // Toggle Fullscreen
        if (IsKeyPressed(KEY_F11))
        {
            togleFullscreen(screenWidth, screenHeight, centerX, centerY);
        }
        if (gameState == TITLE_SCREEN)
        {
            titleScreen(screenWidth, screenHeight, &gameState);
        }
        else if (gameState == CONFIG_SETUP)
        {  configScreen(screenWidth, screenHeight, &gameState, &numBodies, &currentBodyIndex, numBodiesText, &editingNumBodies, &G, GText, &editingG);
        }
        else if (gameState == CONFIG_BODIES)
        {
            configBodiesScreen(screenWidth, screenHeight, &gameState, &numBodies, &currentBodyIndex, G, bodies, dt);
        }
        else if (gameState == MAIN_MENU)
        {
            mainMenuScreen(screenWidth, screenHeight, &gameState, bodies, &numBodies, &G);
        }
        else if (gameState == PAUSED)
        {
            pauseScreen(screenWidth, screenHeight, &gameState);
        }
        if (IsKeyPressed(KEY_P))
        {
            gameState = PAUSED;
        }
        else if (gameState == SIMULATION)
        {
            for (int step = 0; step < stepsPerFrame; step++)
            {
                // Reset forces
                for (int i = 0; i < numBodies; i++)
                {
                    forces[i].fx = 0;
                    forces[i].fy = 0;
                }

                // Calculate forces between all pairs
                for (int i = 0; i < numBodies; i++)
                {
                    for (int j = i + 1; j < numBodies; j++)
                    {
                        Force f = calculateForce(bodies[i], bodies[j], G);
                        forces[i].fx += f.fx;
                        forces[i].fy += f.fy;
                        forces[j].fx -= f.fx;
                        forces[j].fy -= f.fy;
                    }
                }

                // Update all bodies
                for (int i = 0; i < numBodies; i++)
                {
                    updateBody(bodies[i], forces[i], dt);
                }
            }
            simulationScreen(screenWidth, screenHeight, bodies, numBodies, scale);
        }
    }

    CloseWindow();
    return 0;
}