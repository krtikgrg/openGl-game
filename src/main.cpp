#include "main.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "maze.h"
#include "imposter.h"
#include "powobs.h"
#include "coin.h"
#include "bomb.h"
#include "button.h"

#include<unistd.h>
#include<stdio.h>
#include<limits.h>
using namespace std;

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;


GLMatrices Matrices;
GLuint     programID;
GLuint     texture_test;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

int Player::player_grid[9][7];


//global VAO,VBO for text render
unsigned int text_VAO, text_VBO;


//FreeType Initializations
FT_Library ft;
FT_Face face;

//General Game Stuff

Ball ball1;
Player player1;
Player enemy1;
Maze maze1;
Imposter imposter1;
Coin coin1,coin2,coin3;
Bomb bomb1,bomb2,bomb3;
Powobs buttonPowerup;
Button vapouriseButton;
GLint Light = 0;
GLint task1 = 0;
GLint task2 =0;
GLint flag = 0;
GLint con = 0,con2 = 0,con3 = 0,obs = 0,obs2 = 0,obs3 = 0;
GLfloat Time = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color,glm::mat4 VP);

Timer t8(1.0 / 8);

/* Render the scene with openGL */
/* Edit this function according to your assignment */


int get_Move(){
    //vertice => 9*7 => 63

    //making graph
    vector<pair<int,int>> edges;
    vector<int> weights(63);
    for(int i=0;i<63;i++){
        weights[i] = (INT_MAX/2)+1;
    }
    for(int j=0;j<7;j++){
        for(int i = 0;i<9;i++){
            if(!(player1.player_grid[i][j] == 0 || player1.player_grid[i][j] == 1 || player1.player_grid[i][j] == 2))
                continue;
            if(maze1.grid[(i+1)*4][(j*5)+2] != 'w' && (player1.player_grid[i+1][j] == 0 || player1.player_grid[i+1][j] == 1 || player1.player_grid[i+1][j] == 2))
                edges.push_back(make_pair(9*j+i,9*j+i+1));
            if(maze1.grid[i*4][(j*5)+2] != 'w' && (player1.player_grid[i-1][j] == 0 || player1.player_grid[i-1][j] == 1 || player1.player_grid[i-1][j] == 2))
                edges.push_back(make_pair(9*j+i,9*j+i-1));
            if(maze1.grid[i*4 + 2][(j+1)*5]!= 'w' && (player1.player_grid[i][j+1] == 0 || player1.player_grid[i][j+1] == 1 || player1.player_grid[i][j+1] == 2))
                edges.push_back(make_pair(9*j+i,9*(j+1)+i));
            if(maze1.grid[i*4 + 2][(j)*5]!= 'w' && (player1.player_grid[i][j-1] == 0 || player1.player_grid[i][j-1] == 1 || player1.player_grid[i][j-1] == 2))
                edges.push_back(make_pair(9*j+i,9*(j-1)+i));
        }
    }
    // std::cout<<edges.size()<<std::endl;
    int enemyPos = -1;
    int move = -1;
    for(int i=0;i<9;i++){
        for(int j=0;j<7;j++){
            if(player1.player_grid[i][j] == 1)
                weights[9*j+i] = 0;
            else if(player1.player_grid[i][j] == 2)
                enemyPos = 9*j+i;
        }
    }

    //Bellman-Ford
    for(int i=0;i<62;i++){
        for(int j=0;j<edges.size();j++){
            if(weights[edges[j].second] > (weights[edges[j].first]+1)){
                // cout<<"point A "<<edges[j].first<<" point B "<<edges[j].second<<endl;
                // cout<<"weight A "<<weights[edges[j].first]<<" weight B "<<weights[edges[j].second]<<endl;
                // cout<<"enemy is "<<enemyPos<<endl;
                // sleep(5);
                weights[edges[j].second] = (weights[edges[j].first]+1);
                if(edges[j].second == enemyPos){
                    // std::cout<<"Fuck me now"<<std::endl;
                    if((edges[j].second-edges[j].first) >= 9)
                        move = 2;
                    else if((edges[j].second-edges[j].first)<=-9)
                        move = 1;
                    else if((edges[j].second-edges[j].first) > 0)
                        move = 3;
                    else
                        move = 4;
                }
            }
        }
    }
    return move;
}

