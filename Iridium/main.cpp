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
    int control_radius = 25;
    sf::CircleShape control1;
    sf::CircleShape control2;

    Level(int level_number, int corb1_x, int corb1_y, int corb2_x, int corb2_y, sf::Color colour)
    {
        // constructor
        std::string file_path = "./levels/TL" + SSTR(level_number) + ".png";
        level_texture.loadFromFile(file_path);
        level_image.loadFromFile(file_path);
        level_sprite.setTexture(level_texture, true);
        control1.setRadius(control_radius);
        control2.setRadius(control_radius);
        control1.setPosition(corb1_x, corb1_y);
        control2.setPosition(corb2_x, corb2_y);
        control1.setFillColor(colour);
        control2.setFillColor(colour);

        std::cout << "new level: " << level_number << std::endl;
    };

    void SetControlOrb1Position(int corb_x, int corb_y){control1.setPosition(corb_x, corb_y);}
    void SetControlOrb2Position(int corb_x, int corb_y){control2.setPosition(corb_x, corb_y);}
    void SetControlOrb1Colour(sf::Color colour){control1.setFillColor(colour);}
    void SetControlOrb2Colour(sf::Color colour){control2.setFillColor(colour);}
    sf::Vector2f GetControlOrb1Position(void){return control1.getPosition();}
    sf::Vector2f GetControlOrb2Position(void){return control2.getPosition();}
    void CollisionPoints(int* points, int x, int y, int r, int p)

    {
        int num_points = 0;
        for (int i = 0; i < p; i++)
        {
            if (level_image.getPixel(abs((x + r) + (r * cos(((2 * i) * M_PI) / p))), abs((y + r) + (r * sin(((2 * i) * M_PI) / p)))) == black)
                {
                    num_points++;
                    points[num_points] = i;
                }
        }
        points[0] = num_points;
    }
    sf::Sprite GetSprite(){return level_sprite;}
};



sf::Vector2f AddVectors(int *points, int total_detection_points, double total_speed)
{
    sf::Vector2f vector_total(0.f, 0.f);
    for (int i = 0; i < points[0]; i++)
    {
        float angle = (((2 * points[i + 1]) * M_PI) / total_detection_points);
        sf::Vector2f v(-(cos(angle)), -(sin(angle)));
        vector_total += v;
    }
    float length = sqrt((vector_total.x * vector_total.x) + (vector_total.y * vector_total.y));
    sf::Vector2f u((vector_total.x / length) * total_speed, (vector_total.y / length) * total_speed);
    return u;
}

int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(80);
    win.setKeyRepeatEnabled(false);

    int current_level_num = 0;
    Level* level = new Level(current_level_num, 30, 30, 1000, 30, sf::Color::Red);

    bool keyState[sf::Keyboard::KeyCount];
    for (int i = 0; i < (sf::Keyboard::KeyCount); i++){keyState[i] = false;}
    int ball_r;
    const int detection_points = 30;
    int detected_points[detection_points + 1];
    float i_friction_constant;
    ball_r = 25;
    i_friction_constant = 0.3f;
    sf::VertexArray lines(sf::Lines, 4);
    sf::CircleShape ball(ball_r);
    ball.setFillColor(sf::Color::Green);

    float x, y, prev_x, prev_y, gravity, x_speed, y_speed, pull_strength, pull_constant;
    bool pushpull;
    x = 515;
    y = 100;
    gravity = 0.1f;
    x_speed = y_speed = 0.f;
    pull_constant = 0.0005f;
    pushpull = true;                          // pushpull is true when pulling, false when repelling
    sf::Event event;
    win.setKeyRepeatEnabled(false);
    while (win.isOpen())
    {
        win.clear();
        lines[0].position = sf::Vector2f(0, 0); lines[0].color = sf::Color::Blue;
        lines[1].position = sf::Vector2f(0, 0); lines[1].color = sf::Color::Blue;
        lines[2].position = sf::Vector2f(0, 0); lines[2].color = sf::Color::Blue;
        lines[3].position = sf::Vector2f(0, 0); lines[3].color = sf::Color::Blue;
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
                    if (event.key.code == sf::Keyboard::Space && !keyState[event.key.code]) {pushpull = !pushpull;}
                    if (event.key.code == sf::Keyboard::R && !keyState[event.key.code]) {current_level_num++; level = new Level(current_level_num, 30, 30, 1000, 30, sf::Color::Red);}
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
            pull_strength = sqrt(pow(fabs(((y + ball_r) - (level -> GetControlOrb1Position().y + level -> control_radius))), 2) + pow(fabs(((x + level -> control_radius) - (level -> GetControlOrb1Position().x + level -> control_radius))), 2));
            if (pushpull == true)
            {
                x_speed += (cos(atan(fabs(((y + ball_r) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * -((pull_strength * pull_constant));
                y_speed += (sin(atan(fabs(((y + ball_r) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * -((pull_strength * (pull_constant / 0.5f)));
            }else{
                x_speed += (cos(atan(fabs(((y + ball_r) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / -((pull_strength * pull_constant) * -1));
                y_speed += (sin(atan(fabs(((y + ball_r) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / -((pull_strength * pull_constant / 0.5f) * -1));
            }
            lines[0].position = sf::Vector2f(level -> GetControlOrb1Position().x + level -> control_radius, level -> GetControlOrb1Position().y + level -> control_radius);
            lines[1].position = sf::Vector2f((x + ball_r), (y + ball_r));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            pull_strength = sqrt(pow(fabs(((y + ball_r) - (level -> GetControlOrb2Position().y + level -> control_radius))), 2) + pow(fabs(((x + level -> control_radius) - (level -> GetControlOrb2Position().x + level -> control_radius))), 2));
            if (pushpull == true)
            {
                x_speed += (cos(atan(fabs(((y + ball_r) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * ((pull_strength * pull_constant));
                y_speed += (sin(atan(fabs(((y + ball_r) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * -((pull_strength * (pull_constant / 0.5f)));
            }else{
                x_speed += (cos(atan(fabs(((y + ball_r) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / ((pull_strength * pull_constant) * -1));
                y_speed += (sin(atan(fabs(((y + ball_r) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + ball_r) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / -((pull_strength * (pull_constant / 0.5f)) * -1));
            }
            lines[2].position = sf::Vector2f(level -> GetControlOrb2Position().x + level -> control_radius, level -> GetControlOrb2Position().y + level -> control_radius);
            lines[3].position = sf::Vector2f((x + ball_r), (y + ball_r));
        }

        prev_x = x;
        prev_y = y;
        y += y_speed;
        x += x_speed;

        level->CollisionPoints(detected_points, x, y, ball_r, detection_points);
        if (detected_points[0] != 0)
        {
            sf::Vector2f col_tangent = AddVectors(detected_points, detection_points, (sqrt((x_speed * x_speed) + (y_speed * y_speed))));
            x_speed += (col_tangent.x) * i_friction_constant;
            y_speed += (col_tangent.y) * i_friction_constant;
            x = prev_x;
            y = prev_y;
        }
        ball.setPosition(x, y);
        win.draw(level->GetSprite());
        win.draw(lines);
        win.draw(ball);
        win.draw(level -> control1);
        win.draw(level -> control2);
        win.display();
    }
    return 0;
}
