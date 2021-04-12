#include "maze.h"
#include "main.h"

Maze::Maze(float x,float y) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    speed = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[160 * 2 * 3 * 3];
    GLfloat vertex_color_data[160 * 2 * 3 * 3];
    int gind = 0;
    srand(time(0));

    for(int i=0;i<37;i++){
        for(int j =0;j<36;j++){
            this->grid[i][j] = ' ';
        }
    }
    // std::cout<<"k1"<<std::endl;

    for(int i=0;i<37;i++){
        this->grid[i][0] = 'w';
        this->grid[i][35] = 'w';
    }
    for(int j=0;j<36;j++){
        this->grid[0][j] = 'w';
        this->grid[36][j] = 'w';
    }
    

    for(int j=5 ;j<=30; j+=5){
        for(int i=4; i<=32; i+=4){
            int testptr = (int)((((double)rand())/RAND_MAX)*100);
            if (testptr<40){
                GLfloat x1 = i*0.2 - 3.7;
                GLfloat y1 = j*0.2 - 3.6;
                GLfloat x2 = i*0.2 - 3.7;
                GLfloat y2 = (j+5)*0.2 - 3.6;
                GLfloat x3 = x1+0.2 ;
                GLfloat y3 = j*0.2 - 3.6;
                GLfloat x4 = x1+0.2 ;
                GLfloat y4 = (j+5)*0.2 - 3.6;

                for(int t=j;t<j+5;t++){
                    this->grid[i][t] = 'w';
                }

                GLfloat temp_vertex[] ={
                    x2,y2,0.0,
                    x4,y4,0.0,
                    x1,y1,0.0,

                    x4,y4,0.0,
                    x1,y1,0.0,
                    x3,y3,0.0
                };

                GLfloat temp_color[] ={
                    0.0,0.0,0.0,
                    0.0,0.0,0.0,
                    0.0,0.0,0.0,

                    0.0,0.0,0.0,
                    0.0,0.0,0.0,
                    0.0,0.0,0.0
                };
                for(int t=0;t<18;t++){
                    vertex_buffer_data[gind+t] = temp_vertex[t];
                    vertex_color_data[gind+t] = temp_color[t];
                }
                gind += 18;
            }
            testptr = (int)((((double)rand())/RAND_MAX)*100);
            if (testptr<40){
                GLfloat x1 = i*0.2 - 3.7;
                GLfloat y1 = j*0.2 - 3.6;
                GLfloat x2 = (i+4)*0.2 - 3.7;
                GLfloat y2 = (j)*0.2 -3.6;
                GLfloat x3 = x1 ;
                GLfloat y3 = y1+0.2 ;
                GLfloat x4 = x2 ;
                GLfloat y4 = y2+0.2 ;
                
                for(int t=i;t<i+4;t++){
                    this->grid[t][j] = 'w';
                }

                GLfloat temp_vertex[] ={
                    x2,y2,0.0,
                    x4,y4,0.0,
                    x1,y1,0.0,

                    x4,y4,0.0,
                    x1,y1,0.0,
                    x3,y3,0.0
                };

                GLfloat temp_color[] ={
                    0.0,0.0,0.0,
                    0.0,0.0,0.0,
                    0.0,0.0,0.0,

                    0.0,0.0,0.0,
                    0.0,0.0,0.0,
                    0.0,0.0,0.0
                };
                for(int t=0;t<18;t++){
                    vertex_buffer_data[gind+t] = temp_vertex[t];
                    vertex_color_data[gind+t] = temp_color[t];
                }
                gind += 18;
            }
        }
    }
    GLfloat x0 = -3.7+0.0;
    GLfloat x1 = -3.7+0.0;
    GLfloat x2 = -3.7+36*0.2;
    GLfloat x3 = -3.7+36*0.2;
    GLfloat x4 = x0;
    GLfloat x5 = x1;
    GLfloat x6 = x2;
    GLfloat x7 = x3;
    GLfloat x8 = x0;
    GLfloat x9 = x0+0.1;
    GLfloat xA = x5;
    GLfloat xB = x5+0.1;
    GLfloat xC = x2 - 0.1;
    GLfloat xD = x2;
    GLfloat xE = x7-0.1;
    GLfloat xF = x7;

    GLfloat y0 = -3.6+0.0;
    GLfloat y1 = -3.6+0.0+0.1;
    GLfloat y2 = y0;
    GLfloat y3 = y1;
    GLfloat y4 = -3.6+35*0.2;
    GLfloat y5 = y4+0.1;
    GLfloat y6 = y4;
    GLfloat y7 = y5;
    GLfloat y8 = y0;
    GLfloat y9 = y0;
    GLfloat yA = y5;
    GLfloat yB = y5;
    GLfloat yC = y2;
    GLfloat yD = y2;
    GLfloat yE = y7;
    GLfloat yF = y7;

    GLfloat temp_vertex[] = {
        x1,y1,0.0,
        x3,y3,0.0,
        x0,y0,0.0,

        x3,y3,0.0,
        x0,y0,0.0,
        x2,y2,0.0,

        x5,y5,0.0,
        x7,y7,0.0,
        x4,y4,0.0,

        x7,y7,0.0,
        x4,y4,0.0,
        x6,y6,0.0,

        xE,yE,0.0,
        xF,yF,0.0,
        xC,yC,0.0,

        xF,yF,0.0,
        xC,yC,0.0,
        xD,yD,0.0,

        xA,yA,0.0,
        xB,yB,0.0,
        x8,y8,0.0,

        xB,yB,0.0,
        x8,y8,0.0,
        x9,y9,0.0
    };

    GLfloat temp_color[] ={
        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,0.0,
        0.0,0.0,0.0
    };
    for(int i=0;i<72;i++){
        vertex_buffer_data[gind+i] = temp_vertex[i];
        vertex_color_data[gind+i] = temp_color[i];
    }
    gind += 72;
    // std::cout<<"k2 "<<gind<<std::endl;
    // std::cout<<"in Player"<<std::endl;

    this->object = create3DObject(GL_TRIANGLES, gind/3, vertex_buffer_data, vertex_color_data, GL_FILL);
    // std::cout<<"k3"<<std::endl;
}

void Maze::draw(glm::mat4 VP) {
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

void Maze::bind(){
    glBindVertexArray (this->object->VertexArrayID);
}
// void Maze::unbind(){
//     glun (this->object->VertexArrayID);
// }

void Maze::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Maze::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

