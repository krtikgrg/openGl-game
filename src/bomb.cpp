#include "bomb.h"
#include "main.h"

Bomb::Bomb(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[2*362*9];
    GLfloat vertex_color_data[2*362*9];
    for( int i =0 ; i<360 ; i++){
        GLfloat x1,y1,z1,x2,y2,z2,x3,y3,z3,x_1,y_1,z_1,x_2,y_2,z_2,x_3,y_3,z_3;
        x1 = 0.0;
        y1 = 0.0;
        z1 = 0.005;
        y2 = cos(i*(M_PI/180.0));
        x2 = sin(i*(M_PI/180.0));
        z2 = 0.005;
        y3 = cos((i+1)*(M_PI/180.0));
        x3 = sin((i+1)*(M_PI/180.0));
        z3 = 0.005;

        x_1 = 0.0;
        y_1 = 0.0;
        z_1 = 0.003;
        y_2 = 1.2*cos(i*(M_PI/180.0));
        x_2 = 1.2*sin(i*(M_PI/180.0));
        z_2 = 0.003;
        y_3 = 1.2*cos((i+1)*(M_PI/180.0));
        x_3 = 1.2*sin((i+1)*(M_PI/180.0));
        z_3 = 0.003;
        int j = i*9*2;
        
        vertex_buffer_data[j+0] = (x1+0)/6.75;
        vertex_buffer_data[j+1] = (y1+1)/6.75;
        vertex_buffer_data[j+2] = (z1)/6.75;
        vertex_buffer_data[j+3] = (x2+0)/6.75;
        vertex_buffer_data[j+4] = (y2+1)/6.75;
        vertex_buffer_data[j+5] = (z2)/6.75;
        vertex_buffer_data[j+6] = (x3+0)/6.75;
        vertex_buffer_data[j+7] = (y3+1)/6.75;
        vertex_buffer_data[j+8] = (z3)/6.75;

        vertex_buffer_data[j+9] = (x_1+0)/6.75;
        vertex_buffer_data[j+10] = (y_1+1)/6.75;
        vertex_buffer_data[j+11] = (z_1)/6.75;
        vertex_buffer_data[j+12] = (x_2+0)/6.75;
        vertex_buffer_data[j+13] = (y_2+1)/6.75;
        vertex_buffer_data[j+14] = (z_2)/6.75;
        vertex_buffer_data[j+15] = (x_3+0)/6.75;
        vertex_buffer_data[j+16] = (y_3+1)/6.75;
        vertex_buffer_data[j+17] = (z_3)/6.75;
        GLfloat a = 255.0;
        for(int t=0;t<6;t++){
            vertex_color_data[j+3*t+0] = a/255.0;
            vertex_color_data[j+3*t+1] = 0/255.0;
            vertex_color_data[j+3*t+2] = 0/255.0;
            if(t==2){
                a = 255.0;
            }
        }
    }
    int j = 360*9*2;
    GLfloat offset = 2.5;
    GLfloat temp_vertex_data[] = {
        -0.1,GLfloat(0.5+offset),0.006,
        0.1,GLfloat(0.5+offset),0.006,
        -0.1,GLfloat(-0.5+offset),0.006,

        0.1,GLfloat(0.5+offset),0.006,
        -0.1,GLfloat(-0.5+offset),0.006,
        0.1,GLfloat(-0.5+offset),0.006,

        0.5+0.5,GLfloat(0.1+offset+0.4),0.006,
        -0.5+0.5,GLfloat(0.1+offset+0.4),0.006,
        0.5+0.5,GLfloat(-0.1+offset+0.4),0.006,

        -0.5+0.5,GLfloat(0.1+offset+0.4),0.006,
        0.5+0.5,GLfloat(-0.1+offset+0.4),0.006,
        -0.5+0.5,GLfloat(-0.1+offset+0.4),0.006
    };
    GLfloat temp_color_data[] = {
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
        255.0/255.0,0.0/255.0,255.0/255.0,
    };
    for(int i = 0;i<36;i++){
        vertex_buffer_data[j+i] = temp_vertex_data[i]/6.75;
        vertex_color_data[j+i] = temp_color_data[i];
    }
    // std::cout<<"in Bomb"<<std::endl;
    this->object = create3DObject(GL_TRIANGLES, 2*362*3, vertex_buffer_data, vertex_color_data, GL_FILL);
}

void Bomb::bind(){
    glBindVertexArray (this->object->VertexArrayID);
}

void Bomb::draw(glm::mat4 VP) {
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

void Bomb::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Bomb::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

//0-359 => 0-89, 270-359