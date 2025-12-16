#include "raylib.h"
#include <cmath>

const double minDist = 0.0001;

// Structs
struct Body { 
    double x, y;        
    double x_prev, y_prev; 
    double mass;         

};
struct Force { 
    double fx, fy;       
};


double distance(const Body& b1, const Body& b2) {
    return sqrt((b2.x - b1.x) * (b2.x - b1.x) + (b2.y - b1.y) * (b2.y - b1.y));
}

Force calculateForce(const Body& b1, const Body& b2, double G){
    Force force = {0.0, 0.0};
    double dist = distance(b1, b2);
    if (dist < minDist){
        dist = minDist;
    }
    double dx = b2.x - b1.x;
    double dy = b2.y -b1.y;
    
    double F = G * (b1.mass * b2.mass) / (dist * dist);

    force.fx = F * (dx/dist);
    force.fy = F * (dy/dist);
    return force;
}

void updateBody(Body& body, const Force& force, double dt) {
    double ax = force.fx / body.mass;
    double ay = force.fy / body.mass;
    
    double x_new = 2 * body.x - body.x_prev + ax * dt * dt;
    double y_new = 2 * body.y - body.y_prev + ay * dt * dt;
    
    body.x_prev = body.x;
    body.y_prev = body.y;
    body.x = x_new;
    body.y = y_new;
}



int main() {
    // Window setup
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Three Body Problem");
    SetTargetFPS(60);
    
    // Simulation parameters
    const double dt = 0.001;           // Small timestep
    const int stepsPerFrame = 10;      // Multiple physics steps per frame
    const double G = 1.0;              // Gravitational constant
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    const double scale = 100.0;        // Physics units to pixels
    
    // Bodies initialization
    Body bodies[3];
    Force forces[3];
    // Figure-8 orbit initial conditions
    bodies[0] = {-1.0, 0.0, 0.0, 0.0, 1.0};  // x, y, x_prev, y_prev, mass
    bodies[1] = {1.0, 0.0, 0.0, 0.0, 1.0};
    bodies[2] = {0.0, 0.0, 0.0, 0.0, 1.0};

    // Initial velocities (set previous positions)
    bodies[0].x_prev = bodies[0].x - 0.347 * dt;
    bodies[0].y_prev = bodies[0].y - 0.533 * dt;

    bodies[1].x_prev = bodies[1].x - 0.347 * dt;
    bodies[1].y_prev = bodies[1].y - 0.533 * dt;

    bodies[2].x_prev = bodies[2].x - (-0.694) * dt;
    bodies[2].y_prev = bodies[2].y - (-1.066) * dt;

    // Main game loop
    while (!WindowShouldClose()) {
        
        // Physics update (multiple steps per frame)
        for (int step = 0; step < stepsPerFrame; step++) {
            // Reset forces
            for (int i = 0; i < 3; i++) {
                forces[i].fx = 0;
                forces[i].fy = 0;
            }
            
            // Calculate forces between all pairs
            for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 3; j++) {
                    Force f  = calculateForce(bodies[i], bodies[j], G);
                    forces[i].fx += f.fx;
                    forces[i].fy += f.fy;
                    forces[j].fx -= f.fx;
                    forces[j].fy -= f.fy;
                }
            }
            
            // Update all bodies
            for (int i = 0; i < 3; i++) {
                updateBody(bodies[i], forces[i], dt);
            }
        }
        
        // Drawing
        BeginDrawing();
        
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 10, 20, 30});
            // The last number (30) controls fade speed - higher = faster fade
            
            // Draw bodies
            for (int i = 0; i < 3; i++) {
                int screenX = centerX + (int)(bodies[i].x * scale);
                int screenY = centerY + (int)(bodies[i].y * scale);
                
                // Different colors for each body
                Color colors[3] = {RED, BLUE, YELLOW};
                DrawCircle(screenX, screenY, 5, colors[i]);
            }
            
            DrawText("Three Body Simulation", 10, 10, 20, WHITE);
            
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}