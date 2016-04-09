#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>

int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(60);

    int corb1_x, corb1_y, corb2_x, corb2_y;
    corb1_y = corb2_y = 50;
    corb1_x = 50;
    corb2_x = 980;

    sf::VertexArray lines(sf::Lines, 4);

    sf::CircleShape ball(25.f);
    ball.setFillColor(sf::Color::Green);

    sf::CircleShape control1(25.f);
    control1.setFillColor(sf::Color::Red);
    control1.setPosition(corb1_x, corb1_y);

    sf::CircleShape control2(25.f);
    control2.setFillColor(sf::Color::Red);
    control2.setPosition(corb2_x, corb2_y);

    float x, y, gravity_speed, x_speed, y_speed, pull_strength;
    bool on_ground;
    x = 515;
    y = 100;
    gravity_speed = 0.2f;
    x_speed = y_speed = 0.f;
    pull_strength = 0.5f;

    while (win.isOpen())
    {
        win.clear();
        sf::Event event;

        lines[0].position = sf::Vector2f(0, 0);
        lines[1].position = sf::Vector2f(0, 0);
        lines[2].position = sf::Vector2f(0, 0);
        lines[3].position = sf::Vector2f(0, 0);
        x_speed *= 0.95f;
        y_speed *= 0.95f;
        y_speed += gravity_speed;
        while (win.pollEvent(event)){if (event.type == sf::Event::Closed){win.close();}}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            x_speed += cos(atan(abs((y + 25) - (corb1_y + 25)) / abs((x + 25) - (corb1_x + 25)))) * -pull_strength;
            y_speed += sin(atan(abs((y + 25) - (corb1_y + 25)) / abs((x + 25) - (corb1_x + 25)))) * -pull_strength;
            lines[0].position = sf::Vector2f(corb1_x + 25, corb1_y + 25);
            lines[1].position = sf::Vector2f((x + 25), (y + 25));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            x_speed += cos(atan(abs((y + 25) - (corb1_y + 25)) / abs((x + 25) - (corb2_x + 25)))) * pull_strength;
            y_speed += sin(atan(abs((y + 25) - (corb2_y + 25)) / abs((x + 25) - (corb2_x + 25)))) * -pull_strength;
            lines[2].position = sf::Vector2f(corb2_x + 25, corb2_y + 25);
            lines[3].position = sf::Vector2f((x + 25), (y + 25));
        }

        y += y_speed;
        x += x_speed;
        if (y > (720 - 52)){on_ground = true; y_speed = 0;} else {on_ground = false;}
        if (y > (720 - 50)){y = 720 - 50;}
        if (x > (corb2_x - 5)){x = (corb2_x - 6);}
        if (x < (corb1_x + 5)){x = (corb1_x + 6);}
        if (y < (corb1_y + 5)){y = (corb1_y + 6);}

        ball.setPosition(x, y);
        win.draw(lines);
        win.draw(ball);
        win.draw(control1);
        win.draw(control2);
        win.display();
    }
    return 0;
}
