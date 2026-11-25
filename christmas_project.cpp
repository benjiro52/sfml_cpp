#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
using namespace std;

class Snowman {
    // secret
};

class Garland {
private:
    vector<sf::CircleShape> parts;
public:
    Garland(sf::Vector2f topCenter, int rows) {
        float radius = 6.f;

        for (int i = 0; i < rows; i++) {
            float width = 33.f + i * 33.f;         
            float y = topCenter.y + (i + 1) * 30.f;
            
            int count = static_cast<int>(width / 40.f) + 1;

            for (int j = 0; j < count; j++) {
                sf::CircleShape light(radius);
                light.setFillColor(sf::Color::White);
                light.setOrigin({radius, radius}); 

                float x = topCenter.x - width / 2.f + width * (j + 0.5f) / count;
                light.setPosition({x, y});
                parts.push_back(light);
            }
        
        }
    }

    void draw(sf::RenderWindow& window) {
        for (sf::CircleShape& p : parts) {
            window.draw(p);
        }
    }

    void rgb(float total_time_, float speed) {
        vector<sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Yellow,
            sf::Color::Blue,
            sf::Color::Cyan,
            sf::Color::White,
            sf::Color::Green,
            sf::Color::Magenta,
            sf::Color(255, 105, 180), 
            sf::Color(148, 0, 211)
        };

        int step = static_cast<int>(total_time_ * speed);
        for (size_t i = 0; i < parts.size(); i++) {
            parts[i].setFillColor(colors[(i + step) % colors.size()]);
            // parts[i].setFillColor(colors[(i * 3 + step) % colors.size()]);
        }
    }
};

class ChristmasTree{
private:
    vector<sf::RectangleShape> parts;
public:
    ChristmasTree(sf::Vector2f topCenter, int rows) {
        for (int i = 0; i < rows; i++) {
            float width = 33.f + i * 33.f;   // каждый ряд шире на 40
            sf::RectangleShape row({width, 30.f});
            row.setFillColor(sf::Color(0, 120, 40)); // темно зеленый 

            // В SFML позиция фигуры это её левый верхний угол. 
            // Чтобы ряд был по центру ёлки, сдвигаем его влево на половину его ширины. 
            // Чем шире ряд, тем дальше влево он начинается 
            // и поэтому все ряды остаются симметричными относительно одной вертикальной оси.
            row.setPosition({topCenter.x - width / 2.f, topCenter.y + i * 30.f});
            parts.push_back(row);
        }

        sf::RectangleShape trunk({50.f, 60.f}); // ствол
        trunk.setFillColor(sf::Color(100, 60, 20)); // коричневый 

        trunk.setPosition({topCenter.x - 25.f, topCenter.y + rows * 30.f});
        parts.push_back(trunk);
    }

    void draw(sf::RenderWindow& window) {
        for (sf::RectangleShape& p : parts) {
            window.draw(p);
        }
    }
};

class Snow {
private:
    sf::CircleShape circle;
    float time = 0.f;
    float swaySpeed;
    float swayAmount;

public:
    Snow(sf::Vector2f startPos) : circle({10.f}) {
        circle.setFillColor(sf::Color::White);
        circle.setPosition(startPos);

        // рандом в плюсах это смерть
        time = static_cast<float>(rand()) / RAND_MAX * 6.28f;
        swaySpeed = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;
        swayAmount = 20.f + static_cast<float>(rand()) / RAND_MAX * 40.f;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(circle);
    }

    void moveSnow(float speed_, float delta_time_) {
        time += delta_time_;

        //float distance = speed_ * delta_time_;
        float verticalSpeed = speed_;
        float horizontalSpeed = sin(time * swaySpeed) * swayAmount; // синусовая функция
        
        circle.move({ horizontalSpeed * delta_time_, verticalSpeed * delta_time_});
    }
};

void randomSpawn(vector<Snow>& snow_in_game) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distX(0, 1920);
    uniform_int_distribution<int> distY(0, 1);

    float x = static_cast<float>(distX(gen));
    float y = static_cast<float>(distY(gen));

    snow_in_game.push_back(Snow({x, y}));
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "christmas");
    float speed = 250.f;
    float total_time = 0.f;
    vector<Snow> snow_in_game;
    ChristmasTree tree({900.f, 400.f}, 11);
    Garland garland({900.f, 400.f}, 11);

    sf::Music music;
    if (!music.openFromFile("C:/Ivan/git/cpp_games/staff/Last_Christmas_music.mp3")) {
        return -1;
    }
    music.setLooping(true); // на повторе, вопрос как?
    music.setVolume(10.f); 
    music.play();

    sf::Texture background_texture;
    if (!background_texture.loadFromFile("C:/Ivan/git/cpp_games/staff/background_1.jpg")) {
        return -1;
    }
    sf::Sprite background(background_texture);

    sf::Clock delta_clock;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        float delta_time = delta_clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        window.draw(background);

        total_time += delta_time;
        garland.rgb(total_time, 3.f);
        tree.draw(window);
        garland.draw(window);

        // рисовка снега
        if (clock.getElapsedTime().asSeconds() >= 0.15f) {
            randomSpawn(snow_in_game);
            clock.restart();
        }
        for (Snow& i : snow_in_game) {
            i.draw(window);
            i.moveSnow(speed, delta_time);
        }

        window.display();
    }
}
// g++ christmas_project.cpp -o christmas_project.exe -IC:/msys64/ucrt64/include -LC:/msys64/ucrt64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
// ./christmas_project.exe