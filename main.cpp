#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants
const int GRID_SIZE = 256;
const float CELL_SIZE = 5.0f;

// Vegetation types
enum class CellType {
    NormalForest,
    DryGrass,
    DenseTrees,
    Water,
    Burning,
    Burned
};

// Simulation parameters
struct SimParams {
    float p = 0.8f;
    float pstart = 0.01f;
    float w_speed = 0.5f;
    float w_direction = 30.0f;
    float water_ratio = 0.175f;
};

// Grid class
class Grid {
public:
    Grid() : size(0) {}

    Grid(int size, float water_ratio) : size(size), cells(size, std::vector<CellType>(size, CellType::NormalForest)) {
        initializeGrid(water_ratio);
    }

    void initializeGrid(float water_ratio) {
        // Initialize grid similar to Python version
        cells[size / 2][size / 2] = CellType::Burning;
        cells[size / 4][size / 4] = CellType::DryGrass;
        cells[3 * size / 4][3 * size / 4] = CellType::DenseTrees;

        // Implement water initialization
        int num_water_cells = static_cast<int>(size * size * water_ratio);
        for (int i = 0; i < num_water_cells; ++i) {
            int x = rand() % size;
            int y = rand() % size;
            cells[y][x] = CellType::Water;
        }
    }

    void update(float p, float pstart, float w_speed, float w_direction) {
        std::vector<std::vector<CellType>> new_cells = cells;
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                new_cells[y][x] = updateCell(x, y, p, pstart, w_speed, w_direction);
            }
        }
        cells = new_cells;
    }

    CellType getCell(int x, int y) const {
        return cells[y][x];
    }

private:
    int size;
    std::vector<std::vector<CellType>> cells;

    CellType updateCell(int x, int y, float p, float pstart, float w_speed, float w_direction) {
        CellType current = cells[y][x];
        if (current == CellType::Burning) return CellType::Burned;
        if (current == CellType::Burned || current == CellType::Water) return current;

        for (const auto& neighbor : getNeighbors(x, y)) {
            if (cells[neighbor.second][neighbor.first] == CellType::Burning) {
                float angle = calculateAngle(x, y, neighbor.first, neighbor.second);
                float directional_influence = calculateDirectionalInfluence(w_direction, angle);
                float vegetation_factor = getVegetationFactor(current);
                // Implement probability calculation and fire spread logic here
                float adjusted_p = p * vegetation_factor * (1 + 0.1f * w_speed * directional_influence);

                if (static_cast<float>(rand()) / RAND_MAX < adjusted_p) {
                    return CellType::Burning;
                }
            }
        }

        if (static_cast<float>(rand()) / RAND_MAX < pstart) {
            return CellType::Burning;
        }

        return current;
    }

    std::vector<std::pair<int, int>> getNeighbors(int x, int y) {
        std::vector<std::pair<int, int>> neighbors;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
                    neighbors.emplace_back(nx, ny);
                }
            }
        }
        return neighbors;
    }

    float calculateAngle(int x1, int y1, int x2, int y2) {
        return std::atan2(y2 - y1, x2 - x1) * 180 / M_PI;
    }

    float calculateDirectionalInfluence(float w_direction, float angle) {
        float angle_diff = std::abs(w_direction - angle);
        angle_diff = std::min(angle_diff, 360.0f - angle_diff);
        return 1.0f - (angle_diff / 180.0f);
    }

    float getVegetationFactor(CellType type) {
        switch (type) {
            case CellType::NormalForest: return 1.0f;
            case CellType::DryGrass: return 1.5f;
            case CellType::DenseTrees: return 0.5f;
            case CellType::Water: return 0.0f;
            default: return 1.0f;
        }
    }
};

// Main application class
class ForestFireSim {
public:
    void getSimulationParameters() {
        std::cout << "Enter simulation parameters:\n";
        std::cout << "Probability of fire spread (0.0 - 1.0): ";
        std::cin >> params.p;
        std::cout << "Probability of spontaneous ignition (0.0 - 1.0): ";
        std::cin >> params.pstart;
        std::cout << "Wind speed (0.0 - 1.0): ";
        std::cin >> params.w_speed;
        std::cout << "Wind direction in degrees (0 - 360): ";
        std::cin >> params.w_direction;
        std::cout << "Water ratio (0.0 - 1.0): ";
        std::cin >> params.water_ratio;
    }

    ForestFireSim() : window(sf::VideoMode(800, 600), "Forest Fire Simulation"),
                      view(window.getDefaultView()),
                      grid(GRID_SIZE, params.water_ratio) {
        window.setView(view);
        getSimulationParameters();
        grid = Grid(GRID_SIZE, params.water_ratio);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

    void startSimulation() {
        std::cout << "Press Enter to start the simulation...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        run();
    }


private:
    sf::RenderWindow window;
    Grid grid;
    sf::View view;
    SimParams params;
    sf::Vector2f lastMousePos;
    float zoomLevel = 1.0f;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    lastMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2f newMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    sf::Vector2f deltaPos = lastMousePos - newMousePos;
                    view.move(deltaPos);
                    lastMousePos = newMousePos;
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0)
                    zoomLevel *= 1.1f;
                else if (event.mouseWheelScroll.delta < 0)
                    zoomLevel /= 1.1f;
                view.setSize(window.getDefaultView().getSize());
                view.zoom(zoomLevel);
            }
        }
    }

    void update() {
        grid.update(params.p, params.pstart, params.w_speed, params.w_direction);
    }

    void render() {
        window.clear(sf::Color::White);
        window.setView(view);

        // Draw grid
        for (int y = 0; y < GRID_SIZE; ++y) {
            for (int x = 0; x < GRID_SIZE; ++x) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor(getCellColor(grid.getCell(x, y)));
                window.draw(cell);
            }
        }

        window.display();
    }

    sf::Color getCellColor(CellType type) {
        switch (type) {
            case CellType::NormalForest: return sf::Color::Green;
            case CellType::DryGrass: return sf::Color::Yellow;
            case CellType::DenseTrees: return sf::Color(0, 100, 0); // Dark Green
            case CellType::Water: return sf::Color::Blue;
            case CellType::Burning: return sf::Color::Red;
            case CellType::Burned: return sf::Color::Black;
            default: return sf::Color::White;
        }
    }
};



int main() {
    srand(static_cast<unsigned int>(time(0)));
    ForestFireSim sim;
    sim.startSimulation();
    return 0;
}
