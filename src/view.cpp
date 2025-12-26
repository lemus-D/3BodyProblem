#include "view.h"
#include "body.h"
#include "presets.h"

BodyConfig bodyConfigs[MAX_BODIES];

void setup(int width, int height, const char *title, int fps)
{
    // Initialize the game window
    InitWindow(width, height, title);
    SetTargetFPS(fps);
}

void togleFullscreen(int &screenWidth, int &screenHeight, int &centerX, int &centerY)
{
    // Toggle fullscreen mode
    ToggleFullscreen();
    checkScreenSize(screenWidth, screenHeight, centerX, centerY);
}

void checkScreenSize(int &screenWidth, int &screenHeight, int &centerX, int &centerY)
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    centerX = screenWidth / 2;
    centerY = screenHeight / 2;
}

void titleScreen(int screenWidth, int screenHeight, GameState *gameState)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    const char *title = "Three Body Simulation";
    int titleWidth = MeasureText(title, 20);
    DrawText(title, screenWidth / 2 - titleWidth / 2, screenHeight / 2 - 200, 20, WHITE);

    DrawText("Press F11 for Fullscreen", screenWidth / 2 - 120, screenHeight / 2 - 160, 20, WHITE);
    DrawText("Made by Darwin Lemus", screenWidth / 2 - 110, screenHeight / 2 + 250, 20, WHITE);

    int defaultSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    if (GuiButton((Rectangle){(float)screenWidth / 2 - 150, (float)screenHeight / 2 - 60, 300, 80}, "Custom Configuration"))
    {
        *gameState = CONFIG_SETUP;
    }

    if (GuiButton((Rectangle){(float)screenWidth / 2 - 150, (float)screenHeight / 2 + 70, 300, 80}, "Main Menu"))
    {
        *gameState = MAIN_MENU;
    }

    EndDrawing();

    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultSize);
}

void configScreen(int screenWidth, int screenHeight, GameState *gameState, int *numBodies, int *currentBodyIndex, char numBodiesText[], bool *editingNumBodies, double *G, char GText[], bool *editingG)
{ // Initialize with defaults
    for (int i = 0; i < MAX_BODIES; i++)
    {
        bodyConfigs[i] = {1.0, 0.0, 0.0, 0.0, 0.0, "1.0", "0.0", "0.0", "0.0", "0.0", false, false, false, false, false};
    }
    int defaultSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 26);
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // Title
    DrawText("SIMULATION SETUP", screenWidth / 2 - MeasureText("SIMULATION SETUP", 30) / 2, 80, 30, WHITE);

    int labelX = screenWidth / 2 - 260;
    int inputX = screenWidth / 2 + 20;
    int startY = 200;
    int spacing = 130;

    // Number of bodies
    GuiLabel((Rectangle){(float)labelX, (float)startY, 300, 30}, "Number of Bodies (1-10):");
    if (GuiTextBox((Rectangle){(float)inputX, (float)startY, 150, 30}, numBodiesText, 16, *editingNumBodies))
    {
        *editingNumBodies = !*editingNumBodies;
    }

    // G constant with slider
    GuiLabel((Rectangle){(float)labelX + 70, (float)startY + spacing - 6, 200, 30}, "G Constant:");
    float GFloat = (float)*G;
    GuiSlider((Rectangle){(float)inputX, (float)startY + spacing, 150, 20}, "0.1", "5", &GFloat, 0.1f, 5.0f);
    *G = GFloat;

    // Text box for precise G input
    sprintf(GText, "%.2f", *G);
    if (GuiTextBox((Rectangle){(float)inputX + 180, (float)startY + spacing - 5, 83, 30}, GText, 16, *editingG))
    {
        *editingG = !*editingG;
    }
    if (!*editingG)
    {
        double newG = atof(GText);
        if (newG > 0.1 && newG <= 5.0)
            *G = newG;
    }

    // Next button
    if (GuiButton((Rectangle){screenWidth / 2 - 150, screenHeight - 200, 300, 50}, "Configure Bodies"))
    {
        // Validate and update numBodies
        int newNumBodies = atoi(numBodiesText);
        if (newNumBodies >= 1 && newNumBodies <= MAX_BODIES)
        {
            *numBodies = newNumBodies;
            currentBodyIndex = 0;
            *gameState = CONFIG_BODIES;
        }
    }

    // Back button
    if (GuiButton((Rectangle){screenWidth / 2 - 100, screenHeight - 140, 200, 50}, "Back"))
    {
        *gameState = TITLE_SCREEN;
    }

    EndDrawing();
    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultSize);
}

