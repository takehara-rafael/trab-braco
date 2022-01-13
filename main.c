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

double pi = 22/7;

int proj=0;
double zfar = INITIAL_ZFAR;
float Xpos=INITIAL_XPOS, Ypos=INITIAL_YPOS, Zpos=INITIAL_ZPOS;
int Xo=0, Yo=0, Zo=0;
int Xl=0, Yl=1, Zl=0;
float vertices[POINT_QNT*3];
int rot=0;

int angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0, angle7 = 0, angle8 = 0, angle9 = 0;

float pos1=0, pos2=0, pos3=0, pos4=0, pos5=0, pos6=0;

int pause=1, choice=0;

double rand_x = 0, rand_z = 0;

void Display();
void Mouse(int btn, int state);
void Keyboard(unsigned char key);
void BuildScene();
void BuildArm();
void DrawPath();
void DrawCube(GLfloat centerXpos, GLfloat centerYpos, GLfloat centerZpos, GLfloat edgeLength);
void FollowCurve(int);
double rand_number(double min, double max);

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

    glRotatef(rot, Xl, Yl, Zl);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3ub(255,0,255);
    DrawCube(rand_x, 2.6, rand_z, 0.5);

    glPushMatrix();
    BuildScene();
    glPopMatrix();

    glPushMatrix();
    BuildArm();
    glPopMatrix();

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
    glColor3f(0,1,0);
    glTranslatef(0.85,0,0);
    glScalef(1,1,5);
    glutSolidCube(0.5);

    glPushMatrix();
    glTranslatef(0,0, pos3);
    glTranslatef(0.75,0,-0.1);
    glScalef(5,0.5,0.5);
    glutSolidCube(0.25);
    glPopMatrix();

    glTranslatef(0,0, pos6);
    glTranslatef(0.75,0, 0.1);
    glScalef(5,0.5,0.5);
    glutSolidCube(0.25);
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

void DrawCube(GLfloat centerXpos, GLfloat centerYpos, GLfloat centerZpos, GLfloat edgeLength) {
    GLfloat halfSideLength = edgeLength * 0.5;

    GLfloat vertices[] = {

            //face frontal
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // superior esquerdo
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // superior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength, // inferior direito
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength, // inferior esquerdo

            //face distal
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // superior esquerdo
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // superior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // inferior direito
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // inferior esquerdo

            //face lateral esquerda
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // superior esquerdo
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // superior direito
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // inferior direito
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength, // inferior esquerdo

            //face lateral direita
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // superior esquerdo
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // superior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // inferior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength, // inferior esquerdo

            //face superior
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // superior esquerdo
            centerXpos - halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // superior direito
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos - halfSideLength, // inferior direito
            centerXpos + halfSideLength, centerYpos + halfSideLength, centerZpos + halfSideLength, // inferior esquerdo

            //face inferior
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength, // superior esquerdo
            centerXpos - halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // superior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos - halfSideLength, // inferior direito
            centerXpos + halfSideLength, centerYpos - halfSideLength, centerZpos + halfSideLength // inferior esquerdo
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_VERTEX_ARRAY);
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
        if(pos6 - pos3 <= 0.1) {
            pos3 -= 0.01;
            pos6 += 0.01;
        }
    }
    else if(key=='-') {
        if((pos6 - pos3)>0) {
            pos6 -= 0.01;
            pos3 += 0.01;
        }
    }

    glutPostRedisplay();
}

/******************************************************************/

int main(int argc, char **argv) {

    srand(time(NULL));

    rand_x = rand_number(-6, 6);
    rand_z = rand_number(-6, 6);

    glutInit(&argc, argv);

    glEnable(GL_DEPTH_TEST);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_SIZE)/2), ((glutGet(GLUT_SCREEN_HEIGHT))-WINDOW_SIZE)/2);
    glutCreateWindow("Braco");

    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();

    return 0;
}
