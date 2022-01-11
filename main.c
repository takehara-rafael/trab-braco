#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/freeglut.h>
#include <math.h>

#define WINDOW_SIZE 700

struct parte {
    int ang_x, ang_y, ang_z;
    double pos_x, pos_y, pos_z;
} dedo_esq, dedo_dir, garra, cubo;

int proj=0;
int aux=1, i=0;
float posX=0, posY=15, posZ=25,y=0,z=0;
int oX=0, oY=0, oZ=0;
int lX=0, lY=1, lZ=0;

int rot=0;

int angulo1 = 0, angulo2 = 0, angulo3 = 0, angulo4 = 0, angulo5 = 0, angulo6 = 0,
angulo7 = 0, angulo8 = 0, angulo9 = 0;

int grab = 0;

double rand_x = 0, rand_z = 0;

double transMatrix[16];

void Display();
void Mouse(int btn, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void SpecKeys(int key, int x, int y);
void BuildScene();
void BuildArm();
void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);
double rand_number();


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
        glColor3ub(255,0,255);
        if(!grab)
            DrawCube(rand_x, 2.6, rand_z, 0.5);
        glPopMatrix();

    glPushMatrix();
    BuildArm();
    glPopMatrix();

    glPushMatrix();
    BuildScene();
    glPopMatrix();

    glutSwapBuffers();
}

/******************************************************************/

void BuildScene() {

    glPushMatrix();
    
    //chão
    glPushMatrix();
        glColor3ub(255, 255, 255);
        glScalef(1, 0.0001, 1);
        glutSolidCube(100.0);
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
    //desenho do caminho da camera
    glPushMatrix();
             
                 

       for(i=-25;i<=25;i++){
       glBegin(GL_LINES);
        y=15+2*sin(i*(3.14/4.0));
        z=sqrt(pow(25,2)-pow(i,2));
        glVertex3f(i,y,z);  
        y=15+2*sin((i+1)*(3.14/4.0));
        z=sqrt(pow(25,2)-pow((i+1),2));
        glVertex3f(i+1,y,z);
        glEnd();
        glBegin(GL_LINES);
        y=15+2*sin(i*(3.14/4.0));
        z=-sqrt(pow(25,2)-pow(i,2));
        glVertex3f(i,y,z);  
        y=15+2*sin((i+1)*(3.14/4.0));
        z=-sqrt(pow(25,2)-pow((i+1),2));
        glVertex3f(i+1,y,z); 
        glEnd();           
       }

    glPopMatrix();
    
    glPopMatrix();
}

/******************************************************************/

void BuildArm() {

    glPushMatrix();

    //primeiro elo
    glColor3ub(51, 51, 255);
    glTranslatef(0, 2.5, 0);
    glutSolidCube(1.0);

    //primeira esfera
    glColor3f(1,0,0);
    glTranslatef(0,1,0);
    glScalef(0.6,0.6,0.6);
    glRotatef(angulo1, 1, 0, 0);
    glRotatef(angulo2, 0, 1, 0);
    glRotatef(angulo3, 0, 0, 1);
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
    glRotatef(angulo4, 1, 0, 0);
    glRotatef(angulo5, 0, 1, 0);
    glRotatef(angulo6, 0, 0, 1);
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
    glRotatef(garra.ang_x, 1, 0, 0);
    glRotatef(garra.ang_y, 0, 1, 0);
    glRotatef(garra.ang_z, 0, 0, 1);
    glutSolidSphere(0.7,20,20);

    //garra
    glColor3f(0,1,0);
    glTranslatef(0.85,0,0); 
    glScalef(1,1,5);
    glutSolidCube(0.5);

    glPushMatrix();
    glGetDoublev(GL_MODELVIEW_MATRIX, transMatrix);
    
    garra.pos_x = transMatrix[14] + 27;
    garra.pos_y = transMatrix[15] - 1;
    garra.pos_z = transMatrix[16];

    glPointSize(20.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
        glVertex3f(garra.pos_x,garra.pos_y,garra.pos_z);
    glEnd();
    glPopMatrix();

    
    if(abs(garra.pos_x - rand_x) < 0.5 && abs(garra.pos_y - 2.6) < 0.5 && abs(garra.pos_z - rand_z) < 0.5) {
        grab = 1;
        DrawCube(0.2,0,0, 0.5);
    }


    glPushMatrix(); 
        glColor3f(0,1,0);
        glTranslatef(0,0,dedo_esq.pos_z);
        glTranslatef(0.75,0,-0.1);
        glScalef(5,0.5,0.5);
        glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0,1,0);
        glTranslatef(0,0,dedo_dir.pos_z);
        glTranslatef(0.75,0, 0.1);
        glScalef(5,0.5,0.5);
        glutSolidCube(0.25);
    glPopMatrix();

    glPopMatrix();

}

