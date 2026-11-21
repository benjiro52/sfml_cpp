// Рождение: мёртвая клетка с ровно 3 живыми соседями оживает.
// Выживание: живая клетка с 2 или 3 живыми соседями остаётся живой.
// Смерть от одиночества: живая клетка с менее чем 2 соседями умирает.
// Смерть от перенаселения: живая клетка с более чем 3 соседями умирает.

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

// sf::Vertex - это точка с двумя основными свойствами: позиция (x, y) и цвет
// пример: sf::Vertex v({100.f, 50.f}, sf::Color::Red);
void print_grid(sf::RenderWindow& window, const vector<vector<int>>& grid, int cell) {
    sf::VertexArray triangles(sf::PrimitiveType::Triangles);

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == 1) {
                float px = x * cell;
                float py = y * cell;

                sf::Vector2f A(px, py);
                sf::Vector2f B(px + cell, py);
                sf::Vector2f C(px + cell, py + cell);
                sf::Vector2f D(px, py + cell);

                // первый треугольник
                triangles.append(sf::Vertex(A, sf::Color::Green)); // добавить вершину
                triangles.append(sf::Vertex(B, sf::Color::Green));
                triangles.append(sf::Vertex(C, sf::Color::Green));

                // второй треугольник
                triangles.append(sf::Vertex(A, sf::Color::Green));
                triangles.append(sf::Vertex(C, sf::Color::Green));
                triangles.append(sf::Vertex(D, sf::Color::Green));


            }
        }
    }
    window.draw(triangles);
}

void randomSpawn(vector<vector<int>>& grid)  {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distCols(0, grid[0].size() - 1);
    uniform_int_distribution<int> distRows(0, grid.size() - 1);

    for (int i = 0; i < 100; i++) {
        int d_cols = distCols(gen);
        int d_rows = distRows(gen);
        grid[d_rows][d_cols] = 1; 
    }
}

int countNeighbors(const vector<vector<int>>& grid, int x, int y) {
    int rows = grid.size();
    int cols = grid[0].size();
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) { // это сама клетка, ее скипнуть
                continue;
            }

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) { 
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

void algo(vector<vector<int>>& grid) {
    vector<vector<int>> next = grid;

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            int n = countNeighbors(grid, x, y);
            // Рождение
            if (grid[y][x] == 0 && n == 3) {
                next[y][x] = 1;
            }
            
            // Смерть от одиночества & Смерть от перенаселения
            if (grid[y][x] == 1 && n != 2 && n != 3) {
                next[y][x] = 0; 
            }
        }
    }
    grid = next;
}

int main() {
    const int cols = 80;
    const int rows = 60;
    const int cell = 10;

    sf::Clock clock;
    vector<vector<int>> grid(rows, vector<int>(cols, 0)); // 60 строк 80 рядов
    randomSpawn(grid);

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Life");
    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        if (clock.getElapsedTime().asMilliseconds() > 3000) { // 100 standart
            algo(grid);
            clock.restart();
        }

        window.clear(sf::Color::Black);
        print_grid(window, grid, cell);
        window.display();
    }
}

// am i stupid this time????
// g++ life.cpp -o life.exe -IC:/msys64/ucrt64/include -LC:/msys64/ucrt64/lib -lsfml-graphics -lsfml-window -lsfml-system
// i am tired