void configBodiesScreen(int screenWidth, int screenHeight, GameState *gameState, int *numBodies, int *currentBodyIndex, double G, Body bodies[], double dt)
{

    BeginDrawing();
    ClearBackground(DARKGRAY);

    // Title
    DrawText(TextFormat("CONFIGURE BODY %d of %d", *currentBodyIndex + 1, *numBodies),
             screenWidth / 2 - 150, 50, 25, WHITE);

    int labelX = 150;
    int sliderX = 350;
    int textBoxX = 570;
    int startY = 150;
    int spacing = 70;

    BodyConfig *config = &bodyConfigs[*currentBodyIndex];

    // Mass
    GuiLabel((Rectangle){(float)labelX, (float)startY, 150, 30}, "Mass:");
    GuiSlider((Rectangle){(float)sliderX, (float)startY, 200, 20}, "0.1", "10", &config->mass, 0.1f, 10.0f);
    sprintf(config->massText, "%.2f", config->mass);
    if (GuiTextBox((Rectangle){(float)textBoxX, (float)startY - 5, 100, 30}, config->massText, 16, config->editingMass))
    {
        config->editingMass = !config->editingMass;
    }
    if (!config->editingMass)
    {
        double val = atof(config->massText);
        if (val > 0.1 && val <= 10.0)
            config->mass = val;
    }

    // Position X
    GuiLabel((Rectangle){(float)labelX, (float)startY + spacing, 150, 30}, "Position X:");
    GuiSlider((Rectangle){(float)sliderX, (float)startY + spacing, 200, 20}, "-5", "5", &config->x, -5.0f, 5.0f);
    sprintf(config->xText, "%.2f", config->x);
    if (GuiTextBox((Rectangle){(float)textBoxX, (float)startY + spacing - 5, 100, 30}, config->xText, 16, config->editingX))
    {
        config->editingX = !config->editingX;
    }
    if (!config->editingX)
    {
        double val = atof(config->xText);
        if (val >= -5.0 && val <= 5.0)
            config->x = val;
    }

    // Position Y
    GuiLabel((Rectangle){(float)labelX, (float)startY + spacing * 2, 150, 30}, "Position Y:");
    GuiSlider((Rectangle){(float)sliderX, (float)startY + spacing * 2, 200, 20}, "-5", "5", &config->y, -5.0f, 5.0f);
    sprintf(config->yText, "%.2f", config->y);
    if (GuiTextBox((Rectangle){(float)textBoxX, (float)startY + spacing * 2 - 5, 100, 30}, config->yText, 16, config->editingY))
    {
        config->editingY = !config->editingY;
    }
    if (!config->editingY)
    {
        double val = atof(config->yText);
        if (val >= -5.0 && val <= 5.0)
            config->y = val;
    }

    // Velocity X
    GuiLabel((Rectangle){(float)labelX, (float)startY + spacing * 3, 150, 30}, "Velocity X:");
    GuiSlider((Rectangle){(float)sliderX, (float)startY + spacing * 3, 200, 20}, "-2", "2", &config->vx, -2.0f, 2.0f);
    sprintf(config->vxText, "%.2f", config->vx);
    if (GuiTextBox((Rectangle){(float)textBoxX, (float)startY + spacing * 3 - 5, 100, 30}, config->vxText, 16, config->editingVx))
    {
        config->editingVx = !config->editingVx;
    }
    if (!config->editingVx)
    {
        double val = atof(config->vxText);
        if (val >= -2.0 && val <= 2.0)
            config->vx = val;
    }

    // Velocity Y
    GuiLabel((Rectangle){(float)labelX, (float)startY + spacing * 4, 150, 30}, "Velocity Y:");
    GuiSlider((Rectangle){(float)sliderX, (float)startY + spacing * 4, 200, 20}, "-2", "2", &config->vy, -2.0f, 2.0f);
    sprintf(config->vyText, "%.2f", config->vy);
    if (GuiTextBox((Rectangle){(float)textBoxX, (float)startY + spacing * 4 - 5, 100, 30}, config->vyText, 16, config->editingVy))
    {
        config->editingVy = !config->editingVy;
    }
    if (!config->editingVy)
    {
        double val = atof(config->vyText);
        if (val >= -2.0 && val <= 2.0)
            config->vy = val;
    }

    // Navigation buttons
    int buttonY = screenHeight - 120;

    if (*currentBodyIndex > 0)
    {
        if (GuiButton((Rectangle){screenWidth / 2 - 250, (float)buttonY, 150, 50}, "< Previous"))
        {
            (*currentBodyIndex)--;
        }
    }

    if (*currentBodyIndex < *numBodies - 1)
    {
        if (GuiButton((Rectangle){screenWidth / 2 + 100, (float)buttonY, 150, 50}, "Next >"))
        {
            (*currentBodyIndex)++;
        }
    }
    else
    {
        if (GuiButton((Rectangle){screenWidth / 2 + 100, (float)buttonY, 150, 50}, "Start Sim"))
        {
            // Copy configs to bodies array
            for (int i = 0; i < *numBodies; i++)
            {
                bodies[i].x = bodyConfigs[i].x;
                bodies[i].y = bodyConfigs[i].y;
                bodies[i].mass = bodyConfigs[i].mass;

                // Initialize x_prev, y_prev from velocities
                bodies[i].x_prev = bodies[i].x - bodyConfigs[i].vx * dt;
                bodies[i].y_prev = bodies[i].y - bodyConfigs[i].vy * dt;
            }
            *gameState = SIMULATION;
        }
    }

    // Back to setup
    if (GuiButton((Rectangle){screenWidth / 2 - 75, (float)buttonY + 60, 150, 40}, "Back to Setup"))
    {
        *gameState = CONFIG_SETUP;
    }

    EndDrawing();
}

void mainMenuScreen(int screenWidth, int screenHeight, GameState *gameState, Body bodies[], int *numBodies, double *G)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawText("PRESET SCENARIOS", screenWidth / 2 - MeasureText("PRESET SCENARIOS", 30) / 2, 50, 30, WHITE);

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
            loadPreset(i, bodies, *numBodies, *G);
            *gameState = SIMULATION;
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
}

void pauseScreen(int screenWidth, int screenHeight, GameState *gameState)
{
    if (IsKeyPressed(KEY_O))
    {
        *gameState = SIMULATION;
    }
    if (IsKeyPressed(KEY_R))
    {
        *gameState = TITLE_SCREEN;
    }
    BeginDrawing();
    DrawText("Paused", screenWidth / 2 - 40, screenHeight / 2 - 20, 20, WHITE);
    DrawText("Press O to Resume", screenWidth / 2 - 80, screenHeight / 2 + 20, 20, WHITE);
    DrawText("Press R to Restart", screenWidth / 2 - 80, screenHeight / 2 + 60, 20, WHITE);
    EndDrawing();
}

void simulationScreen(int screenWidth, int screenHeight, Body bodies[], int numBodies, double scale)
{
   BeginDrawing();

            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 10, 20, 30});
            // The last number (30) controls fade speed - higher = faster fade

            // Draw bodies
            for (int i = 0; i < numBodies; i++)
            {
                int screenX = screenWidth/2  + (int)(bodies[i].x * scale);
                int screenY = screenHeight/2 + (int)(bodies[i].y * scale);

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