#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class Level
{
public:
    sf::Image level_image;
    sf::Texture level_texture;
    sf::Sprite level_sprite;
    Level(int level_number)
    {
        // constructor
        std::string base_path = "C:/Users/Kian Shepherd/Desktop/Iridium/Iridium/levels/testlevel";
        std::string file_number = SSTR(level_number);
        std::string file_ext = ".jpg";
        std::string file_path = base_path + file_number + file_ext;
        level_texture.loadFromFile(file_path);
        level_sprite.setTexture(level_texture, true);
    };

    bool IsColliding(int x, int y, int r)
    {

    };
    sf::Sprite GetImage()
    {
        return level_sprite;
    }
};


int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(60);
    win.setKeyRepeatEnabled(false);

    Level* level = new Level(1);

    bool keyState[sf::Keyboard::KeyCount];
    for (int i = 0; i < (sf::Keyboard::KeyCount); i++){keyState[i] = false;}
    int corb1_x, corb1_y, corb2_x, corb2_y, ball_r, corb_r;
    corb1_y = corb2_y = 30;
    corb1_x = 30;
    corb2_x = 1000;
    ball_r = 25;
    corb_r = 25;

    sf::VertexArray lines(sf::Lines, 4);

    sf::CircleShape ball(ball_r);
    ball.setFillColor(sf::Color::Green);

    sf::CircleShape control1(corb_r);
    control1.setFillColor(sf::Color::Red);
    control1.setPosition(corb1_x, corb1_y);

    sf::CircleShape control2(corb_r);
    control2.setFillColor(sf::Color::Red);
    control2.setPosition(corb2_x, corb2_y);

    float x, y, gravity, x_speed, y_speed, pull_strength, pull_constant;
    int pushpull;
    x = 515;
    y = 100;
    gravity = 0.2f;
    x_speed = y_speed = 0.f;
    pull_constant = 0.0009f;
    pushpull = 1;                          // pushpull is 1 when pulling, -1 when repelling
    sf::Event event;
    win.setKeyRepeatEnabled(false);
    while (win.isOpen())
    {
        win.clear();
        lines[0].position = sf::Vector2f(0, 0);
        lines[1].position = sf::Vector2f(0, 0);
        lines[2].position = sf::Vector2f(0, 0);
        lines[3].position = sf::Vector2f(0, 0);
        y_speed += gravity;
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
            pull_strength = sqrt(pow(fabs(((y + ball_r) - (corb1_y + corb_r))), 2) + pow(fabs(((x + corb_r) - (corb1_x + corb_r))), 2));
            x_speed += (cos(atan(fabs(((y + ball_r) - (corb1_y + corb_r))) / fabs(((x + ball_r) - (corb1_x + corb_r)))))) * -((pull_strength * pull_constant) * pushpull);
            y_speed += (sin(atan(fabs(((y + ball_r) - (corb1_y + corb_r))) / fabs(((x + ball_r) - (corb1_x + corb_r)))))) * -((pull_strength * (pull_constant / 0.5f)) * pushpull);

            lines[0].position = sf::Vector2f(corb1_x + corb_r, corb1_y + corb_r);
            lines[1].position = sf::Vector2f((x + ball_r), (y + ball_r));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            pull_strength = sqrt(pow(fabs(((y + ball_r) - (corb2_y + corb_r))), 2) + pow(fabs(((x + corb_r) - (corb2_x + corb_r))), 2));
            x_speed += (cos(atan(fabs(((y + ball_r) - (corb2_y + corb_r))) / fabs(((x + ball_r) - (corb2_x + corb_r)))))) * ((pull_strength * pull_constant) * pushpull);
            y_speed += (sin(atan(fabs(((y + ball_r) - (corb2_y + corb_r))) / fabs(((x + ball_r) - (corb2_x + corb_r)))))) * -((pull_strength * (pull_constant / 0.5f)) * pushpull);

            lines[2].position = sf::Vector2f(corb2_x + corb_r, corb2_y + corb_r);
            lines[3].position = sf::Vector2f((x + ball_r), (y + ball_r));
        }

        y += y_speed;
        x += x_speed;

        if (x > (corb2_x - 5)){x = (corb2_x - 6); x_speed = 0;}
        if (x < (corb1_x + 5)){x = (corb1_x + 6); x_speed = 0;}
        if (y < (corb1_y + 5)){y = (corb1_y + 6); y_speed = 0;}
        if (y > (720 - (ball_r * 2))){y = (720 - ((ball_r * 2) + 1)); y_speed = 0; x_speed *= 0.9f;}

        ball.setPosition(x, y);
        win.draw(level->GetImage());
        win.draw(lines);
        win.draw(ball);
        win.draw(control1);
        win.draw(control2);
        win.display();
        win.setFramerateLimit(80);
    }
    return 0;
}
