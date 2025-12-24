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
            checkScreenSize(screenWidth, screenHeight, centerX, centerY);
            BeginDrawing();
            ClearBackground(DARKGRAY);

            DrawText("PRESET SCENARIOS", screenWidth / 2 - MeasureText("PRESET SCENARIOS", 30) / 2, 50, 30, WHITE);
            DrawText("Press S to Start Simulation", screenWidth / 2 - MeasureText("Press S to Start Simulation", 20) / 2, 90, 20, LIGHTGRAY);

            int buttonWidth = 220;
            int buttonHeight = 60;
            int buttonSpacingX = 250;
            int buttonSpacingY = 150;
            int startX = screenWidth / 2 - (3 * buttonSpacingX) / 2 + buttonSpacingX / 2 - buttonWidth / 2;
            int startY = 180;

            int defaultSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
            GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

            for (int i = 0; i < NUM_PRESETS; i++)
            {
                int row = i / 3;
                int col = i % 3;

                int buttonX = startX + col * buttonSpacingX;
                int buttonY = startY + row * buttonSpacingY;

                Rectangle buttonRect = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

                if (GuiButton(buttonRect, presets[i].name))
                {
                    loadPreset(i, bodies, numBodies, G);
                    firstFrame = true;
                    gameState = SIMULATION;
                }

                Rectangle descRect = {
                    (float)buttonX,
                    (float)buttonY + buttonHeight + 10,
                    (float)buttonWidth,
                    60};

                DrawTextEx(GetFontDefault(), presets[i].description, (Vector2){descRect.x, descRect.y}, 14, 1.0f, LIGHTGRAY);
            }

            GuiSetStyle(DEFAULT, TEXT_SIZE, defaultSize);

            EndDrawing();

            if (IsKeyPressed(KEY_S))
            {
                gameState = SIMULATION;
            }
            continue;
        }
        else if (gameState == PAUSED)
        {
            if (IsKeyPressed(KEY_O))
            {
                gameState = SIMULATION;
            }
            if (IsKeyPressed(KEY_R))
            {
                gameState = TITLE_SCREEN;
            }
            BeginDrawing();
            DrawText("Paused", screenWidth / 2 - 40, screenHeight / 2 - 20, 20, WHITE);
            DrawText("Press O to Resume", screenWidth / 2 - 80, screenHeight / 2 + 20, 20, WHITE);
            DrawText("Press R to Restart", screenWidth / 2 - 80, screenHeight / 2 + 60, 20, WHITE);
            EndDrawing();
            continue;
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
            // Drawing
            BeginDrawing();

            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 10, 20, 30});
            // The last number (30) controls fade speed - higher = faster fade

            // Draw bodies
            for (int i = 0; i < numBodies; i++)
            {
                int screenX = centerX + (int)(bodies[i].x * scale);
                int screenY = centerY + (int)(bodies[i].y * scale);

                // Different colors for each body
                Color colors[10] = {RED, BLUE, YELLOW, GREEN, ORANGE, PURPLE, PINK, BROWN, DARKBLUE, LIGHTGRAY}; // Add more colors if needed
                DrawCircle(screenX, screenY, 5 * bodies[i].mass > 8 ? 8 : 5 * bodies[i].mass < 5 ? 5
                                                                                                 : 5 * bodies[i].mass,
                           colors[i]);
            }

            DrawText("Three Body Simulation", 10, 10, 20, WHITE);
            DrawText("Press P to Pause", 10, 40, 20, WHITE);

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}