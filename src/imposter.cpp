#include "imposter.h"
#include "main.h"

Imposter::Imposter(float x, float y) {
    this->ult_position_x = -3.2;
    this->ult_position_y = 2.9;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    for (int i=0;i<9;i++){
        for(int j=0;j<7;j++){
            this->player_grid[i][j]=0;
        }
    }
    this->player_grid[0][6] = 1;
    this->grid_x = 0;
    this->grid_y = 6;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[270*9];
    GLfloat vertex_color_data[270*9];
    for( int i =0 ; i<360 ; i++){
        if(i==45){
            i=134;
            continue;
        }
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
        int j;
        if(i>=135){
            j = (i-90)*9;
        }
        else{
            j = i*9;
        }
        vertex_buffer_data[j+0] = (x1+0)/4.75;
        vertex_buffer_data[j+1] = (y1+1)/4.75;
        vertex_buffer_data[j+2] = (z1)/4.75;
        vertex_buffer_data[j+3] = (x2+0)/4.75;
        vertex_buffer_data[j+4] = (y2+1)/4.75;
        vertex_buffer_data[j+5] = (z2)/4.75;
        vertex_buffer_data[j+6] = (x3+0)/4.75;
        vertex_buffer_data[j+7] = (y3+1)/4.75;
        vertex_buffer_data[j+8] = (z3)/4.75;
        for(int t=0;t<3;t++){
            vertex_color_data[j+3*t+0] = 120.0/255.0;
            vertex_color_data[j+3*t+1] = 142.0/255.0;
            vertex_color_data[j+3*t+2] = 82.0/255.0;
        }
    }
    // std::cout<<"in Imposter"<<std::endl;
    this->object = create3DObject(GL_TRIANGLES, 270*3, vertex_buffer_data, vertex_color_data, GL_FILL);
}

void Imposter::bind(){
    glBindVertexArray (this->object->VertexArrayID);
}

void Imposter::draw(glm::mat4 VP) {
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

void Imposter::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Imposter::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

//0-359 => 0-89, 270-359