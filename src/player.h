#include "main.h"
#include "maze.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, int type);
    glm::vec3 position;
    float rotation;
    float ult_position_x,ult_position_y;
    GLint health;
    static int player_grid[9][7];
    int grid_x,grid_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int move(int x,int y,Maze &maze);
    double speed;
    void bind();
    void vapourise();
    void moveEnemy(int x,int y,Maze &maze);
private:
    VAO *object;
};
#endif // PLAYER_H
