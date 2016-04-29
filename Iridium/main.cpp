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

class Collectable
{
public:
    bool collected;
    int x_pos;
    int y_pos;
    int radius;
    sf::CircleShape ball;
    Collectable(int x, int y)
    {
        x_pos = x;
        y_pos = y;
        radius = 10;
        collected = false;
        ball.setFillColor(sf::Color::Blue);
        ball.setRadius(radius);
        ball.setPosition(x_pos, y_pos);
    }

    bool IsColliding(int x, int y, int r)
    {
        if (sqrt(pow(abs((x + r) - (x_pos + radius)), 2) + pow(abs((y + r) - (y_pos + radius)), 2)) < (radius + r))
        {
            ball.setFillColor(sf::Color::Yellow);
            collected = true;
            return true;
        }
        return false;
    }
};

class Portal
{
public:
    bool teleported;
    int x1_pos;
    int y1_pos;
    int x2_pos;
    int y2_pos;
    int radius;
    sf::CircleShape portal1;
    sf::CircleShape portal2;

    Portal(int x1, int y1, int x2, int y2)
    {
        x1_pos = x1;
        y1_pos = y1;
        x2_pos = x2;
        y2_pos = y2;
        radius = 20;
        teleported = false;
        portal1.setFillColor(sf::Color(0,150,215));
        portal1.setRadius(radius);
        portal1.setPosition(x1_pos, y1_pos);
        portal2.setFillColor(sf::Color(255,125,0));
        portal2.setRadius(radius);
        portal2.setPosition(x2_pos, y2_pos);
    }
    void teleport (float& x, float& y, bool portal) // portal 1 is true, portal 2 is false
    {
        if (teleported == false)
        {
            teleported = true;
            if (portal){x = x2_pos; y = y2_pos;}
            else {x = x1_pos; y = y1_pos;}
        }
    }
    void IsColliding(float& x, float& y, int& r)
    {
        if (sqrt(pow(abs((x + r) - (x1_pos + radius)), 2) + pow(abs((y + r) - (y1_pos + radius)), 2)) < (r + radius))
        {
            teleport(x, y, true);
        }
        else if (sqrt(pow(abs((x + r) - (x2_pos + radius)), 2) + pow(abs((y + r) - (y2_pos + radius)), 2)) < (r + radius))
        {
            teleport(x, y, false);
        }
        else {teleported = false;}
    }
};

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
        Levels[0] = {165, 165, 15, 30, 30, 1000, 30, 30, 0, 0, 0};
        Levels[1] = {075, 615, 15, 30, 30, 1000, 30, 30, 5, 305, 215, 305, 485, 530, 350, 755, 215, 755, 485, 0, 0, 0, 0};
        Levels[2] = {165, 255, 15, 30, 30, 1000, 30, 30, 0, 0, 0};
        Levels[3] = {075, 615, 15, 30, 30, 1000, 30, 30, 3, 215, 185, 530, 185, 845, 185, 0, 0, 0};
        Levels[4] = {345, 435, 15, 25, 295, 1015, 295, 20, 6, 305, 148, 395, 148, 485, 148, 575, 148, 665, 148, 755, 148, 0, 0, 0, 0};
        Levels[5] = {125, 75, 15, 30, 30, 1000, 30, 30, 0, 0, 3, 125, 600, 380, 75, 380, 600, 650, 75, 650, 600, 920, 75};
        Levels[6] = {};
        Levels[7] = {};
        Levels[8] = {};
        Levels[9] = {};
        Levels[10] = {};
        Levels[11] = {};
        Levels[12] = {};
        Levels[13] = {};
        Levels[14] = {};
        Levels[15] = {};
        Levels[16] = {};
        Levels[17] = {};
        Levels[18] = {};
        Levels[19] = {};
        Levels[20] = {};
        Levels[21] = {};
        Levels[22] = {};
        Levels[23] = {};
        Levels[24] = {};
        Levels[25] = {};
        Levels[26] = {};
        Levels[27] = {};
        Levels[28] = {};
        Levels[29] = {};
        Levels[30] = {};
        Levels[31] = {};
        Levels[32] = {};
        Levels[33] = {};
        Levels[34] = {};
        Levels[35] = {};
        Levels[36] = {};
        Levels[37] = {};
        Levels[38] = {};
        Levels[39] = {};
        Levels[40] = {};
        Levels[41] = {};
        Levels[42] = {};
        Levels[43] = {};
        Levels[44] = {};
        Levels[45] = {};
        Levels[46] = {};
        Levels[47] = {};
        Levels[48] = {};
        Levels[49] = {};
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
    Collectable* collectables[50];
    //collectables.resize(50);
    Portal* portals[50];
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
        numCollectables = leveldata->Levels[level_number][8];
        for (int i = 0; i < numCollectables; i++)
        {
            collectables[i] = new Collectable(leveldata->Levels[level_number][(9 + (2 * i))], leveldata->Levels[level_number][(10 + (2 * i))]);
        }

        numEnemys = leveldata->Levels[level_number][(9 + (2 * numCollectables))];
        for (int i = 0; i < numEnemys; i++)
        {
            break;
        }
        std::cout<<(numCollectables)<<"\n";
        std::cout<<(numEnemys)<<"\n";
        std::cout<<(10 + (2 * numCollectables) + (4 * numEnemys))<<"\n";
        numPortals = leveldata->Levels[level_number][(10 + (2 * numCollectables) + (4 * numEnemys))];
        for (int i = 0; i < numPortals; i++)
        {
            portals[i] = new Portal(leveldata->Levels[level_number][((10 + (2 * numCollectables) + 1 + (4 * numEnemys)) + (4 * i))], leveldata->Levels[level_number][((10 + (2 * numCollectables) + 2 + (4 * numEnemys)) + (4 * i))], leveldata->Levels[level_number][((10 + (2 * numCollectables) + 3 + (4 * numEnemys)) + (4 * i))], leveldata->Levels[level_number][((10 + (2 * numCollectables) + 4 + (4 * numEnemys)) + (4 * i))]);
        }

        control1.setFillColor(colour);
        control2.setFillColor(colour);

        std::cout << "new level: " << level_number << std::endl;
    };
    sf::Sprite GetSprite(){return level_sprite;}
    sf::Vector2f GetControlOrb1Position(void){return control1.getPosition();}
    sf::Vector2f GetControlOrb2Position(void){return control2.getPosition();}

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
    bool all_collected;
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
        ball.setRadius(level->ballr);
        x = level->bally;
        y = level->ballx;
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
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / Quadrant_m1 * ((pull_strength * pull_constant) * -1));
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb2Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb2Position().x + level -> control_radius)))))) * (1 / Quadrant_m2 * ((pull_strength * pull_constant / 0.5f) * -1));
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
                x_speed += (cos(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / Quadrant_m1 * ((pull_strength * pull_constant) * -1));
                y_speed += (sin(atan(fabs(((y + level -> ballr) - (level -> GetControlOrb1Position().y + level -> control_radius))) / fabs(((x + level -> ballr) - (level -> GetControlOrb1Position().x + level -> control_radius)))))) * (1 / Quadrant_m2 * ((pull_strength * pull_constant / 0.5f) * -1));
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
        if (pushpull){y_speed += gravity;}else{y_speed -= gravity;}
        prev_x = x;
        prev_y = y;
        y += y_speed;
        x += x_speed;

        level->CollisionPoints(detected_points, x, y, ball.getRadius(), detection_points);
        for (int i = 0; i < level->numCollectables; i++)
        {
            level->collectables[i]->IsColliding(x, y, level->ballr);
        }
        for (int i = 0; i < level->numPortals; i++)
        {
            level->portals[i]->IsColliding(x, y, level->ballr);
        }
        if (detected_points[0] != 0)
        {
            if (detected_points[0] > 0)
            {
                sf::Vector2f col_tangent = AddVectors(detected_points, detection_points, (sqrt((x_speed * x_speed) + (y_speed * y_speed))));
                x_speed += (col_tangent.x) * i_friction_constant;
                y_speed += (col_tangent.y) * i_friction_constant;
                x = prev_x;
                y = prev_y;
                //x += x_speed;
                //y += y_speed;
            }
            else // collided with obstacle
            {
                x_speed = 0;
                y_speed = 0;
            }
        }
        if (level->CheckEnding(x, y, level -> ballr))
        {
            all_collected = true;
            for (int i = 0; i < level->numCollectables; i++)
            {
                if (level->collectables[i]->collected == false)
                {
                    all_collected = false;
                }
            }
            if (all_collected)
            {
                current_level_num++;
                level = new Level(current_level_num, sf::Color::Red);
                x = level->ballx;
                y = level->bally;
                ball.setRadius(level->ballr);
            }
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
            for (int i = 0; i < game->level->numCollectables; i++)
            {
                win.draw(game->level->collectables[i]->ball);
            }

            for (int i = 0; i < game->level->numPortals; i++)
            {
                win.draw(game->level->portals[i]->portal1);
                win.draw(game->level->portals[i]->portal2);
            }
            win.draw(game->lines);
            win.draw(game->ball);
            win.draw(game->level -> control1);
            win.draw(game->level -> control2);
        }
        win.display();
    }
    return 0;
}
