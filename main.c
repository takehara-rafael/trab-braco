#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/freeglut.h>
#include <math.h>

#define WINDOW_SIZE 700
#define POINT_QNT 50
#define INITIAL_XPOS 0
#define INITIAL_YPOS 15
#define INITIAL_ZPOS 25
#define INITIAL_ZFAR 50
#define O(y,x) (y + (x<<2))

struct world {
    GLdouble x,y,z;
} w;

double pi = M_PI;

int proj=0;
double zfar = INITIAL_ZFAR;
float Xpos=INITIAL_XPOS, Ypos=INITIAL_YPOS, Zpos=INITIAL_ZPOS;
int Xo=0, Yo=0, Zo=0;
int Xl=0, Yl=1, Zl=0;
float vertices[POINT_QNT*3];
int rot=0;

int angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0, angle7 = 0, angle8 = 0, angle9 = 0;

float pos1=0, pos2=0, pos3=0, pos4=0, pos5=0, pos6=0;

int pause=1, choice=0, grab=0, closed;

GLdouble x_coord = 0, y_coord = 0, z_coord = 0, xx = 0, yy = 0 , zz = 0;

double rand_x = 0, rand_z = 0;

GLdouble matrix_cube[16], matrix_scene[16], proj_matrix[16]; 

GLdouble camViewMatrix[16], invCamViewMatrix[16];

GLint vp[4];

int k = 0;

void Display();
void Mouse(int btn, int state);
void Keyboard(unsigned char key);
void BuildScene();
void BuildArm();
void DrawPath();
void DrawCube(GLfloat centerXpos, GLfloat centerYpos, GLfloat centerZpos, GLfloat edgeLength);
void FollowCurve(int);
void moveHand(int);
double rand_number(double min, double max);
int gluInvertMatrix(const double m[16], double invOut[16]);
static inline void Matrix4x4MultiplyBy4x4 (GLdouble *src1, GLdouble *src2, struct world *w);

/******************************************************************/

void Display() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    if(proj==1) {
        glOrtho(-100, 100, -100, 100, -100, 100);
    } else {
        gluPerspective(45,1,1,zfar);
    }

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(Xpos, Ypos, Zpos, Xo, Yo, Zo, Xl, Yl, Zl);

    glGetDoublev (GL_MODELVIEW_MATRIX, camViewMatrix);

    gluInvertMatrix(camViewMatrix, invCamViewMatrix);

    glRotatef(rot, Xl, Yl, Zl);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    BuildScene();
    glPopMatrix();

    glPushMatrix();
    BuildArm();
    glPopMatrix();

      if(abs(w.x) < 0.1 && abs(w.y) < 0.1 && abs(w.z) < 2.5)
      {
          grab=1;
      }  

     //printf("(X:%f, Y:%f, Z:%f)", w.x, w.y, w.z);


    // if( rand_x > 0) {
    //     if ((matrix_scene[4] > -0.0002) && (matrix_scene[4] < 0)) {

    //         if (fabs(matrix_scene[6]) > fabs(matrix_cube[6])) {
    //             if (fabs(matrix_scene[6] / matrix_cube[6]) < 1.2) {
    //                 grab=1;
    //                 k++;
    //             }
    //         } else if (fabs(matrix_scene[6]) < fabs(matrix_cube[6])) {
    //             if (fabs(matrix_cube[6] / matrix_scene[6]) < 1.2) {
    //                 grab=1;
    //                 k++;
    //             }
    //         }
    //     }
    // } else {
    //     if ((matrix_scene[4] < 0.0002) && (matrix_scene[4] > 0)) {

    //         if (fabs(matrix_scene[6]) > fabs(matrix_cube[6])) {
    //             if (fabs(matrix_scene[6] / matrix_cube[6]) < 1.2) {
    //                 printf("grab %d\n\n", k);
    //                 k++;
    //             }
    //         } else if (fabs(matrix_scene[6]) < fabs(matrix_cube[6])) {
    //             if (fabs(matrix_cube[6] / matrix_scene[6]) < 1.2) {
    //                 printf("grab %d\n\n", k);
    //                 k++;
    //             }
    //         }
    //     }
    // }


    glPushMatrix();
    glColor3f(1,0,0);
    DrawPath();
    glPopMatrix();

    glutSwapBuffers();
}

/******************************************************************/

