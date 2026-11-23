#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "christmas");
    sf::Music music;
    if (!music.openFromFile("C:/Ivan/git/cpp_games/music/Last_Christmas_music.mp3")) {
        cout << "No music found";
        return -1;
    }

    music.setLooping(true);
    music.setVolume(20.f);
    music.play();

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
         
        window.clear(sf::Color::Black);
        window.display();
    }
   
}
// g++ christmas_project_beta.cpp -o christmas_project_beta.exe -IC:/msys64/ucrt64/include -LC:/msys64/ucrt64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio