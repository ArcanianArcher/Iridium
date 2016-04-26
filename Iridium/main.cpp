#include <SFML/Graphics.hpp>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const double PI = 3.141592653589793238463;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

sf::Vector2f AddVectors(int *points, int total_detection_points, double total_speed)
{
    sf::Vector2f vector_total(0.f, 0.f);
    for (int i = 0; i < points[0]; i++)
    {
        float angle = (((2 * points[i + 1]) * PI) / total_detection_points);
        sf::Vector2f v(-(cos(angle)), -(sin(angle)));
        vector_total += v;
    }
    float length = sqrt((vector_total.x * vector_total.x) + (vector_total.y * vector_total.y));
    sf::Vector2f u((vector_total.x / length) * total_speed, (vector_total.y / length) * total_speed);
    return u;
}

class LevelData
{
public:
    /*
    Level
    ballx, bally, ballr, corb1x, corb1y, corb2x, corb2y, corbr
    #collectibles, coll1x, coll1y, ...
    #enemys, enemy1x1, enemy1y1, enemy1x2, enemy1y2, ...
    #portals, portala1x, portala1y, portala1exitx, portala1exity, ...
    */

    std::vector<std::vector<int>> Levels;

    LevelData()
    {
        Levels.resize(50);
        Levels[0] = {180, 180, 15, 30, 30, 1000, 30, 30, 0, 0, 0, 0, 0, 0};
    }

};

class Level
{
public:

    sf::Image level_image;
    sf::Texture level_texture;
    sf::Sprite level_sprite;
    sf::CircleShape control1;
    sf::CircleShape control2;
    int control_radius;// = 25;
    LevelData* leveldata;
    int ballx;
    int bally;
    int ballr;
    int numCollectables, numEnemys, numPortals;
    Level(int level_number, sf::Color colour)
    {
        // constructor
        leveldata = new LevelData();
        std::string file_path = "./levels/M" + SSTR(level_number + 1) + ".png";
        level_texture.loadFromFile(file_path);
        level_image.loadFromFile(file_path);
        level_sprite.setTexture(level_texture, true);
        control_radius = leveldata->Levels[level_number][7];
        ballx = leveldata->Levels[level_number][0];
        bally = leveldata->Levels[level_number][1];
        ballr = leveldata->Levels[level_number][2];
        control1.setRadius(leveldata->Levels[level_number][7]);
        control2.setRadius(leveldata->Levels[level_number][7]);
        control1.setPosition(leveldata->Levels[level_number][3], leveldata->Levels[level_number][4]);
        control2.setPosition(leveldata->Levels[level_number][5], leveldata->Levels[level_number][6]);
        /*numCollectables = levelData[8];
        for (int i = 0; i < numCollectables; i++)
        {
            break;
        }
        numEnemys = levelData[10];
        for (int i = 0; i < numEnemys; i++)
        {
            break;
        }
        numPortals = levelData[12];
        for (int i = 0; i < numPortals; i++)
        {
            break;
        }
        */
        control1.setFillColor(colour);
        control2.setFillColor(colour);

        std::cout << "new level: " << level_number << std::endl;
        std::cout << ballx << bally << ballr << std::endl;
    };

    int GetBallX(){return (level_image.getPixel(0, 0).r + level_image.getPixel(0, 0).g + level_image.getPixel(0, 0).b);}// + level_image.getPixel(1, 1).a);}
    int GetBallY(){return (level_image.getPixel(1, 0).r + level_image.getPixel(1, 0).g + level_image.getPixel(1, 0).b);}
    int GetBallR(){return (level_image.getPixel(2, 0).r);}
    void SetControlOrb1Colour(sf::Color colour){control1.setFillColor(colour);}
    void SetControlOrb2Colour(sf::Color colour){control2.setFillColor(colour);}
    sf::Vector2f GetControlOrb1Position(void){return control1.getPosition();}
    sf::Vector2f GetControlOrb2Position(void){return control2.getPosition();}
    sf::Sprite GetSprite(){return level_sprite;}

    bool CheckEnding(int x, int y, int r)
    {
        if (level_image.getPixel((x + r), (y + r)) == sf::Color(128,0,128))
        {
            return true;
        }else{
            return false;
        }
    }

    void CollisionPoints(int *points, int x, int y, int r, int p)
    {
        int num_points = 0;
        sf::Color PointColour;
        for (int i = 0; i < p; i++)
        {
            PointColour = (level_image.getPixel(abs((x + r) + (r * cos(((2 * i) * PI) / p))), abs((y + r) + (r * sin(((2 * i) * PI) / p)))));
            if (PointColour == sf::Color::Black)
            {
                num_points++;
                points[num_points] = i;
            }
            if (PointColour == sf::Color::Red)
            {
                //std::cout << "Hello\n";
                num_points = -1;
                points[0] = num_points;
                return;
            }
        }
        points[0] = num_points;
    }
};

class Game
{
public:
    int current_level_num;
    int Quadrant_m1 = 1;
    int Quadrant_m2 = 1;
    int detection_points = 36;
    int detected_points[36 + 1];
    float x, y, prev_x, prev_y, gravity, x_speed, y_speed, pull_strength, pull_constant, i_friction_constant;
    bool pushpull = true;
    sf::VertexArray lines;
    sf::CircleShape ball;
    Level* level;

    Game(sf::VertexArray line)
    {
        current_level_num = 0;
        level = new Level(current_level_num, sf::Color::Red);
        x_speed = y_speed = 0.f;
        gravity = 0.1f;
        i_friction_constant = 0.3f;
        pull_constant = 0.0005f;
        lines = line;
        ball.setFillColor(sf::Color::Green);
        x = level->bally;
        y = level->ballx;
        ball.setRadius(level->ballr);
    }

    void ResetLines()
    {
        lines[0].position = sf::Vector2f(0, 0); lines[0].color = sf::Color::Blue;
        lines[1].position = sf::Vector2f(0, 0); lines[1].color = sf::Color::Blue;
        lines[2].position = sf::Vector2f(0, 0); lines[2].color = sf::Color::Blue;
        lines[3].position = sf::Vector2f(0, 0); lines[3].color = sf::Color::Blue;
    }

