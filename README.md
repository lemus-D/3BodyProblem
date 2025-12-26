# Three Body Problem Simulation

A real-time physics simulation of the gravitational three-body problem, built in C++ with Raylib. This project demonstrates the chaotic dynamics of three massive bodies interacting through gravity, with no closed-form solution.

## 🎯 Project Goals

This project was built as a learning experience to:
- Understand gravitational physics and numerical integration
- Learn C++ development with external libraries
- Practice algorithmic thinking and problem-solving
- Master the development workflow from concept to working application
- Develop clean, modular code architecture with proper separation of concerns

## 🤖 Development Process

**Built with Claude (Anthropic's AI Assistant)**

This project was developed through an iterative collaboration with Claude, where I:
- Drove the architectural decisions and implementation approach
- Reasoned through the physics calculations and data structures
- Made key design choices (Verlet integration, struct-based architecture, modular file organization)
- Debugged and problem-solved compilation and setup issues
- Designed and implemented the UI/UX flow and menu system
- Claude provided guidance, explanations, and helped me think through solutions rather than just giving me code

This README explicitly documents AI assistance as part of demonstrating modern development practices and transparent collaboration with AI tools.

## ✨ Features

### Physics & Simulation
- **Accurate Physics**: Implements Velocity Verlet integration for stable, long-term orbital simulations
- **Real-time Visualization**: 60 FPS rendering with smooth motion trails
- **Multiple Physics Steps**: Runs 10 integration steps per frame for enhanced accuracy
- **Collision Prevention**: Implements minimum distance constraints to prevent numerical singularities
- **Scalable N-Body Support**: Handles up to 10 bodies simultaneously

### User Interface
- **Preset Scenarios**: 6 pre-configured simulations showcasing different orbital dynamics:
  - Figure-8 Orbit (stable periodic solution)
  - Binary Star + Planet (chaotic three-body system)
  - Mini Solar System (hierarchical system with central massive body)
  - Chaotic Butterfly (high-energy unstable configuration)
  - Lagrange Triangle (rotating equilateral triangle)
  - Pythagorean Three-Body (unequal mass system)

- **Custom Configuration**: Full control over simulation parameters
  - Set number of bodies (1-10)
  - Adjust gravitational constant G
  - Configure individual body properties:
    - Mass (0.1 - 10.0)
    - Initial position (x, y)
    - Initial velocity (vx, vy)
  - Hybrid input: sliders for quick adjustments + text boxes for precise values

- **Interactive Controls**:
  - F11: Toggle fullscreen
  - P: Pause/Resume simulation
  - Dynamic screen size adaptation

### Visual Features
- **Trail Effects**: Semi-transparent fade creates beautiful orbital paths
- **Color-Coded Bodies**: Each body has a unique color for easy tracking
- **Mass-Proportional Sizing**: Body size reflects its mass
- **Adaptive Rendering**: Smooth transitions between windowed and fullscreen modes

## 🔧 Technical Implementation

### Physics Engine
- **Gravitational Force**: F = G × m₁ × m₂ / r²
- **Verlet Integration**: x(t+Δt) = 2x(t) - x(t-Δt) + a(t)Δt²
- **Force Accumulation**: Calculates pairwise forces between all bodies before updating positions
- **Numerical Stability**: Small timestep (0.001) with multiple iterations per frame
- **Minimum Distance Constraint**: Prevents singularities when bodies approach too closely

### Architecture & Code Organization

**Modular Design Philosophy:**
- **Separation of Concerns**: Physics, rendering, and UI are separated into distinct modules
- **Header/Implementation Split**: Clean interface definitions with separate implementations
- **Struct-Based Data**: Simple, efficient data structures for performance
- **Centralized Configuration**: Preset definitions and constants in dedicated files

**Key Modules:**
1. **body.h/cpp**: Body structure and physics update logic
2. **force.h/cpp**: Force calculation and gravitational interactions
3. **presets.h/cpp**: Preset scenario configurations and loading
4. **view.h/cpp**: Rendering and visualization utilities
5. **raygui_impl.cpp**: GUI implementation (raygui integration)
6. **main.cpp**: Application entry point, game loop, and state management

### Key Design Decisions
1. **Verlet over Euler**: Chosen for energy conservation in orbital mechanics
2. **Struct over Class**: Simpler data structures for this scale of simulation
3. **Pairwise force calculation**: Uses nested loops with i < j pattern to avoid duplicate calculations
4. **Frame-based trails**: Semi-transparent overlay creates natural fading effect without storing trail data
5. **Modular file structure**: Enables easier maintenance and future expansion
6. **State machine pattern**: Clean separation between title screen, configuration, and simulation states

## 🚀 Getting Started

### Prerequisites
- **MSYS2** (Windows) with MinGW-w64 compiler
- **Raylib** graphics library
- **raygui** (included in project)
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
   cd 3BODYPROBLEM
   
   # Compile
   g++ -g src/*.cpp -o build/simulation.exe -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm
   
   # Run
   ./build/simulation.exe
```

## 📁 Project Structure
```
3BODYPROBLEM/
├── .vscode/
│   ├── settings.json           # VS Code workspace settings
│   └── tasks.json              # Build task configuration
├── build/
│   └── simulation.exe          # Compiled executable
├── include/
│   ├── raylib.h                # Raylib graphics library header
│   └── raygui.h                # raygui UI library header
├── lib/
│   └── libraylib.a             # Raylib static library
├── src/
│   ├── main.cpp                # Application entry point & game loop
│   ├── body.h                  # Body structure definition
│   ├── body.cpp                # Body physics implementation
│   ├── force.h                 # Force calculation interface
│   ├── force.cpp               # Gravitational force implementation
│   ├── presets.h               # Preset configurations
│   ├── presets.cpp             # Preset loading functions
│   ├── view.h                  # Rendering utilities
│   ├── view.cpp                # Visualization implementation
│   └── raygui_impl.cpp         # raygui integration
└── README.md                   # Project documentation
```

## 🎮 Usage

### Running the Simulation

1. **Launch the application**
```bash
   ./build/simulation.exe
```

2. **Choose your path:**
   - **Preset Scenarios**: Quick start with pre-configured interesting systems
   - **Custom Configuration**: Build your own simulation from scratch

### Custom Configuration Workflow

1. **Simulation Setup**:
   - Enter number of bodies (1-10)
   - Set gravitational constant G (0.1-5.0)

2. **Body Configuration**:
   - For each body, configure:
     - Mass (affects gravitational pull and visual size)
     - Position (starting location in space)
     - Velocity (initial motion vector)
   - Use sliders for quick adjustments or text boxes for precision
   - Navigate between bodies with Previous/Next buttons

3. **Run Simulation**:
   - Press "Start Simulation" to begin
   - Press P to pause/resume
   - Press F11 for fullscreen mode

### Keyboard Controls
- **F11**: Toggle fullscreen mode
- **P**: Pause/Resume simulation
- **O**: Resume from pause

## 🧮 The Physics

The three-body problem has no general closed-form solution (proved by Henri Poincaré in the late 1800s). This simulation uses numerical methods to approximate the motion:

1. **Force Calculation**: For each pair of bodies, calculate gravitational attraction
2. **Force Accumulation**: Sum all forces acting on each body (superposition principle)
3. **Integration**: Update positions and velocities using Velocity Verlet method
4. **Iteration**: Repeat with small timesteps for accuracy and stability

**Why Verlet Integration?**
- Conserves energy better than Euler method
- Produces stable orbits over long time periods
- Time-reversible (important for orbital mechanics)
- Computationally efficient

## 🔮 Future Enhancements

- [x] Multiple preset initial conditions
- [x] Interactive controls (pause)
- [x] Support for N-body simulations
- [ ] Speed adjustment controls (time dilation)
- [ ] Zoom and pan controls
- [ ] Energy/momentum tracking and display
- [ ] Collision physics with momentum conservation
- [ ] 3D visualization
- [ ] Trail length customization
- [ ] Export/import custom configurations
- [ ] Replay and time-reversal features
- [ ] Center of mass tracking option

## 📚 What I Learned

### Technical Skills
- **Numerical Integration Methods**: Deep understanding of trade-offs between Euler, Verlet, and Runge-Kutta methods
- **C++ Development Workflow**: Setting up build systems, managing libraries, debugging compilation issues
- **Modular Programming**: Designing clean, maintainable code with proper separation of concerns
- **Physics Simulation**: Implementing Newton's laws in discrete timesteps
- **Graphics Programming**: Real-time rendering, visual effects, and UI development with Raylib/raygui
- **Problem Decomposition**: Breaking complex problems into manageable functions and data structures

### Development Practices
- **Version Control**: Organizing project structure for maintainability
- **Documentation**: Writing clear, comprehensive README files
- **User Experience**: Designing intuitive interfaces and control schemes
- **Testing & Debugging**: Identifying and resolving visual artifacts and numerical issues
- **AI-Assisted Development**: Leveraging Claude as a thought partner and technical advisor while maintaining ownership of design decisions

### Physics & Mathematics
- **Gravitational Dynamics**: Understanding multi-body gravitational interactions
- **Chaos Theory**: Observing how small changes in initial conditions lead to dramatically different outcomes
- **Numerical Stability**: Managing precision and error accumulation in long-running simulations
- **Conservation Laws**: Implementing physics that respects energy and momentum conservation

## 📄 License

MIT License - feel free to use and modify

## 🙏 Acknowledgments

- **Claude (Anthropic)**: AI pair programming assistant that helped guide architecture decisions, explain physics concepts, debug issues, and think through implementation details
- **Raylib**: Simple and effective graphics library created by Ramon Santamaria
- **raygui**: Immediate-mode GUI library for Raylib
- **Three-Body Problem**: A beautiful example of deterministic chaos in classical mechanics
- **Christopher Moore**: Discoverer of the figure-8 solution in 1993

## 📖 References

- Poincaré, Henri. "New Methods of Celestial Mechanics" (1892)
- Moore, Cristopher. "Braids in Classical Dynamics" (1993)
- Hairer, Wanner, Lubich. "Geometric Numerical Integration" (2006)

---

*This project demonstrates modern software development practices including AI-assisted coding, where the developer drives the process while leveraging AI tools for guidance and knowledge. All design decisions, architecture choices, and implementations were made by the developer with Claude serving as a collaborative thought partner.*