void display(string str){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLinkProgram(programID);
    glUseProgram (programID);
    float a,b,c;
    a=5*cos(camera_rotation_angle*M_PI/180.0f);
    b=0;
    c=5*sin(camera_rotation_angle*M_PI/180.0f);
    glm::vec3 eye (a,b,c);
    glm::vec3 target (0, 0, 0);
    glm::vec3 up (0, 1, 0);
    Matrices.view = glm::lookAt( eye, target, up );
    glm::mat4 VP = Matrices.projection * Matrices.view;
    GLfloat vertex_array_buffer[] = {
        -3.7,3.4,0.0,
        3.5,3.4,0.0,
        -3.7,-3.6,0.0,

        3.5,3.4,0.0,
        -3.7,-3.6,0.0,
        3.5,-3.6,0.0
    };
    color_t blk = {0,0,0};
    struct VAO* object = create3DObject(GL_TRIANGLES, 6, vertex_array_buffer, blk, GL_FILL);
    glBindVertexArray(0);
    glBindVertexArray(object->VertexArrayID);
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(object);
    glUseProgram(0);
    glLinkProgram(texture_test);
    glUseProgram(texture_test);
    glBindVertexArray(0);
    glBindVertexArray(text_VAO);

    RenderText(str, -300.0f, 0.0f, 0.80f, glm::vec3(1.0f, 1.0f, 1.0f),VP);

    glUseProgram(0);
    glfwSwapBuffers(window);
    glfwPollEvents();
    sleep(5);
    exit(0);
}

void move_Enemy(){
    int a = get_Move();
    if(a == -1){
        // std::cout<<"Fuck everything"<<std::endl;
        return;
    }
    if(a == 1){
        enemy1.moveEnemy(0,100,maze1);
    }
    else if(a == 2){
        enemy1.moveEnemy(0,-100,maze1);
    }
    else if(a == 3){
        enemy1.moveEnemy(-80,0,maze1);
    }
    else{
        enemy1.moveEnemy(80,0,maze1);
    }
    if(enemy1.grid_x == player1.grid_x && enemy1.grid_y == player1.grid_y){
        display("YOU LOST BUDDY!!!");
        exit(0); //exit you lost screen
    }
}

