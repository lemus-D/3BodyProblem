#include <iostream>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//Custom headers
#include "body.h"
#include "presets.h"
#include "force.h"

#include <cmath>

const double minDist = 0.0001;
const int MAX_BODIES = 10;

// Game states
enum GameState
{
    TITLE_SCREEN,
    CONFIG_BODIES,
    MAIN_MENU,
    SIMULATION,
    PAUSED
};



int main()
{
    // Window setup
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Three Body Problem");
    SetTargetFPS(60);

    // Simulation parameters
    const double dt = 0.001;      // Small timestep
    const int stepsPerFrame = 10; // Multiple physics steps per frame
    double G = 1.0;               // Gravitational constant
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    const double scale = 100.0; // Physics units to pixels

    // Bodies initialization
    Body bodies[MAX_BODIES];
    Force forces[MAX_BODIES];
    int numBodies = 3;

    // Figure-8 orbit initial conditions
    bodies[0] = {-1.0, 0.0, 0.0, 0.0, 1.0}; // x, y, x_prev, y_prev, mass
    bodies[1] = {1.0, 0.0, 0.0, 0.0, 1.0};
    bodies[2] = {0.0, 0.0, 0.0, 0.0, 1.0};

    // Initial velocities (set previous positions)
    bodies[0].x_prev = bodies[0].x - 0.347 * dt;
    bodies[0].y_prev = bodies[0].y - 0.533 * dt;

    bodies[1].x_prev = bodies[1].x - 0.347 * dt;
    bodies[1].y_prev = bodies[1].y - 0.533 * dt;

    bodies[2].x_prev = bodies[2].x - (-0.694) * dt;
    bodies[2].y_prev = bodies[2].y - (-1.066) * dt;

    GameState gameState = TITLE_SCREEN;

    // Main game loop
    while (!WindowShouldClose())
    {
        // In your main loop, add:
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
        }
        if (gameState == TITLE_SCREEN)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                gameState = SIMULATION;
            }
            if (GuiButton((Rectangle){300, 250, 200, 60}, "Custom Configuration"))
            {
                gameState = CONFIG_BODIES;
            }
            if (GuiButton((Rectangle){300, 320, 200, 60}, "Main Menu"))
            {
                gameState = MAIN_MENU;
            }
            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("Three Body Simulation", screenWidth / 2 - 100, screenHeight / 2 - 200, 20, WHITE);
            DrawText("Press ENTER to Start", screenWidth / 2 - 100, screenHeight / 2 - 160, 20, WHITE);
            EndDrawing();
            continue;
        }
        else if (gameState == CONFIG_BODIES)
        {
            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("Custom Configuration Screen", screenWidth / 2 - 150, screenHeight / 2 - 40, 20, WHITE);
            DrawText("Press B to go Back", screenWidth / 2 - 100, screenHeight / 2, 20, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_B))
            {
                gameState = TITLE_SCREEN;
            }
            continue;
        }
        else if (gameState == MAIN_MENU)
        {
            BeginDrawing();
            ClearBackground(DARKGRAY);

            // Title and instructions - centered at top
            DrawText("PRESET SCENARIOS", screenWidth / 2 - MeasureText("PRESET SCENARIOS", 30) / 2, 50, 30, WHITE);
            DrawText("Press S to Start Simulation", screenWidth / 2 - MeasureText("Press S to Start Simulation", 20) / 2, 90, 20, LIGHTGRAY);

            // Layout for 2 rows of 3 buttons
            int buttonWidth = 220;
            int buttonHeight = 60;
            int buttonSpacingX = 250;                                                                       // Space between buttons horizontally
            int buttonSpacingY = 120;                                                                       // Space between rows
            int startX = screenWidth / 2 - (3 * buttonSpacingX) / 2 + buttonSpacingX / 2 - buttonWidth / 2; // Center the grid
            int startY = 180;

            // Draw 6 presets in 2 rows of 3
            for (int i = 0; i < NUM_PRESETS; i++)
            {
                int row = i / 3; // Which row (0 or 1)
                int col = i % 3; // Which column (0, 1, or 2)

                int buttonX = startX + col * buttonSpacingX;
                int buttonY = startY + row * buttonSpacingY;

                Rectangle buttonRect = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

                // Draw button
                if (GuiButton(buttonRect, presets[i].name))
                {
                    loadPreset(i, bodies, numBodies, G);
                    gameState = SIMULATION;
                }

                // Draw description below button (wrapped text)
                int descX = buttonX;
                int descY = buttonY + buttonHeight + 5;
                int descWidth = buttonWidth;

                // Simple word wrap for description
                DrawText(TextFormat("%.30s", presets[i].description), descX, descY, 12, LIGHTGRAY);
                if (strlen(presets[i].description) > 30)
                {
                    DrawText(TextFormat("%.30s", presets[i].description + 30), descX, descY + 15, 12, LIGHTGRAY);
                }
            }

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
            BeginDrawing();
            DrawText("Paused", screenWidth / 2 - 40, screenHeight / 2 - 20, 20, WHITE);
            DrawText("Press O to Resume", screenWidth / 2 - 80, screenHeight / 2 + 20, 20, WHITE);
            EndDrawing();
            continue;
        }
        if (IsKeyPressed(KEY_P))
        {
            gameState = PAUSED;
        }
        if (gameState == SIMULATION)
        {
            for (int step = 0; step < stepsPerFrame; step++)
            {
                // Reset forces
                for (int i = 0; i < 3; i++)
                {
                    forces[i].fx = 0;
                    forces[i].fy = 0;
                }

                // Calculate forces between all pairs
                for (int i = 0; i < 3; i++)
                {
                    for (int j = i + 1; j < 3; j++)
                    {
                        Force f = calculateForce(bodies[i], bodies[j], G);
                        forces[i].fx += f.fx;
                        forces[i].fy += f.fy;
                        forces[j].fx -= f.fx;
                        forces[j].fy -= f.fy;
                    }
                }

                // Update all bodies
                for (int i = 0; i < 3; i++)
                {
                    updateBody(bodies[i], forces[i], dt);
                }
            }

            // Drawing
            BeginDrawing();

            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 10, 20, 30});
            // The last number (30) controls fade speed - higher = faster fade

            // Draw bodies
            for (int i = 0; i < 3; i++)
            {
                int screenX = centerX + (int)(bodies[i].x * scale);
                int screenY = centerY + (int)(bodies[i].y * scale);

                // Different colors for each body
                Color colors[3] = {RED, BLUE, YELLOW};
                DrawCircle(screenX, screenY, 5, colors[i]);
            }

            DrawText("Three Body Simulation", 10, 10, 20, WHITE);
            DrawText("Press P to Pause", 10, 40, 20, WHITE);

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}