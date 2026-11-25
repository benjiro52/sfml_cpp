#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
using namespace std;

class ChristmasTree{
private:
    
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
    vector<Snow> snow_in_game;
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