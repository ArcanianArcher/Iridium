#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>

int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(60);
    win.setKeyRepeatEnabled(false);

    bool keyState[sf::Keyboard::KeyCount];
    for (int i = 0; i < (sf::Keyboard::KeyCount); i++)
    {
        keyState[i] = true;
    }

    int corb1_x, corb1_y, corb2_x, corb2_y;
    corb1_y = corb2_y = 30;
    corb1_x = 30;
    corb2_x = 1000;

    sf::VertexArray lines(sf::Lines, 4);

    sf::CircleShape ball(25);
    ball.setFillColor(sf::Color::Green);

    sf::CircleShape control1(25);
    control1.setFillColor(sf::Color::Red);
    control1.setPosition(corb1_x, corb1_y);

    sf::CircleShape control2(25);
    control2.setFillColor(sf::Color::Red);
    control2.setPosition(corb2_x, corb2_y);

    float x, y, gravity, x_speed, y_speed, pull_strength;
    bool on_ground;
    int pushpull;
    x = 515;
    y = 100;
    gravity = 0.2f;
    x_speed = y_speed = 0.f;
    pull_strength = 0.5f;
    pushpull = -1;                          // pushpull is 1 when pushpulling, -1 when repelling
    sf::Event event;
    win.setKeyRepeatEnabled(false);
    while (win.isOpen())
    {


        win.clear();
        lines[0].position = sf::Vector2f(0, 0);
        lines[1].position = sf::Vector2f(0, 0);
        lines[2].position = sf::Vector2f(0, 0);
        lines[3].position = sf::Vector2f(0, 0);
        x_speed *= 0.995f;
        y_speed *= 0.98f;
        y_speed += gravity;
        std::cout << pushpull << std::endl;
        while (win.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    win.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {win.close();}
                    if (event.key.code == sf::Keyboard::Space && !keyState[event.key.code]) {pushpull *= -1;}
                    keyState[event.key.code] = true;
                    break;
                case sf::Event::KeyReleased:
                    keyState[event.key.code] = false;
                    break;
                default:
                    break;
                }
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (corb1_x >= x){x_speed += (pull_strength * pushpull);}
            if (corb1_x <  x){x_speed -= (pull_strength * pushpull);}
            if (corb1_y <  y){y_speed -= (pull_strength * pushpull);}
            if (corb1_y >= y){y_speed += (pull_strength * pushpull);}
            lines[0].position = sf::Vector2f(corb1_x + 25, corb1_y + 25);
            lines[1].position = sf::Vector2f((x + 25), (y + 25));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (corb2_x >= x){x_speed += (pull_strength * pushpull);}
            if (corb2_x <  x){x_speed -= (pull_strength * pushpull);}
            if (corb2_y <  y){y_speed -= (pull_strength * pushpull);}
            if (corb2_y >= y){y_speed += (pull_strength * pushpull);}
            lines[2].position = sf::Vector2f(corb2_x + 25, corb2_y + 25);
            lines[3].position = sf::Vector2f((x + 25), (y + 25));
        }

        y += y_speed;
        x += x_speed;
        if (y > (720 - 52)){on_ground = true; y_speed = 0;} else {on_ground = false;}
        if (y > (720 - 50)){y = 720 - 50;}
        if (x > (corb2_x - 5)){x = (corb2_x - 6); x_speed = 0;}
        if (x < (corb1_x + 5)){x = (corb1_x + 6); x_speed = 0;}
        if (y < (corb1_y + 5)){y = (corb1_y + 6); y_speed = 0;}

        ball.setPosition(x, y);
        win.draw(lines);
        win.draw(ball);
        win.draw(control1);
        win.draw(control2);
        win.display();
        win.setFramerateLimit(80);
    }
    return 0;
}
