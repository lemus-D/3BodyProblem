#include <iostream>
#include "raylib.h"

#include "raygui.h"

#include <cmath>

const int MAX_BODIES = 10;

enum GameState
{
    TITLE_SCREEN,
    CONFIG_SETUP,
    CONFIG_BODIES,
    MAIN_MENU,
    SIMULATION,
    PAUSED
};

struct Body;

struct BodyConfig
{
    float mass, x, y, vx, vy;
    char massText[16], xText[16], yText[16], vxText[16], vyText[16];
    bool editingMass, editingX, editingY, editingVx, editingVy;
};

void setup(int width, int height, const char *title, int fps);

void togleFullscreen(int &screenWidth, int &screenHeight, int &centerX, int &centerY);

void checkScreenSize(int &screenWidth, int &screenHeight, int &centerX, int &centerY);

void titleScreen(int screenWidth, int screenHeight, GameState *gameState);

void configScreen(int screenWidth, int screenHeight, GameState *gameState, int *numBodies, int *currentBodyIndex, char numBodiesText[], bool *editingNumBodies, double *G, char GText[], bool *editingG);

void configBodiesScreen(int screenWidth, int screenHeight, GameState *gameState, int *numBodies, int *currentBodyIndex, double G, Body bodies[], double dt);

void mainMenuScreen(int screenWidth, int screenHeight, GameState *gameState, Body bodies[],int *numBodies , double *G);

void pauseScreen(int screenWidth, int screenHeight, GameState *gameState);

void simulationScreen(int screenWidth, int screenHeight, Body bodies[], int numBodies, double scale);