/******************************************************************/

double rand_number(double min, double max) {

    int choice;
    double num;

    choice = rand() % 2; // 0 ou 1
    if(choice)
    {
        num = (((double)rand()/(double)RAND_MAX)*min); //gera número entre min e 0
    }
    else
    {
        num = (((double)rand()/(double)RAND_MAX)*max); // gera número entre 0 e max
    }

    return num;
}

void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
{
    GLfloat halfSideLength = edgeLength * 0.5;
    
    GLfloat vertices[] = {

        //face frontal
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // superior esquerdo
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // superior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // inferior direito
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // inferior esquerdo

        //face distal
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // superior esquerdo
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // superior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // inferior direito
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // inferior esquerdo

        //face lateral esquerda
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // superior esquerdo
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // superior direito
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // inferior direito
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // inferior esquerdo

        //face lateral direita
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // superior esquerdo
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // superior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // inferior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // inferior esquerdo

        //face superior
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // superior esquerdo
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // superior direito
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // inferior direito
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // inferior esquerdo

        //face inferior
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // superior esquerdo
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // superior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // inferior direito
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength // inferior esquerdo
    };
        glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_QUADS, 0, 24);
        glDisableClientState(GL_VERTEX_ARRAY);
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
    //caminho da camera
      else if(key=='.'){
        if(posX==-25){
            aux=1;
        }
        else if (posX == 25){
            aux=-1;
        } 
        posX=posX+aux; posY=15+2*sin(posX*(3.14/4.0));        
        if(aux==1){
            posZ=sqrt(pow(25,2)-pow(posX,2));
        }
        else{
            posZ=-sqrt(pow(25,2)-pow(posX,2));
        }
        oX=0; oY=0; oZ=0;
        lX=0; lY=1; lZ=0;

    } else if(key==','){
        if(posX==-25){
            aux=-1;
        }
        else if (posX == 25){
            aux=1;
        } 
        posX=posX-aux; posY=15+2*sin(posX*(3.14/4.0));
        if(aux==1){
            posZ=sqrt(pow(25,2)-pow(posX,2));
        }
        else{
            posZ=-sqrt(pow(25,2)-pow(posX,2));
        }                
        oX=0; oY=0; oZ=0;
        lX=0; lY=1; lZ=0;
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
        garra.ang_x = (garra.ang_x - 5) % 360;
    }
    else if(key=='B') {
        garra.ang_x = (garra.ang_x + 5) % 360;
    }
       else if(key=='n') {
        garra.ang_y = (garra.ang_y - 5) % 360;
    }
    else if(key=='N') {
        garra.ang_y = (garra.ang_y + 5) % 360;
    }
       else if(key=='m') {
        garra.ang_z = (garra.ang_z - 5) % 360;
    }
    else if(key=='M') {
        garra.ang_z = (garra.ang_z + 5) % 360;
    }

    //movimento linear da garra
    else if(key=='+') {
        grab = 0;
           if(dedo_dir.pos_z - dedo_esq.pos_z <= 0.1) {
            dedo_esq.pos_z -= 0.01;
            dedo_dir.pos_z += 0.01;
        }
    }
    else if(key=='-') {
        grab = 0;
        if((dedo_dir.pos_z - dedo_esq.pos_z)>0) {
            dedo_dir.pos_z -= 0.01;
            dedo_esq.pos_z += 0.01;
        }
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
    glutSpecialFunc(SpecKeys);

    glutMainLoop();

    return 0;
}
