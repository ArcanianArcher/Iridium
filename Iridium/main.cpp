#include <SFML/Graphics.hpp>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()



class Level
{
public:
    sf::Image level_image;
    sf::Texture level_texture;
    sf::Sprite level_sprite;
    sf::Color black = sf::Color::Black;
    Level(int level_number)
    {
        // constructor
        std::string file_path = "./levels/testlevel" + SSTR(level_number) + ".png";
        level_texture.loadFromFile(file_path);
        level_image.loadFromFile(file_path);
        level_sprite.setTexture(level_texture, true);
        std::cout << "new level: " << level_number << std::endl;
    };

    int IsColliding(int x, int y, int r, int p)
    {

        int x_points[p];
        int y_points[p];
        std::cout << "center (x,y): ("<<x<<", "<<y<<")"<<std::endl;
        for (int i = 0; i < p; i++)
        {
            x_points[i] = abs(x + (r * cos(((2 * i) * M_PI) / p)));
            y_points[i] = abs(y + (r * sin(((2 * i) * M_PI) / p)));
            std::cout << "x, y points[" <<i<<"] (x,y): ("<<x_points[i]<<", "<<y_points[i]<<")"<<std::endl;
        }
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;

        for (int i = 0; i < p; i++)
        {
            if (level_image.getPixel(abs(x + (r * cos(((2 * i) * M_PI) / p))), abs(y + (r * sin(((2 * i) * M_PI) / p)))) == black)
                //{return i;} //live code
                {std::cout<<"colliding: "<<i<<std::endl; return i;}else{std::cout<<std::endl;} //testing purposess
        }
        return -1;
    };
    sf::Sprite GetSprite(){return level_sprite;}
};


int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(5);
    win.setKeyRepeatEnabled(false);

    int current_level_num = 1;
    Level* level = new Level(current_level_num);

    bool keyState[sf::Keyboard::KeyCount];
    int is_collided;
    for (int i = 0; i < (sf::Keyboard::KeyCount); i++){keyState[i] = false;}
    int corb1_x, corb1_y, corb2_x, corb2_y, ball_r, corb_r, detection_points;
    float friction_constant;
    corb1_y = corb2_y = 30;
    corb1_x = 30;
    corb2_x = 1000;
    ball_r = 25;
    corb_r = 25;
    friction_constant = 0.7f;
    detection_points = 8;

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
                    if (event.key.code == sf::Keyboard::Space && !keyState[event.key.code]) {pushpull *= -1; current_level_num++; level = new Level(current_level_num);}
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
        is_collided = level->IsColliding(x, y, ball_r, detection_points);
        if (is_collided != -1)
        {
            if (is_collided == 0 || is_collided == 4)
            {
                x_speed *= -1; x += x_speed; x_speed *= friction_constant; y_speed *= friction_constant;
            }
            else if (is_collided == 2 || is_collided == 6)
            {
                y_speed *= -1; y += y_speed; x_speed *= friction_constant; y_speed *= friction_constant;
            }
            else
            {
                x_speed *= -1; x += x_speed; y_speed *= -1; y += y_speed; x_speed *= friction_constant; y_speed *= friction_constant;
            }
        }
        if (x > (corb2_x - 5)){x = (corb2_x - 6); x_speed = 0;}
        if (x < (corb1_x + 5)){x = (corb1_x + 6); x_speed = 0;}
        if (y < (corb1_y + 5)){y = (corb1_y + 6); y_speed = 0;}
        if (y > (720 - (ball_r * 2))){y = (720 - ((ball_r * 2) + 1)); y_speed = 0; x_speed *= 0.9f;}

        ball.setPosition(x, y);
        win.draw(level->GetSprite());
        win.draw(lines);
        win.draw(ball);
        win.draw(control1);
        win.draw(control2);
        win.display();
    }
    return 0;
}
