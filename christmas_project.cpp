#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
using namespace std;

class Snow {
private:
    sf::CircleShape circle;
public:
    Snow(sf::Vector2f startPos) : circle({10.f}) {
        circle.setFillColor(sf::Color::White);
        circle.setPosition(startPos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(circle);
    }

    void moveSnow(float speed_, float delta_time_) {
        float distance = speed_ * delta_time_;
        circle.move({0.f, distance});
    }
};

void randomSpawn(vector<Snow>& snow_in_game) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distX(0, 760);
    uniform_int_distribution<int> distY(0, 1);

    float x = static_cast<float>(distX(gen));
    float y = static_cast<float>(distY(gen));

    snow_in_game.push_back(Snow({x, y}));
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1980, 1080}), "christmas");
    sf::Music music;
    if (!music.openFromFile("C:/Ivan/git/cpp_games/music/Last_Christmas_music.mp3")) {
        cout << "No music found";
        return -1;
    }
    music.setLooping(true); // на повторе, вопрос как?
    music.setVolume(20.f); 
    music.play();

    vector<Snow> snow_in_game;

    sf::Clock delta_clock;
    sf::Clock clock;

    float speed = 300.f;

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        float delta_time = delta_clock.restart().asSeconds();
        window.clear(sf::Color::Black);
        if (clock.getElapsedTime().asSeconds() >= 0.2f) {
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