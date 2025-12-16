# Three Body Problem Simulation

A real-time physics simulation of the gravitational three-body problem, built in C++ with Raylib. This project demonstrates the chaotic dynamics of three massive bodies interacting through gravity, with no closed-form solution.

## Project Goals

This project was built as a learning experience to:
- Understand gravitational physics and numerical integration
- Learn C++ development with external libraries
- Practice algorithmic thinking and problem-solving
- Master the development workflow from concept to working application

## Development Process

**Built with Claude 

This project was developed through an iterative collaboration with Claude, where I:
- Drove the architectural decisions and implementation approach
- Reasoned through the physics calculations and data structures
- Made key design choices (Verlet integration, struct-based architecture, etc.)
- Debugged and problem-solved compilation and setup issues
- Claude provided guidance, explanations, and helped me think through solutions rather than just giving me code

This README explicitly documents AI assistance as part of demonstrating modern development practices and transparent collaboration with AI tools.

## Features

- **Accurate Physics**: Implements Velocity Verlet integration for stable, long-term orbital simulations
- **Real-time Visualization**: 60 FPS rendering with smooth motion trails
- **Multiple Physics Steps**: Runs 10 integration steps per frame for enhanced accuracy
- **Collision Prevention**: Implements minimum distance constraints to prevent numerical singularities
- **Figure-8 Orbit**: Pre-configured with stable initial conditions that produce beautiful periodic motion

## Technical Implementation

### Physics Engine
- **Gravitational Force**: F = G × m₁ × m₂ / r²
- **Verlet Integration**: x(t+Δt) = 2x(t) - x(t-Δt) + a(t)Δt²
- **Force Accumulation**: Calculates pairwise forces between all bodies before updating positions
- **Numerical Stability**: Small timestep (0.001) with multiple iterations per frame

### Architecture
- **Struct-based design** for simplicity and performance
- **Helper functions** for distance calculation, force computation, and physics updates
- **Separation of concerns**: Physics loop separate from rendering

### Key Design Decisions
1. **Verlet over Euler**: Chosen for energy conservation in orbital mechanics
2. **Struct over Class**: Simpler data structures for this scale of simulation
3. **Pairwise force calculation**: Uses nested loops with i < j pattern to avoid duplicate calculations
4. **Frame-based trails**: Semi-transparent overlay creates natural fading effect without storing trail data

## Getting Started

### Prerequisites
- **MSYS2** (Windows) with MinGW-w64 compiler
- **Raylib** graphics library
- **Git** (optional, for cloning)

### Installation

1. **Install MSYS2**
```bash
   # Download from https://www.msys2.org/
   # After installation, open MSYS2 terminal and run:
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-raylib
```

2. **Add to PATH**
   - Add `C:\msys64\mingw64\bin` to your system PATH
   - Restart your terminal/IDE

3. **Clone and Build**
```bash
   git clone <your-repo-url>
   cd 3BodyProblem
   
   # Compile
   g++ -g src/main.cpp -o build/simulation.exe -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm
   
   # Run
   ./build/simulation.exe
```

## Project Structure
```
3BodyProblem/
├── src/
│   └── main.cpp           # Main simulation code
├── include/
│   └── raylib.h           # Raylib header
├── lib/
│   └── libraylib.a        # Raylib library
├── build/
│   └── simulation.exe     # Compiled executable
└── README.md
```

## Usage

Simply run the executable - the simulation starts immediately with pre-configured figure-8 orbit initial conditions. Close the window to exit.

**Modifying Initial Conditions:**
Edit the body initialization in `main.cpp`:
```cpp
bodies[0] = {x, y, x_prev, y_prev, mass};
```

##  The Physics

The three-body problem has no general closed-form solution (proved by Henri Poincaré). This simulation uses numerical methods to approximate the motion:

1. **Force Calculation**: For each pair of bodies, calculate gravitational attraction
2. **Force Accumulation**: Sum all forces acting on each body
3. **Integration**: Update positions using Velocity Verlet method
4. **Iteration**: Repeat with small timesteps for accuracy

## Future Enhancements

- [ ] Interactive controls (pause, speed adjustment, zoom)
- [ ] Multiple preset initial conditions
- [ ] Energy/momentum tracking and display
- [ ] Support for N-body simulations (generalize to any number of bodies)
- [ ] Collision physics
- [ ] 3D visualization

## What I Learned

- **Numerical Integration Methods**: Understanding trade-offs between Euler, Verlet, and Runge-Kutta methods
- **C++ Development Workflow**: Setting up build systems, managing libraries, debugging compilation issues
- **Physics Simulation**: Implementing Newton's laws in discrete timesteps
- **Graphics Programming**: Real-time rendering and visual effects
- **Problem Decomposition**: Breaking complex problems into manageable functions and data structures
- **AI-Assisted Development**: Leveraging Claude as a thought partner and technical advisor while maintaining ownership of design decisions

## License

MIT License - feel free to use and modify

## Acknowledgments

- **Claude**: AI pair programming assistant that helped guide architecture decisions, explain physics concepts, debug issues, and think through implementation details
- **Raylib**: Simple and effective graphics library
- **Three-Body Problem**: A beautiful example of deterministic chaos in classical mechanics

---

*This project demonstrates modern software development practices including AI-assisted coding, where the developer drives the process while leveraging AI tools for guidance and knowledge.*