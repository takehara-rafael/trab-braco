/*
Integrantes:
    André Antunes Marques
    Gabriel Rücker
    Marcos Mendonça Vilela
    Rafael Yukio Takehara Carvalho
*/

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
} w, elo2, esf2, elo3, esf3;

double pi = M_PI;

int proj=0;
double zfar = INITIAL_ZFAR;
float Xpos=INITIAL_XPOS, Ypos=INITIAL_YPOS, Zpos=INITIAL_ZPOS;
int Xo=0, Yo=0, Zo=0;
int Xl=0, Yl=1, Zl=0;
float vertices[POINT_QNT*3];
int rot=0;

int angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0, angle7 = 0, angle8 = 0, angle9 = 0;

float pos3=0, pos6=0;

int pause=1, choice=0, grab=0, rMove=0;

double rand_x = 0, rand_z = 0;

GLdouble matrix_cube[16], matrix_scene[16], proj_matrix[16]; 

GLdouble camViewMatrix[16], invCamViewMatrix[16];

GLint vp[4];

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
void RandomMovement(int);

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

        if((pos6 - pos3) < (-0.3)) {
                if (
                    (w.x < rand_x+0.5 && w.x > rand_x-0.5) &&
                    (w.y < 3 && w.y > 2.2) &&
                    (w.z < rand_z+0.5 && w.z > rand_z-0.5)) {
                    grab = 1;
                }
        }


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
    for(int i=-INITIAL_ZPOS; i<=INITIAL_ZPOS; i++) {
        vertices[j] = i;

        j++;
        vertices[j] = 15+(4*sin(i*(pi/18)));

        j++;
        vertices[j]= -sqrt(pow(INITIAL_ZPOS,2)-pow(i,2));

        j++;
    }
    for(int i=INITIAL_ZPOS; i>=-INITIAL_ZPOS; i--) {
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
    glScalef(1,1,0.2);
    glPopMatrix();

    if(grab) {
        glutTimerFunc(0,moveHand,grab);
        glPushMatrix();
            glColor3ub(255,0,255);
            DrawCube(0.5,0,0,0.5);
        glPopMatrix();
    }

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

    if(grab) {
        if ((pos6 - pos3) >= -1) {
            pos6 -= 0.02;
            pos3 += 0.02;
        }

        glutPostRedisplay();
        glutTimerFunc(30000 / 15, moveHand, 0);
    }
}

/******************************************************************/

void DrawCube(GLfloat centerXpos, GLfloat centerYpos, GLfloat centerZpos, GLfloat edgeLength) {

    glTranslatef(centerXpos, centerYpos, centerZpos);
    glutSolidCube(edgeLength);

    glGetDoublev(GL_MODELVIEW_MATRIX, matrix_cube);

}

/******************************************************************/

void RandomMovement(int timerOn) {

    if(rMove) {

        int opt = rand() % 9;
        switch (opt) {
            case 0:
                angle1 = (angle1 - 5) % 360;
                break;
            case 1:
                angle2 = (angle2 - 5) % 360;
                break;
            case 2:
                angle3 = (angle3 - 5) % 360;
                break;
            case 3:
                angle4 = (angle4 - 5) % 360;
                break;
            case 4:
                angle5 = (angle5 - 5) % 360;
                break;
            case 5:
                angle6 = (angle6 - 5) % 360;
                break;
            case 6:
                angle7 = (angle7 - 5) % 360;
                break;
            case 7:
                angle8 = (angle8 - 5) % 360;
                break;
            case 8:
                angle9 = (angle9 - 5) % 360;
                break;
        }

        glutPostRedisplay();
        glutTimerFunc(2000/15, RandomMovement, 0);
    }

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
    }

    else if(key==27) { //tecla ESC é utilizada para finalizar o programa
        exit(0);
    } else if(key=='g') { //tecla g é utilizada para iniciar a trajetória
        pause = !pause;
        glutTimerFunc(0,FollowCurve,0);
    } else if(key=='z') { //tecla z é utilizada para mudar a posição do cubo
        rand_x = rand_number(-4, 4);
        rand_z = rand_number(-4, 4);
    } else if(key=='x') { //tecla x é usada para soltar o cubo
        grab = 0;
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

    else if(key=='.') {
        rMove = !rMove;
        glutTimerFunc(0,RandomMovement,0);
    }


    //movimento linear da garra
    else if(key=='+') {
        if((pos6 - pos3) < 1) {
            pos3 -= 0.02;
            pos6 += 0.02;
        }
    }
    else if(key=='-') {
        if((pos6 - pos3)>-0.5) {
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

int gluInvertMatrix(const double m[16], double invOut[16]) {
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

    if (det == 0) {
        return -1;
    }
    det = 1.0 / det;

    for (i = 0; i < 16; i++) {
        invOut[i] = inv[i] * det;
    }
    return 1;
}

static inline void Matrix4x4MultiplyBy4x4 (GLdouble *src1, GLdouble *src2, struct world *w) {
w->x = (*(src1+O(0,0)) * *(src2+O(0,3))) + (*(src1+O(0,1)) * *(src2+O(1,3))) + (*(src1+O(0,2)) * *(src2+O(2,3))) + (*(src1+O(0,3)) * *(src2+O(3,3))); 
w->y = (*(src1+O(1,0)) * *(src2+O(0,3))) + (*(src1+O(1,1)) * *(src2+O(1,3))) + (*(src1+O(1,2)) * *(src2+O(2,3))) + (*(src1+O(1,3)) * *(src2+O(3,3))); 
w->z = (*(src1+O(2,0)) * *(src2+O(0,3))) + (*(src1+O(2,1)) * *(src2+O(1,3))) + (*(src1+O(2,2)) * *(src2+O(2,3))) + (*(src1+O(2,3)) * *(src2+O(3,3))); 
};


