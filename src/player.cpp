#include "player.h"
#include "main.h"

Player::Player(float x, float y, int type) {
    this->ult_position_x = 3.12;
    this->ult_position_y = -3.1;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    if(type == 0){
        for (int i=0;i<9;i++){
            for(int j=0;j<7;j++){
                this->player_grid[i][j]=0;
            }
        }
    }
    if(type == 0){
        this->player_grid[8][0] = 1;
        this->grid_x = 8;
        this->grid_y = 0;
    }
    // else{
    //     this->player_grid[0][6] = 2;
    //     this->grid_x = 0;
    //     this->grid_y = 6;
    // }
    this->health = 100;
    speed = 1;
    color_t color1,color2;
    if(type == 0){
        color1 = color_t{0,0,160};
        color2 = color_t{173, 216, 230};
    }
    else{
        color1 = color_t{255,0,0};
        color2 = color_t{128,128,128};
    }
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[370*9];
    GLfloat vertex_color_data[370*9];
    GLfloat temp_vertex_buffer_data[] = {
        0.0, 1.25, 0.01,
        1.25, 1.25, 0.01,
        0.0, 0.75, 0.01,

        1.25, 1.25, 0.01,
        0.0, 0.75, 0.01,
        1.25, 0.75, 0.01,

        -1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,

        1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,

        -1.0, 1.0, 0.0,
        -1.5, 1.0, 0.0,
        -1.5, -1.0, 0.0,

        -1.0, 1.0, 0.0,
        -1.5, -1.0, 0.0,
        -1.0, -1.0, 0.0,

        -1.0, -1.0, 0.0,
        -1.0, -1.75, 0.0,
        -0.5, -1.0, 0.0,

        -1.0, -1.75, 0.0,
        -0.5, -1.0, 0.0,
        -0.5, -1.75, 0.0,

        0.5, -1.75, 0.0,
        0.5, -1.0, 0.0,
        1.0, -1.0, 0.0,

        0.5, -1.75, 0.0,
        1.0, -1.75, 0.0,
        1.0, -1.0, 0.0
    };
    GLfloat temp_color_buffer_data[]= {
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),

        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),

        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),

        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),
        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),
        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),

        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),
        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),
        (GLfloat)(color2.r/255.0), (GLfloat)(color2.g/255.0), (GLfloat)(color2.b/255.0),

        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),

        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0),
        (GLfloat)(color1.r/255.0), (GLfloat)(color1.g/255.0), (GLfloat)(color1.b/255.0)
    };
    for( int i =0 ; i<360 ; i++){
        GLfloat x1,y1,z1,x2,y2,z2,x3,y3,z3;
        x1 = 0.0;
        y1 = 0.0;
        z1 = 0.005;
        y2 = cos(i*(M_PI/180.0));
        x2 = sin(i*(M_PI/180.0));
        z2 = 0.005;
        y3 = cos((i+1)*(M_PI/180.0));
        x3 = sin((i+1)*(M_PI/180.0));
        z3 = 0.005;
        int j = i*9;
        vertex_buffer_data[j+0] = (x1+0)/6.5;
        vertex_buffer_data[j+1] = (y1+1)/6.5;
        vertex_buffer_data[j+2] = (z1)/6.5;
        vertex_buffer_data[j+3] = (x2+0)/6.5;
        vertex_buffer_data[j+4] = (y2+1)/6.5;
        vertex_buffer_data[j+5] = (z2)/6.5;
        vertex_buffer_data[j+6] = (x3+0)/6.5;
        vertex_buffer_data[j+7] = (y3+1)/6.5;
        vertex_buffer_data[j+8] = (z3)/6.5;
        for(int t=0;t<3;t++){
            vertex_color_data[j+3*t+0] = (GLfloat)(color2.r/255.0);
            vertex_color_data[j+3*t+1] = (GLfloat)(color2.g/255.0);
            vertex_color_data[j+3*t+2] = (GLfloat)(color2.b/255.0);
        }
    }
    for(int i=0;i<90;i++){
        vertex_buffer_data[360*9+i] = temp_vertex_buffer_data[i]/6.5;
        vertex_color_data[360*9+i] = temp_color_buffer_data[i];
    }
    // std::cout<<"in Player"<<std::endl;
    this->object = create3DObject(GL_TRIANGLES, 370*3, vertex_buffer_data, vertex_color_data, GL_FILL);
}

