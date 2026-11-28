#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
using namespace std;

class Snowman {
private:
    vector<sf::CircleShape> parts_circles;
    vector<sf::RectangleShape> parts_rectangles;
    size_t visible_circles = 0;
    size_t visible_rectangles = 0;

    void addCircle(float r, sf::Vector2f center, sf::Color color) {
        sf::CircleShape c(r);
        c.setFillColor(color);
        c.setPosition({center.x - r, center.y - r});   // центр -> левый верхний угол
        parts_circles.push_back(c);
    }

    void addStick(sf::Vector2f start, float angle, sf::Color color) {
        sf::RectangleShape stick({80.f, 6.f});
        stick.setFillColor(color);
        stick.setPosition(start);
        stick.setRotation(sf::degrees(angle));
        parts_rectangles.push_back(stick);
    }

public:
    Snowman(sf::Vector2f base) {
        vector<float> radii = {60.f, 40.f, 20.f};
        float bottom = base.y;
        float bodyY = 0.f;
        float headY = 0.f;

        for (size_t i = 0; i < radii.size(); i++) {
            float r = radii[i];
            float centerY = bottom - r;
            addCircle(r, {base.x, centerY}, sf::Color::White);
            bottom = centerY - r + 2.f;

            if (i == 1) bodyY = centerY;
            headY = centerY;
        }

        addCircle(3.f, {base.x - 8.f, headY - 9.f}, sf::Color::Black);
        addCircle(3.f, {base.x + 8.f, headY - 9.f}, sf::Color::Black);
        addCircle(4.f, {base.x, headY}, sf::Color(255, 140, 0));

        sf::Color brown(100, 60, 20);
        addStick({base.x, bodyY}, 210.f, brown);
        addStick({base.x, bodyY}, -30.f, brown);
    }

    void addNextCircle() {
        if (visible_circles < parts_circles.size()) visible_circles++;
    }

    void addNextRectangle() {
        if (visible_rectangles < parts_rectangles.size()) visible_rectangles++;
    }

    void drawCircles(sf::RenderWindow& window) {
        for (size_t i = 0; i < visible_circles; i++) {
            window.draw(parts_circles[i]);
        }
    }

    void drawRectangles(sf::RenderWindow& window) {
        for (size_t i = 0; i < visible_rectangles; i++) {
            window.draw(parts_rectangles[i]);
        }
    }
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

    float getY() { 
        return circle.getPosition().y; 
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
    srand(static_cast<unsigned>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "christmas");
    float speed = 250.f;
    float total_time = 0.f;
    int count_clicks = 0;
    vector<Snow> snow_in_game;

    ChristmasTree tree({900.f, 400.f}, 11);
    Garland garland({900.f, 400.f}, 11);
    Snowman snowman({1360.f, 790.f});

    sf::Music music1;
    if (!music1.openFromFile("C:/Ivan/git/cpp_games/staff/Last_Christmas_music.mp3")) return -1;
    sf::Music music2;
    if (!music2.openFromFile("C:/Ivan/git/cpp_games/staff/Snowman_music.mp3")) return -1;
    sf::Music music3;
    if (!music3.openFromFile("C:/Ivan/git/cpp_games/staff/Jingle_Bell_Rock.mp3")) return -1;

    //ласт кристмас на дефолте
    music1.setLooping(true);
    music1.setVolume(10.f); 
    music1.play();

    sf::Texture background_texture;
    if (!background_texture.loadFromFile("C:/Ivan/git/cpp_games/staff/background_1.jpg")) return -1;
    sf::Sprite background(background_texture);

    sf::Clock delta_clock;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    count_clicks++;
                    if (count_clicks >= 7 && count_clicks <= 8) {
                        snowman.addNextRectangle();
                    } else {
                        snowman.addNextCircle();
                    }
                }
            }
            // музло
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Num1) {
                    music1.stop();
                    music2.stop();
                    music3.stop();

                    music1.setLooping(true);
                    music1.setVolume(10.f); 
                    music1.play();
                }
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Num2) {
                    music1.stop();
                    music2.stop();
                    music3.stop();

                    music2.setLooping(true);
                    music2.setVolume(10.f); 
                    music2.play();
                }
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Num3) {
                    music1.stop();
                    music2.stop();
                    music3.stop();

                    music3.setLooping(true);
                    music3.setVolume(10.f); 
                    music3.play();
                }
            }
        }
        float delta_time = delta_clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        window.draw(background);

        total_time += delta_time;
        garland.rgb(total_time, 3.f);

        tree.draw(window);
        garland.draw(window);
        snowman.drawRectangles(window);
        snowman.drawCircles(window); 

        // рисовка снега
        if (clock.getElapsedTime().asSeconds() >= 0.15f) {
            randomSpawn(snow_in_game);
            clock.restart();
        }
        for (Snow& i : snow_in_game) {
            i.draw(window);
            i.moveSnow(speed, delta_time);
        }

        // удаление снежинок(оптимизация)
        for (size_t i = 0; i < snow_in_game.size(); i++) { 
            if (snow_in_game[i].getY() > 1080.f) {
                snow_in_game.erase(snow_in_game.begin() + i);
                i--;
            }
        }

        window.display();
    }
}
// g++ christmas_project.cpp -o christmas_project.exe -IC:/msys64/ucrt64/include -LC:/msys64/ucrt64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
// ./christmas_project.exe