void paint_Black(glm::mat4 VP){
    // std::cout<< "in this i think"<<std::endl;
    GLint playerX,playerY;
    playerX = (int)(player1.position.x * 100.1); 
    playerY = (int)(player1.position.y * 100.1);
    GLint rightX = playerX + 120;
    GLint leftX = playerX - 120;
    GLint downY = playerY - 120;
    GLint upY = playerY + 120;
    GLfloat temp_vertices[8*3*3];
    GLint gind = 0;
    if(upY < 340){
        temp_vertices[gind+0] = -3.7;
        temp_vertices[gind+1] = 3.4;
        temp_vertices[gind+2] = 0.01;

        temp_vertices[gind+3] = 3.5;
        temp_vertices[gind+4] = 3.4;
        temp_vertices[gind+5] = 0.01;

        temp_vertices[gind+6] = -3.7;
        temp_vertices[gind+7] = upY/100.0;
        temp_vertices[gind+8] = 0.01;

        temp_vertices[gind+9] = 3.5;
        temp_vertices[gind+10] = 3.4;
        temp_vertices[gind+11] = 0.01;

        temp_vertices[gind+12] = -3.7;
        temp_vertices[gind+13] = upY/100.0;
        temp_vertices[gind+14] = 0.01;

        temp_vertices[gind+15] = 3.5;
        temp_vertices[gind+16] = upY/100.0;
        temp_vertices[gind+17] = 0.01;
        gind = 18;
    }
    else{
        upY = 340;
    }
    if (downY>-360){
        temp_vertices[gind+0] = 3.5;
        temp_vertices[gind+1] = -3.6;
        temp_vertices[gind+2] = 0.01;

        temp_vertices[gind+3] = 3.5;
        temp_vertices[gind+4] = downY/100.0;
        temp_vertices[gind+5] = 0.01;

        temp_vertices[gind+6] = -3.7;
        temp_vertices[gind+7] = -3.6;
        temp_vertices[gind+8] = 0.01;

        temp_vertices[gind+2+7] = 3.5;
        temp_vertices[gind+2+8] = downY/100.0;
        temp_vertices[gind+2+9] = 0.01;

        temp_vertices[gind+12] = -3.7;
        temp_vertices[gind+13] = -3.6;
        temp_vertices[gind+14] = 0.01;

        temp_vertices[gind+15] = -3.7;
        temp_vertices[gind+16] = downY/100.0;
        temp_vertices[gind+17] = 0.01;
        gind+=18;
    }
    else{
        downY = -360;
    }
    if(leftX > -370){
        temp_vertices[gind+0] = -3.7;
        temp_vertices[gind+1] = upY/100.0;
        temp_vertices[gind+2] = 0.01;

        temp_vertices[gind+3] = leftX/100.0;
        temp_vertices[gind+4] = upY/100.0;
        temp_vertices[gind+5] = 0.01;

        temp_vertices[gind+6] = -3.7;
        temp_vertices[gind+7] = downY/100.0;
        temp_vertices[gind+8] = 0.01;

        temp_vertices[gind+2+7] = leftX/100.0;
        temp_vertices[gind+2+8] = upY/100.0;
        temp_vertices[gind+2+9] = 0.01;

        temp_vertices[gind+12] = -3.7;
        temp_vertices[gind+13] = downY/100.0;
        temp_vertices[gind+14] = 0.01;

        temp_vertices[gind+15] = leftX/100.0;
        temp_vertices[gind+16] = downY/100.0;
        temp_vertices[gind+17] = 0.01;
        gind+=18;
    }
    else{
        leftX = -370;
    }
    if (rightX<350){
        temp_vertices[gind+0] = rightX/100.0;
        temp_vertices[gind+1] = upY/100.0;
        temp_vertices[gind+2] = 0.01;

        temp_vertices[gind+3] = 3.5;
        temp_vertices[gind+4] = upY/100.0;
        temp_vertices[gind+5] = 0.01;

        temp_vertices[gind+6] = rightX/100.0;
        temp_vertices[gind+7] = downY/100.0;
        temp_vertices[gind+8] = 0.01;

        temp_vertices[gind+2+7] = 3.5;
        temp_vertices[gind+2+8] = upY/100.0;
        temp_vertices[gind+2+9] = 0.01;

        temp_vertices[gind+12] = rightX/100.0;
        temp_vertices[gind+13] = downY/100.0;
        temp_vertices[gind+14] = 0.01;

        temp_vertices[gind+15] = 3.5;
        temp_vertices[gind+16] = downY/100.0;
        temp_vertices[gind+17] = 0.01;
        gind+=18;
    }
    else{
        rightX = 350;
    }
    GLfloat vertices[gind];
    for(int i=0;i<gind;i++){
        vertices[i] = temp_vertices[i];
    }
    color_t blk = {0,0,0};
    struct VAO* object = create3DObject(GL_TRIANGLES, gind/3, vertices, blk, GL_FILL);
    glBindVertexArray(0);
    glBindVertexArray(object->VertexArrayID);
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    // Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(object);

}

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color,glm::mat4 VP)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1000), 0.0f, static_cast<float>(1000));
    glUniformMatrix4fv(glGetUniformLocation(texture_test, "projection"), 1, GL_FALSE, glm::value_ptr(VP));
    // activate corresponding render state	
    glUniform3f(glGetUniformLocation(texture_test, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    // glActiveTexture(0);
}


void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glLinkProgram(programID);
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    float a,b,c;
    // if(flag == 0){
    a=5*cos(camera_rotation_angle*M_PI/180.0f);
    b=0;
    c=5*sin(camera_rotation_angle*M_PI/180.0f);
    // }
    // else{
        // a = player1.position.x;
        // b = player1.position.y;
        // c = player1.position.z;
    // }
    
    
    
    glm::vec3 eye (a,b,c);
    // glm::vec3 eye (player1.position.x,player1.position.y,c);
    
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    
    
    glm::vec3 target (0, 0, 0);
    // glm::vec3 target(player1.position.x,player1.position.y,0);
    
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    // for(int t=0;t<4;t++){
    //     for(int t1=0;t1<4;t1++){
    //         std::cout<<Matrices.projection[t][t1]<<" "<<std::endl;
    //     }
    //     std::cout<<std::endl;
    // }
    // exit(0);
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    // ball1.draw(VP);
    glBindVertexArray(0);
    player1.bind();
    player1.draw(VP);
    glBindVertexArray(0);
    maze1.bind();
    maze1.draw(VP);
    if(task2 == 0){
        glBindVertexArray(0);
        buttonPowerup.bind();
        buttonPowerup.draw(VP);
    }
    if(task1 == 0){
        glBindVertexArray(0);
        enemy1.bind();
        enemy1.draw(VP);
        glBindVertexArray(0);
        vapouriseButton.bind();
        vapouriseButton.draw(VP);
    }
    if(con ==1){
        glBindVertexArray(0);
        coin1.bind();
        coin1.draw(VP);
    }
    if(con2 ==1){
        glBindVertexArray(0);
        coin2.bind();
        coin2.draw(VP);
    }
    if(con3 ==1){
        glBindVertexArray(0);
        coin3.bind();
        coin3.draw(VP);
    }
    if(obs ==1){
        glBindVertexArray(0);
        bomb1.bind();
        bomb1.draw(VP);
    }
    if(obs2 ==1){
        glBindVertexArray(0);
        bomb2.bind();
        bomb2.draw(VP);
    }
    if(obs3 ==1){
        glBindVertexArray(0);
        bomb3.bind();
        bomb3.draw(VP);
    }
    if(flag == 1){
        paint_Black(VP);
    }
    glUseProgram(0);
    glLinkProgram(texture_test);
    glUseProgram(texture_test);
    glBindVertexArray(0);
    glBindVertexArray(text_VAO);
    string tk1="",tk2="";
    string lght = "";
    if(task1 == 0)
        tk1 += "Not Done(Vaporise Enemy)";
    else
        tk1 += "Done(Enemy vaporised)";
    if(task2 == 0)
        tk2 += "Not Done(Spawn Powerups/Obstacles and collect them)";
    else if(task2 == 1)
        tk2 += "Not Done(Powerups/Obstacles spawned but not collected)";
    else{
        tk2 += "Done(Powerups/Obstacles spawned and collected)";
    }
    if(flag == 1){
        lght += "Off";
    }
    else{
        lght += "On"; 
    }
    int test_time = 180 - int(Time);
    if (test_time == 0){
        //display GameOver and exit
        display("OOPS! GAME OVER(TIME OUT)");
        exit(0); 
    }
    RenderText("Health: " + to_string(player1.health), -370.0f, 386.0f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f),VP);
    RenderText("Task1 : " + tk1, -370.0f, 375.0f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f),VP);
    RenderText("Task2 : " + tk2, -370.0f, 364.0f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f),VP);
    RenderText("Time Remaining : " + to_string(int(test_time)), -370.0f, 353.0f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f),VP);
    RenderText("Light : " + lght, -170.0f, 353.0f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f),VP);
    RenderText("<=Start",355.0f,-330.0f,0.27f,glm::vec3(0.0f,0.0f,0.0f),VP);
    if((task1 == 1 && task2 == 2)){
        RenderText("end=>",-400.0f,+290.0f,0.25f,glm::vec3(0.0f,0.0f,0.0f),VP);
    }
    glUseProgram(0);

}
void spawnCoinObs(){
    int numofcoins = (int)((rand()/(double)RAND_MAX)*3)+1;
    int indX,indY;
    if(numofcoins >=1 ){
        indX = 8-(int)((rand()/(double)RAND_MAX)*8);
        indY = (int)((rand()/(double)RAND_MAX)*6);
        while(player1.player_grid[8-indX][indY] != 0){
            indX = 8-(int)((rand()/(double)RAND_MAX)*8);
            indY = (int)((rand()/(double)RAND_MAX)*6);
        }
        coin1 = Coin(3.12-indX*0.8,-3.1+indY*1.0);
        coin1.grid_x = 8-indX;
        coin1.grid_y = indY;
        player1.player_grid[8-indX][indY] = 5;
        con = 1;

        if(numofcoins>=2){
            indX = 8-(int)((rand()/(double)RAND_MAX)*8);
            indY = (int)((rand()/(double)RAND_MAX)*6);
            while(player1.player_grid[8-indX][indY] != 0){
                indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                indY = (int)((rand()/(double)RAND_MAX)*6);
            }
            coin2 = Coin(3.12-indX*0.8,-3.1+indY*1.0);
            coin2.grid_x = 8-indX;
            coin2.grid_y = indY;
            player1.player_grid[8-indX][indY] = 5;
            con2 = 1;
            if(numofcoins>=3){
                indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                indY = (int)((rand()/(double)RAND_MAX)*6);
                while(player1.player_grid[8-indX][indY] != 0){
                    indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                    indY = (int)((rand()/(double)RAND_MAX)*6);
                }
                coin3 = Coin(3.12-indX*0.8,-3.1+indY*1.0);
                coin3.grid_x = 8-indX;
                coin3.grid_y = indY;
                player1.player_grid[8-indX][indY] = 5;
                con3 = 1;
            }
        }
    }

    int numofobs = (int)((rand()/(double)RAND_MAX)*3)+1;
    if(numofobs>0){
        indX = 8-(int)((rand()/(double)RAND_MAX)*8);
        indY = (int)((rand()/(double)RAND_MAX)*6);
        while(player1.player_grid[8-indX][indY] != 0){
            indX = 8-(int)((rand()/(double)RAND_MAX)*8);
            indY = (int)((rand()/(double)RAND_MAX)*6);
        }
        bomb1 = Bomb(3.12-indX*0.8,-3.1+indY*1.0);
        bomb1.grid_x = 8-indX;
        bomb1.grid_y = indY;
        player1.player_grid[8-indX][indY] = 6;
        obs = 1;
        if(numofobs>1){
            indX = 8-(int)((rand()/(double)RAND_MAX)*8);
            indY = (int)((rand()/(double)RAND_MAX)*6);
            while(player1.player_grid[8-indX][indY] != 0){
                indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                indY = (int)((rand()/(double)RAND_MAX)*6);
            }
            bomb2 = Bomb(3.12-indX*0.8,-3.1+indY*1.0);
            bomb2.grid_x = 8-indX;
            bomb2.grid_y = indY;
            player1.player_grid[8-indX][indY] = 6;
            obs2 = 1;
            if(numofobs>2){
                indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                indY = (int)((rand()/(double)RAND_MAX)*6);
                while(player1.player_grid[8-indX][indY] != 0){
                    indX = 8-(int)((rand()/(double)RAND_MAX)*8);
                    indY = (int)((rand()/(double)RAND_MAX)*6);
                }
                bomb3 = Bomb(3.12-indX*0.8,-3.1+indY*1.0);
                bomb3.grid_x = 8-indX;
                bomb3.grid_y = indY;
                player1.player_grid[8-indX][indY] = 6;
                obs3 = 1;
            }
        }
    }
    return;
}
void tick_input(GLFWwindow *window) {
    // std::cout<<"testing"<<endl;
    int reval;
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window,GLFW_KEY_UP);
    int down = glfwGetKey(window,GLFW_KEY_DOWN);
    int plus = glfwGetKey(window,GLFW_KEY_KP_ADD);
    if(plus){
        flag = 1 - flag;
    }
    else{
        if (left) {
            // std::cout<<"k1"<<std::endl;
            reval = player1.move(-80,0,maze1);
        }
        else if (right) {
            // std::cout<<"k2"<<std::endl;
            reval = player1.move(80,0,maze1);
        }
        else if (up) {
            // std::cout<<"k3"<<std::endl;
            reval = player1.move(0,100,maze1);
        }
        else if (down) {
            // std::cout<<"k4"<<std::endl;
            reval = player1.move(0,-100,maze1);
        }
        if(reval == -1){
            //display You lost and exit
            display("YOU LOST BUDDY!!!");
            exit(0);
        }
        else if(reval == 1){
            enemy1.vapourise();
            task1 = 1;
        }
        else if(reval == 2){
            //display coins and bombs plus destroy button
            task2 = 1;
            spawnCoinObs();
        }
        else if(reval == 4){
            //got coin so increment score and destroy coin

            if(bomb1.grid_x == player1.grid_x && bomb1.grid_y == player1.grid_y){
                obs = 2;
            }
            else if(obs2 == 1 && bomb2.grid_x == player1.grid_x && bomb2.grid_y == player1.grid_y){
                obs2 = 2;
            }
            else if(obs3 == 1 && bomb3.grid_x == player1.grid_x && bomb3.grid_y == player1.grid_y){
                obs3 = 2;
            }
            if(con == 2 && (con3 == 2 || con3 ==0) && (con2 ==2 || con2 == 0)){
                task2 = 2;
            }
        }
        else if(reval == 3){
            //got bomb so decrement score and destroy bomb
            if(coin1.grid_x == player1.grid_x && coin1.grid_y == player1.grid_y){
                con = 2;
            }
            else if(con2 == 1 && coin2.grid_x == player1.grid_x && coin2.grid_y == player1.grid_y){
                con2 = 2;
            }
            else if(con3==1 && coin3.grid_x == player1.grid_x && coin3.grid_y == player1.grid_y){
                con3 = 2;
            }
            if(con == 2 && (con3 == 2 || con3 ==0) && (con2 ==2 || con2 == 0)){
                task2 = 2;
            }
        }
    }
}