void Player::moveEnemy(int x,int y,Maze &maze){
    int newX,newY;
    newX = int((this->position.x)*100.1);
    newY = int((this->position.y)*100.1);
    // std::cout<<newX<<" ints made "<<newY<<std::endl;

    newX += x;
    newY += y;
    int offsetX = 312-newX;
    int offsetY = newY+310;
    if(offsetX<0){
        return;
    }
    if(offsetY < 0){
        // newY -=y;
        return;
    }
    int kX=0,kY = 0;
    int test = 0;
    while(test<offsetX){
        test+=20;
        kX++;
    }
    test = 0;
    while(test<offsetY){
        test+=20;
        kY++;
    }
    // std::cout<<offsetX<<" offsets "<<offsetY   <<std::endl;
    // std::cout<<kX<<" multiples "<<kY   <<std::endl;
    int curX = this->grid_x;
    int curY = this->grid_y;
    int nuX = 8-(kX/4);
    int nuY = kY/5;

    this->player_grid[this->grid_x][this->grid_y] = 0;
    this->grid_x = 8-(kX/4);
    this->grid_y = kY/5;
    this->player_grid[this->grid_x][this->grid_y] = 2;
    this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
}

int Player::move(int x,int y,Maze &maze){
    //3.12, -3.1
    // std::cout<<"In my function"<<std::endl;
    // std::cout<<this->position.x<<" "<<this->position.y<<std::endl;
    int newX,newY;
    newX = int((this->position.x)*100.1);
    newY = int((this->position.y)*100.1);
    // std::cout<<newX<<" ints made "<<newY<<std::endl;

    newX += x;
    newY += y;
    int offsetX = 312-newX;
    int offsetY = newY+310;
    if(offsetX<0){
        return 0;
    }
    if(offsetY < 0){
        // newY -=y;
        return 0;
    }
    int kX=0,kY = 0;
    int test = 0;
    while(test<offsetX){
        test+=20;
        kX++;
    }
    test = 0;
    while(test<offsetY){
        test+=20;
        kY++;
    }
    // std::cout<<offsetX<<" offsets "<<offsetY   <<std::endl;
    // std::cout<<kX<<" multiples "<<kY   <<std::endl;
    int curX = this->grid_x;
    int curY = this->grid_y;
    int nuX = 8-(kX/4);
    int nuY = kY/5;
    if(x>0){
        if(maze.grid[(nuX)*4][curY*5+2] == 'w')
            return 0;
    }
    else if(x<0){
        if(maze.grid[(curX)*4][curY*5+2] == 'w')
            return 0;
    }
    else if(y>0){
        if(maze.grid[(curX)*4+2][nuY*5] == 'w')
            return 0;
    }
    else{
        if(maze.grid[(curX)*4+2][curY*5] == 'w')
            return 0;
    }

    if(this->player_grid[nuX][nuY] == 2){
        return -1;
    }
    else if(this->player_grid[nuX][nuY] == 3){
        this->player_grid[this->grid_x][this->grid_y] = 0;
        this->grid_x = 8-(kX/4);
        this->grid_y = kY/5;
        this->player_grid[this->grid_x][this->grid_y] = 1;
        this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
        return 2;
    }
    else if(this->player_grid[nuX][nuY] == 4){
        this->player_grid[this->grid_x][this->grid_y] = 0;
        this->grid_x = 8-(kX/4);
        this->grid_y = kY/5;
        this->player_grid[this->grid_x][this->grid_y] = 1;
        this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
        return 1;
    }
    else if(this->player_grid[nuX][nuY] == 5){
        this->player_grid[this->grid_x][this->grid_y] = 0;
        this->health += 20;
        this->grid_x = 8-(kX/4);
        this->grid_y = kY/5;
        this->player_grid[this->grid_x][this->grid_y] = 1;
        this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
        return 3;
    }
    else if(this->player_grid[nuX][nuY] == 6){
        this->player_grid[this->grid_x][this->grid_y] = 0;
        this->health -= 20;
        this->grid_x = 8-(kX/4);
        this->grid_y = kY/5;
        this->player_grid[this->grid_x][this->grid_y] = 1;
        this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
        return 4;
    }
    this->player_grid[this->grid_x][this->grid_y] = 0;
    this->grid_x = 8-(kX/4);
    this->grid_y = kY/5;
    this->player_grid[this->grid_x][this->grid_y] = 1;
    this->position = glm::vec3(float(newX)/100, float(newY)/100, 0);
    return 0;
}

void Player::bind(){
    glBindVertexArray (this->object->VertexArrayID);
}

void Player::vapourise(){
    this->player_grid[grid_x][grid_y] = 0;
    return;
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

