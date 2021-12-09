#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>

#define WINDOW_SIZE 700

int proj=0;

int posX=0, posY=15, posZ=25;
int oX=0, oY=0, oZ=0;
int lX=0, lY=1, lZ=0;

int rot=0;

int angulo1 = 0, angulo2 = 0, angulo3 = 0, angulo4 = 0, angulo5 = 0, angulo6 = 0,
angulo7 = 0, angulo8 = 0, angulo9 = 0;

void Display();
void Mouse(int btn, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void SpecKeys(int key, int x, int y);
void BuildScene();
void BuildArm();


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
        gluPerspective(45,1,1,50);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(posX, posY, posZ, oX, oY, oZ, lX, lY, lZ);

    glRotatef(rot, lX, lY, lZ);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    BuildScene();
    glPopMatrix();

    glPushMatrix();
    BuildArm();
    glPopMatrix();

    glutSwapBuffers();
}

/******************************************************************/

void BuildScene() {
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glScalef(1, 0.0001, 1);
    glutSolidCube(100.0);
    glPopMatrix();


    glPushMatrix();
    glColor3ub(150, 75, 0);

    glPushMatrix();
    glTranslatef(0, 2, 0);
    glScalef(1, 0.02, 1);
    glutSolidCube(20.0);
    glPopMatrix();

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

    glPopMatrix();
}

/******************************************************************/

void BuildArm() {

    //primeiro elo
    glPushMatrix();
    glColor3ub(51, 51, 255);
    glTranslatef(0, 2.5, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(1,0,0);
    glTranslatef(0,3.2,0);
    glScalef(0.6,0.6,0.6);
    //primeira esfera
    glRotatef(angulo1, 1, 0, 0);
    glRotatef(angulo2, 0, 1, 0);
    glRotatef(angulo3, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //primeiro elo
    glColor3f(0,1,0);
    glTranslatef(0,2,0);
    glPushMatrix();
        glScalef(1.5,10,1);
        glutSolidCube(0.5);
    glPopMatrix();

    //segunda esfera
    glColor3f(1,0,0);
    glTranslatef(0,3,0);
    glRotatef(angulo4, 1, 0, 0);
    glRotatef(angulo5, 0, 1, 0);
    glRotatef(angulo6, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //segundo elo
    glColor3f(0,1,0);
    glTranslatef(2.5,0,0);
    glPushMatrix();
        glScalef(10,1.5,1);
        glutSolidCube(0.5);
    glPopMatrix();

    //terceira esfera
    glColor3f(1,0,0);
    glTranslatef(3,0,0);
    glRotatef(angulo7, 1, 0, 0);
    glRotatef(angulo8, 0, 1, 0);
    glRotatef(angulo9, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //garra
    glColor3f(0,1,0);
    glTranslatef(0.85,0,0); 
    glPushMatrix();
        glScalef(1,1,5);
        glutSolidCube(0.5);
        glPushMatrix();
            glTranslatef(0.75,0,-0.2);
            glScalef(5,0.5,0.5);
            glutSolidCube(0.25);
        glPopMatrix();
            glTranslatef(0.75,0, 0.2);
            glScalef(5,0.5,0.5);
            glutSolidCube(0.25);
    glPopMatrix();


}

/******************************************************************/

void Mouse(int btn, int state, int x, int y) {
    switch(btn) {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=1;
                posX=0; posY=5; posZ=5;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                posX=0; posY=20; posZ=20;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                posX=30; posY=0; posZ=0;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
    }

}

/******************************************************************/

void Keyboard(unsigned char key, int x, int y) {
    //movimentações da câmera
    if(key=='d') {
        oX+=5;
        lY=1;
        lZ=0;
    } else if(key=='a') {
        oX-=5;
        lY=1;
        lZ=0;
    } else if(key=='w') {
        oY+=5;
        lY=1;
        lZ=0;
    } else if(key=='s') {
        oY-=5;
        lY=1;
        lZ=0;
    } else if(key=='r') {
        rot+=5;
    } else if(key=='R') {
        rot-=5;
    } else if(key==27) {
        exit(0);
    } else if(key=='p') {
        proj=0;
        posX=0, posY=15, posZ=25;
        oX=0, oY=0, oZ=0;
        lX=0, lY=1, lZ=0;
        rot=0;
    }
    //rotações
    
    // primeira esfera
    else if(key=='u') {
        angulo1 = (angulo1 - 5) % 360;
    }
    else if(key=='U') {
        angulo1 = (angulo1 + 5) % 360;
    }
        else if(key=='i') {
        angulo2 = (angulo2 - 5) % 360;
    }
    else if(key=='I') {
        angulo2 = (angulo2 + 5) % 360;
    }
        else if(key=='o') {
        angulo3 = (angulo3 - 5) % 360;
    }
    else if(key=='O') {
        angulo3 = (angulo3 + 5) % 360;
    }


    //segunda esfera
    else if(key=='j') {
        angulo4 = (angulo4 - 5) % 360;
    }
    else if(key=='J') {
        angulo4 = (angulo4 + 5) % 360;
    }
    else if(key=='k') {
        angulo5 = (angulo5 - 5) % 360;
    }
    else if(key=='K') {
        angulo5 = (angulo5 + 5) % 360;
    }
       else if(key=='l') {
        angulo6 = (angulo6 - 5) % 360;
    }
    else if(key=='L') {
        angulo6 = (angulo6 + 5) % 360;
    }


    //terceira esfera
    else if(key=='b') {
        angulo7 = (angulo7 - 5) % 360;
    }
    else if(key=='B') {
        angulo7 = (angulo7 + 5) % 360;
    }
       else if(key=='n') {
        angulo8 = (angulo8 - 5) % 360;
    }
    else if(key=='N') {
        angulo8 = (angulo8 + 5) % 360;
    }
       else if(key=='m') {
        angulo9 = (angulo9 - 5) % 360;
    }
    else if(key=='M') {
        angulo9 = (angulo9 + 5) % 360;
    }



    glutPostRedisplay();
}

/******************************************************************/

void SpecKeys(int key, int x, int y) {
    if(key==GLUT_KEY_RIGHT) {
        posX+=5;
        oX+=5;
    } else if (key==GLUT_KEY_LEFT) {
        posX-=5;
        oX-=5;
    } else if (key==GLUT_KEY_UP) {
        posY+=5;
        oY+=5;
    } else if (key==GLUT_KEY_DOWN) {
        posY-=5;
        oY-=5;
    } else if (key==GLUT_KEY_PAGE_UP) {
        posZ+=5;
        oZ+=5;
    } else if (key==GLUT_KEY_PAGE_DOWN) {
        posZ-=5;
        oZ-=5;
    }
    glutPostRedisplay();
}

/******************************************************************/

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_SIZE)/2), ((glutGet(GLUT_SCREEN_HEIGHT))-WINDOW_SIZE)/2);
    glutCreateWindow("Braco");

    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecKeys);

    glutMainLoop();

    return 0;
}