    void RightOrb()
    {
        if ((x - level -> GetControlOrb2Position().x > 0) && (y - level -> GetControlOrb2Position().y <= 0)) {Quadrant_m1 = -1; Quadrant_m2 = 1;}       // Q1
            else if ((x - level -> GetControlOrb2Position().x <= 0) && (y - level -> GetControlOrb2Position().y < 0)) {Quadrant_m1 = 1; Quadrant_m2 = 1;}   // Q2
            else if ((x - level -> GetControlOrb2Position().x < 0) && (y - level -> GetControlOrb2Position().y >= 0)) {Quadrant_m1 = 1; Quadrant_m2 = -1;}  // Q3
            else {Quadrant_m1 = -1; Quadrant_m2 = -1;}                                                                                                      // Q4
            pull_strength = (sqrt(pow(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))), 2) + pow(fabs(((x + level -> control_radius) - (level -> GetControlOrb2Position().x + level -> control_radius))), 2))) * 0.9f;
            if (pushpull == true)
            {
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * Quadrant_m1 * ((pull_strength * pull_constant));
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * Quadrant_m2 * ((pull_strength * (pull_constant / 0.5f)));
            }else{
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / Quadrant_m1 * ((pull_strength * pull_constant) * -1)) * 0.5f;
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / Quadrant_m2 * ((pull_strength * pull_constant / 0.5f) * -1)) * 0.5f;
            }
            lines[2].position = sf::Vector2f(level -> GetControlOrb2Position().x + level -> control_radius, level -> GetControlOrb2Position().y + level -> control_radius);
            lines[3].position = sf::Vector2f((x + ball.getRadius()), (y + ball.getRadius()));
    }

    void LeftOrb()
    {
        if ((x - level -> GetControlOrb1Position().x > 0) && (y - level -> GetControlOrb1Position().y <= 0)) {Quadrant_m1 = -1; Quadrant_m2 = 1;}       // Q1
            else if ((x - level -> GetControlOrb1Position().x <= 0) && (y - level -> GetControlOrb1Position().y < 0)) {Quadrant_m1 = 1; Quadrant_m2 = 1;}   // Q2
            else if ((x - level -> GetControlOrb1Position().x < 0) && (y - level -> GetControlOrb1Position().y >= 0)) {Quadrant_m1 = 1; Quadrant_m2 = -1;}  // Q3
            else {Quadrant_m1 = -1; Quadrant_m2 = -1;}                                                                                                      // Q4
            pull_strength = (sqrt(pow(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))), 2) + pow(fabs(((x + level -> control_radius) - (level -> GetControlOrb1Position().x + level -> control_radius))), 2))) * 0.9f;
            if (pushpull == true)
            {
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * Quadrant_m1 * ((pull_strength * pull_constant));
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * Quadrant_m2 * ((pull_strength * (pull_constant / 0.5f)));
            }else{
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / Quadrant_m1 * ((pull_strength * pull_constant) * -1)) * 0.5f;
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / Quadrant_m2 * ((pull_strength * pull_constant / 0.5f) * -1)) * 0.5f;
            }
            lines[0].position = sf::Vector2f(level -> GetControlOrb1Position().x + level -> control_radius, level -> GetControlOrb1Position().y + level -> control_radius);
            lines[1].position = sf::Vector2f((x + ball.getRadius()), (y + ball.getRadius()));
    }

    void SwitchPushPull()
    {
        pushpull = !pushpull;
    }

    void Movement()
    {
        y_speed += gravity;
        prev_x = x;
        prev_y = y;
        y += y_speed;
        x += x_speed;

        level->CollisionPoints(detected_points, x, y, ball.getRadius(), detection_points);
        if (detected_points[0] != 0)
        {
            if (detected_points[0] > 0)
            {
                sf::Vector2f col_tangent = AddVectors(detected_points, detection_points, (sqrt((x_speed * x_speed) + (y_speed * y_speed))));
                x_speed += (col_tangent.x) * i_friction_constant;
                y_speed += (col_tangent.y) * i_friction_constant;
                x += x_speed;
                y += y_speed;
                x = prev_x;
                y = prev_y;
            }
            else // collided with obstacle
            {
                x = level->GetBallX();
                y = level->GetBallY();
                x_speed = 0;
                y_speed = 0;
            }
        }
        if (level->CheckEnding(x, y, level -> ballr))
        {
            current_level_num++;
            level = new Level(current_level_num, sf::Color::Red);
            x = level->ballx;
            y = level->bally;
            ball.setRadius(level->ballr);

        }
        ball.setPosition(x, y);
    }
};



int main()
{
    sf::RenderWindow win(sf::VideoMode(1080, 720), "Iridium");
    win.setFramerateLimit(80);
    win.setKeyRepeatEnabled(false);
    sf::Event event;
    sf::VertexArray lines(sf::Lines, 4);
    bool keyState[sf::Keyboard::KeyCount];
    for(int i = 0; i < (sf::Keyboard::KeyCount); i++){keyState[i] = false;}
    int game_state = 1; //main menu 0, game 1...
    Game* game = new Game(lines);

    while (win.isOpen())
    {
        win.clear();
        if (game_state == 1)
        {
            game->ResetLines();
            game->Movement();
            while(win.pollEvent(event))
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        win.close();
                        break;
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape) {win.close();}
                        if (event.key.code == sf::Keyboard::Space && !keyState[event.key.code])
                            {game->SwitchPushPull();}
                        //if (event.key.code == sf::Keyboard::R && !keyState[event.key.code]) {current_level_num++; level = new Level(current_level_num, 30, 30, 1000, 30, sf::Color::Red);}
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
                game->LeftOrb();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                game->RightOrb();
            }

            win.draw(game->level->GetSprite());
            win.draw(game->lines);
            win.draw(game->ball);
            win.draw(game->level -> control1);
            win.draw(game->level -> control2);
        }
        win.display();
    }
    return 0;
}
