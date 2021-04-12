#include "main.h"

#ifndef BOMB_H
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y);
    glm::vec3 position;
    float rotation;
    float ult_position_x,ult_position_y;
    // int player_grid[9][7];
    int grid_x,grid_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    void bind();
private:
    VAO *object;
};

#endif // BOMB_H