void tick_elements() {
    // ball1.tick();
    // player1.tick();
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // ball1       = Ball(0, 0, COLOR_RED);
    // std::cout<<"p1"<<std::endl;
    player1 = Player(3.12,-3.1,0);
    // std::cout<<"p2"<<std::endl;
    glBindVertexArray(0);
    maze1 = Maze(0,0);
    for(int j=35;j>=0;j--){
        for(int i =0;i<37;i++){
            std::cout<<maze1.grid[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    glBindVertexArray(0);
    imposter1 = Imposter(-3.2,2.9);
    int indX = 8-(int)((rand()/(double)RAND_MAX)*8);
    int indY = (int)((rand()/(double)RAND_MAX)*6);
    while(player1.player_grid[8-indX][indY] != 0){
        indX = 8-(int)((rand()/(double)RAND_MAX)*8);
        indY = (int)((rand()/(double)RAND_MAX)*6);
    }
    enemy1 = Player(3.12-indX*0.8,-3.1+indY*1.0,1);
    enemy1.grid_x = 8-indX;
    enemy1.grid_y = indY;
    player1.player_grid[8-indX][indY] = 2;
    indX = 8-(int)((rand()/(double)RAND_MAX)*8);
    indY = (int)((rand()/(double)RAND_MAX)*6);
    while(player1.player_grid[8-indX][indY] != 0){
        indX = 8-(int)((rand()/(double)RAND_MAX)*8);
        indY = (int)((rand()/(double)RAND_MAX)*6);
    }
    buttonPowerup = Powobs(3.12-indX*0.8,-3.1+indY*1.0);
    buttonPowerup.grid_x = 8-indX;
    buttonPowerup.grid_y = indY;
    player1.player_grid[8-indX][indY] = 3;
    indX = 8-(int)((rand()/(double)RAND_MAX)*8);
    indY = (int)((rand()/(double)RAND_MAX)*6);
    while(player1.player_grid[8-indX][indY] != 0){
        indX = 8-(int)((rand()/(double)RAND_MAX)*8);
        indY = (int)((rand()/(double)RAND_MAX)*6);
    }
    vapouriseButton = Button(3.12-indX*0.8,-3.1+indY*1.0);
    vapouriseButton.grid_x = 8-indX;
    vapouriseButton.grid_y = indY;
    player1.player_grid[8-indX][indY] = 4;
    glBindVertexArray(0);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    texture_test = LoadShaders("../src/shaders/shader_text.vert","../src/shaders/shader_text.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 1.0f); // R, G, B, A
    glClearDepth (1.0f);

    //new changes
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void init_freetype(){
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(0);
    }
    // char buffer[1000];
    // getcwd(buffer,1000);
    // std::cout<<buffer<<std::endl;
    if (FT_New_Face(ft, "../src/fonts/ARIAL.TTF", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        exit(0);
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1000;
    int height = 1000;

    window = initGLFW(width, height);

    
    initGL (window, width, height);
    init_freetype();

    /* Draw in loop */
    int ctr = 0;
    int looper = 1;
    while ((!glfwWindowShouldClose(window)) && looper == 1) {
        // Process timers

        if (t8.processTick()) {
            // 25 fps
            // OpenGL Draw commands
            Time = Time + (1.0/8.0);
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            ctr++;
            if(task1 == 1 && task2 == 2 && player1.grid_y == 6 && player1.grid_x == 0){
                looper = 0;
            }
            tick_elements();
            tick_input(window);
            if(ctr==8){
                if(flag == 1){
                    player1.health += 10;
                }
                if(task1 == 0){
                    move_Enemy();
                }
                ctr = 0;
            }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    display("WELL DONE, YOU WON BUDDY!!!");
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}