void BuildScene() {

    //chão
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glScalef(1, 0.0001, 1);
    glutSolidCube(50.0);
    glPopMatrix();

    //mesa
    glPushMatrix();
    glColor3ub(150, 75, 0);
    glTranslatef(0, 2, 0);
    glScalef(1, 0.02, 1);
    glutSolidCube(20.0);
    glPopMatrix();

    //pés da mesa
    glPushMatrix();
    glTranslatef(8, 1, 8);
    glScalef(0.5, 2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8, 1, 8);
    glScalef(0.5, 2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8, 1, -8);
    glScalef(0.5, 2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, 1, -8);
    glScalef(0.5, 2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

/********************************************************************************/


void DrawPath() {

    //desenho do caminho da camera
    int j=0;
    int i;
    for(i=-INITIAL_ZPOS; i<=INITIAL_ZPOS; i++) {
        vertices[j] = i;

        j++;
        vertices[j] = 15+(4*sin(i*(pi/18)));

        j++;
        vertices[j]= -sqrt(pow(INITIAL_ZPOS,2)-pow(i,2));

        j++;
    }
    for(i=INITIAL_ZPOS; i>=-INITIAL_ZPOS; i--) {
        vertices[j] = i;

        j++;
        vertices[j] = 15+(4*sin(i*(pi/18)));

        j++;
        vertices[j] = sqrt(pow(INITIAL_ZPOS,2)-pow(i,2));

        j++;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT, 3*sizeof(float),vertices);
    glDrawArrays(GL_LINE_LOOP, 0, POINT_QNT*2);
    glDisableClientState(GL_VERTEX_ARRAY);
}


/******************************************************************/

void BuildArm() {

    //primeiro elo
    glColor3ub(51, 51, 255);
    glTranslatef(0, 2.5, 0);
    glutSolidCube(1.0);

    //primeira esfera
    glColor3f(1,0,0);
    glTranslatef(0,0.6,0);
    glScalef(0.6,0.6,0.6);
    glRotatef(angle1, 1, 0, 0);
    glRotatef(angle2, 0, 1, 0);
    glRotatef(angle3, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //segundo elo
    glColor3f(0,1,0);
    glTranslatef(0,2.5,0);
    glPushMatrix();
    glScalef(1.5,10,1);
    glutSolidCube(0.5);
    glPopMatrix();

    //segunda esfera
    glColor3f(1,0,0);
    glTranslatef(0,2,0);
    glRotatef(angle4, 1, 0, 0);
    glRotatef(angle5, 0, 1, 0);
    glRotatef(angle6, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //terceiro elo
    glColor3f(0,1,0);
    glTranslatef(2.5,0,0);
    glPushMatrix();
    glScalef(10,1.5,1);
    glutSolidCube(0.5);
    glPopMatrix();

    //terceira esfera
    glColor3f(1,0,0);
    glTranslatef(2,0,0);
    glRotatef(angle7, 1, 0, 0);
    glRotatef(angle8, 0, 1, 0);
    glRotatef(angle9, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //garra
    glColor3f(0,1,0);
    glTranslatef(0.85,0,0);
    glPushMatrix();
    glScalef(1,1,5);
    glutSolidCube(0.5);

    glGetDoublev(GL_MODELVIEW_MATRIX, matrix_scene);
    Matrix4x4MultiplyBy4x4(invCamViewMatrix, matrix_scene, &w);
        glColor3ub(1,0,1);
        // xx = matrix_scene[0] * xx + matrix_scene[4] * yy + matrix_scene[8] * zz + matrix_scene[12]+7.04;
        // yy = matrix_scene[1] * xx + matrix_scene[5] * yy + matrix_scene[9] * zz + matrix_scene[13]-8.32;
        // zz = matrix_scene[2] * xx + matrix_scene[6] * yy + matrix_scene[10] * zz + matrix_scene[14]+28.85;
        glScalef(1,1,0.2);
        //DrawCube(w.x,w.y,w.z-6,0.5);
        glPopMatrix();

    if(grab){
        glutTimerFunc(0,moveHand,grab);
        glPushMatrix();
            glColor3ub(255,0,255);
            DrawCube(0.5,0,0,0.5);
        glPopMatrix();
    }
        //else{
        //     glutTimerFunc(0,closeHand,0);
        //     abriu = !abriu;
        // }

    //dedos
    glPushMatrix();
    glColor3f(0,1,0);
    glTranslatef(0,0, pos3);
    glTranslatef(0.75,0,-1);
    glScalef(4,1,1);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0, pos6);
    glTranslatef(0.75,0, 1);
    glScalef(4,1,1);
    glutSolidCube(0.5);
    glPopMatrix();

    glPopMatrix();

    glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);
    glGetIntegerv(GL_VIEWPORT, vp);
    glPushMatrix();
    // if(rand_x>0)
    //     x_coord = matrix_cube[0] * x_coord + matrix_cube[4] * y_coord + matrix_cube[8] * z_coord + matrix_cube[12]-rand_x;
    // else
    //     x_coord = matrix_cube[0] * x_coord + matrix_cube[4] * y_coord + matrix_cube[8] * z_coord + matrix_cube[12]+rand_x;
    // y_coord = matrix_cube[1] * x_coord + matrix_cube[5] * y_coord + matrix_cube[9] * z_coord + matrix_cube[13]-2.9;
    // if(rand_z>0)
    //     z_coord = matrix_cube[2] * x_coord + matrix_cube[6] * y_coord + matrix_cube[10] * z_coord + matrix_cube[14]-rand_z;
    // else
    //     z_coord = matrix_cube[2] * x_coord + matrix_cube[6] * y_coord + matrix_cube[10] * z_coord + matrix_cube[14]+rand_z;
    // DrawCube(rand_x, 2.6, rand_z, 0.5);
    
    if(!grab) {
        glColor3ub(255,0,255);
        DrawCube(rand_x, 2.6, rand_z, 0.5);
    }
    glPopMatrix();
}

/******************************************************************/

double rand_number(double min, double max) {

    double num=0;

    choice = rand() % 2; // 0 ou 1
    while(num>(min/3) && num<(max/3)) { // regra para que o cubo seja gerado no espaço de trabalho do braço robótico
        if(choice == 1) {
            num = (((double)rand()/(double)RAND_MAX)*min); //gera número entre min e 0
        } else if(choice == 0) {
            num = (((double)rand()/(double)RAND_MAX)*max); // gera número entre 0 e max
        }
    }

    return num;
}

/******************************************************************/

//função para a câmera seguir a curva construída
void FollowCurve(int timerOn) {

    if(!pause) {
        //caso Z=0
        if(Zpos == 0) {
            if(Xpos == -INITIAL_ZPOS) {
                Xpos += 1;
                Ypos = 15+(4*sin(Xpos*(pi/18)));
                Zpos = -sqrt(pow(INITIAL_ZPOS,2)-pow(Xpos,2));
            } else if(Xpos == INITIAL_ZPOS) {
                Xpos -= 1;
                Ypos = 15+(4*sin(Xpos*(pi/18)));
                Zpos = sqrt(pow(INITIAL_ZPOS,2)-pow(Xpos,2));
            }
        }

        //caso Z>0
        if(Xpos>-INITIAL_ZPOS && Zpos>0) {
            Xpos -= 1;
            Ypos = 15+(4*sin(Xpos*(pi/18)));
            Zpos = sqrt(pow(INITIAL_ZPOS,2)-pow(Xpos,2));
        }

        //caso Z<0
        if(Xpos<INITIAL_ZPOS && Zpos<0) {
            Xpos += 1;
            Ypos = 15+(4*sin(Xpos*(pi/18)));
            Zpos = -sqrt(pow(INITIAL_ZPOS,2)-pow(Xpos,2));
        }

        glutPostRedisplay();
        glutTimerFunc(2000/15,FollowCurve,0); // 15 FPS -> execução recursiva
    }
}

/******************************************************************/

void moveHand(int a) {

        printf("pos6:%f pos3:%f", pos6, pos3);

        if((pos6 - pos3)>=-1) {
            pos6 -= 0.02;
            pos3 += 0.02;
        }

    glutPostRedisplay(); 
    glutTimerFunc(30000/15,moveHand,0);
}

/******************************************************************/

void DrawCube(GLfloat centerXpos, GLfloat centerYpos, GLfloat centerZpos, GLfloat edgeLength) {

    glTranslatef(centerXpos, centerYpos, centerZpos);
    glutSolidCube(edgeLength);

    glGetDoublev(GL_MODELVIEW_MATRIX, matrix_cube);

}

/******************************************************************/

void Mouse(int btn, int state) {
    switch(btn) {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=1;
                Xpos=0; Ypos=5; Zpos=5;
                Xo=0; Yo=0; Zo=0;
                Xl=0; Yl=1; Zl=0;
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                Xpos=0; Ypos=100; Zpos=0;
                Xo=0; Yo=0; Zo=0;
                Xl=0; Yl=0; Zl=1;
                zfar = 100;
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                Xpos=INITIAL_XPOS, Ypos=INITIAL_YPOS, Zpos=INITIAL_ZPOS;
                Xo=0, Yo=0, Zo=0;
                Xl=0, Yl=1, Zl=0;
                rot=0;
                zfar = INITIAL_ZFAR;
                glutPostRedisplay();
            }
            break;
    }
}

/******************************************************************/

void Keyboard(unsigned char key) {
    //movimentações da câmera
    if(key=='d') {
        Xo+=5;
        Yl=1;
        Zl=0;
    } else if(key=='a') {
        Xo-=5;
        Yl=1;
        Zl=0;
    } else if(key=='w') {
        Yo+=5;
        Yl=1;
        Zl=0;
    } else if(key=='s') {
        Yo-=5;
        Yl=1;
        Zl=0;
    } else if(key=='r') {
        rot+=5;
    } else if(key=='R') {
        rot-=5;
    } else if(key==27) {
        exit(0);
    } else if(key=='g') { //tecla g é utilizada para iniciar a trajetória
        pause = !pause;
        glutTimerFunc(0,FollowCurve,0);
    }

    //rotações
    // primeira esfera
    else if(key=='i') {
        angle1 = (angle1 - 5) % 360;
    } else if(key=='I') {
        angle1 = (angle1 + 5) % 360;
    } else if(key=='o') {
        angle2 = (angle2 - 5) % 360;
    } else if(key=='O') {
        angle2 = (angle2 + 5) % 360;
    } else if(key=='p') {
        angle3 = (angle3 - 5) % 360;
    } else if(key=='P') {
        angle3 = (angle3 + 5) % 360;
    }

    //segunda esfera
    else if(key=='j') {
        angle4 = (angle4 - 5) % 360;
    } else if(key=='J') {
        angle4 = (angle4 + 5) % 360;
    } else if(key=='k') {
        angle5 = (angle5 - 5) % 360;
    } else if(key=='K') {
        angle5 = (angle5 + 5) % 360;
    } else if(key=='l') {
        angle6 = (angle6 - 5) % 360;
    } else if(key=='L') {
        angle6 = (angle6 + 5) % 360;
    }

    //terceira esfera
    else if(key=='b') {
        angle7 = (angle7 - 5) % 360;
    } else if(key=='B') {
        angle7 = (angle7 + 5) % 360;
    } else if(key=='n') {
        angle8 = (angle8 - 5) % 360;
    } else if(key=='N') {
        angle8 = (angle8 + 5) % 360;
    } else if(key=='m') {
        angle9 = (angle9 - 5) % 360;
    } else if(key=='M') {
        angle9 = (angle9 + 5) % 360;
    }

    //movimento linear da garra
    else if(key=='+') {
        if(pos6 - pos3 < 1) {
            pos3 -= 0.02;
            pos6 += 0.02;
        }
    }
    else if(key=='-') {
        if((pos6 - pos3)>0) {
            pos6 -= 0.02;
            pos3 += 0.02;
        }
    }

    glutPostRedisplay();
}

/******************************************************************/

int main(int argc, char **argv) {

    srand(time(NULL));

    rand_x = rand_number(-4, 4);
    rand_z = rand_number(-4, 4);

    printf("RAND_X:%lf, RAND_Z:%lf", rand_x, rand_z);

    glutInit(&argc, argv);

    glEnable(GL_DEPTH_TEST);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_SIZE)/2), ((glutGet(GLUT_SCREEN_HEIGHT))-WINDOW_SIZE)/2);
    glutCreateWindow("Braco");

    glutDisplayFunc(Display);
    glutMouseFunc((void*)Mouse);
    glutKeyboardFunc((void*)Keyboard);

    glutMainLoop();

    return 0;
}

int gluInvertMatrix(const double m[16], double invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return -1;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return 1;
}

static inline void Matrix4x4MultiplyBy4x4 (GLdouble *src1, GLdouble *src2, struct world *w)
{
  
    w->x = (*(src1+O(1,0)) * *(src2+O(0,1))) + (*(src1+O(1,1)) * *(src2+O(1,1))) + (*(src1+O(1,2)) * *(src2+O(2,1))) + (*(src1+O(1,3)) * *(src2+O(3,1))); 
    w->y = (*(src1+O(1,0)) * *(src2+O(0,2))) + (*(src1+O(1,1)) * *(src2+O(1,2))) + (*(src1+O(1,2)) * *(src2+O(2,2))) + (*(src1+O(1,3)) * *(src2+O(3,2))); 
    w->z = (*(src1+O(1,0)) * *(src2+O(0,3))) + (*(src1+O(1,1)) * *(src2+O(1,3))) + (*(src1+O(1,2)) * *(src2+O(2,3))) + (*(src1+O(1,3)) * *(src2+O(3,3)));
};

