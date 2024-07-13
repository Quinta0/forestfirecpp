# Forest Fire Simulation

This C++ project simulates the spread of forest fires using a cellular automaton model. It provides a visual representation of fire propagation in a forest environment, taking into account various factors such as wind speed, direction, and different vegetation types.

## Features

- Interactive grid-based simulation
- Customizable simulation parameters
- Real-time visualization using SFML graphics library
- Different vegetation types (Normal Forest, Dry Grass, Dense Trees, Water)
- Wind speed and direction influence on fire spread
- Zoom and pan functionality for detailed observation

## Prerequisites

To build and run this project, you'll need:

- C++17 compatible compiler
- CMake (version 3.28 or higher)
- SFML library
- vcpkg package manager (recommended for easy SFML installation)

## Building the Project

1. Clone the repository:
```bash
git clone
```
2. Install SFML using vcpkg:
```bash
vcpkg install sfml:x64-windows
```
3. Configure the project using CMake:
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```
4. Build the project:
```bash
cmake --build .
```

## Running the Simulation

After building the project, run the executable:
    
```bash
./ForestFireCPP
```

You will be prompted to enter simulation parameters:

- Probability of fire spread (0.0 - 1.0)
- Probability of spontaneous ignition (0.0 - 1.0)
- Wind speed (0.0 - 1.0)
- Wind direction in degrees (0 - 360)
- Water ratio (0.0 - 1.0)

After entering the parameters, press Enter to start the simulation.

## Controls

- Left-click and drag to pan the view
- Mouse wheel to zoom in/out

## Contributing

Contributions to improve the simulation or add new features are welcome. Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by various forest fire simulation models and cellular automaton concepts and my already existing Python project (https://github.com/Quinta0/ForestFire.git)
- SFML library for graphics rendering (https://www.sfml-dev.org/)