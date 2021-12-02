#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>

int proj=0;

int posX=0, posY=5, posZ=10;
int oX=0, oY=0, oZ=0;
int lX=0, lY=1, lZ=0;

int rot=0;

void Display();
void Mouse(int btn, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void SpecKeys(int key, int x, int y);

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
        glOrtho(-50, 50, -50, 50, -50, 50);
    } else {
        gluPerspective(45,1,1,50);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(posX, posY, posZ, oX, oY, oZ, lX, lY, lZ);
    
    glRotatef(rot, lX, lY, lZ);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
   glColor3ub(255, 255, 255);
   glutWireCube(1.0);
   
   glutSwapBuffers();
}

/******************************************************************/

void Mouse(int btn, int state, int x, int y) {
    switch(btn) {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=1;
                posX=0; posY=10; posZ=20;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                posX=0; posY=10; posZ=20;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN) {
                proj=0;
                posX=20; posY=0; posZ=0;
                oX=0; oY=0; oZ=0;
                lX=0; lY=1; lZ=0;
                glutPostRedisplay();
            }
            break;
    }

}

/******************************************************************/

void Keyboard(unsigned char key, int x, int y) {
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
    
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Braco");
    
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecKeys);
    
    glutMainLoop();

    return 0;
    }
