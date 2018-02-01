#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>


void changeSize(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    float ratio = (float) width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45, ratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
}

float angleZ = 0.0f, angleX = 0.0f, red = 0.0f, green = 0.0f, blue = 0.0f;

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 10.0f,
              0.0f, 0.0f,  0.0f,
              0.0f, 1.0f,  0.0f);

    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    glRotatef(angleX, 0.0f, 1.0f, 0.0f);

    glColor4f(red, green, blue, 0.5f);
    glBegin(GL_POLYGON);
        glVertex3f(-5.0f, -5.0f, -5.0f);
        glVertex3f(-5.0f, 5.0f, -5.0f);
        glVertex3f(5.0f, 5.0f, -5.0f);
        glVertex3f(5.0f, -5.0f, -5.0f);
    glEnd();



    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
        case 66:
            red = 0.0f;
            green = 0.0f;
            blue = 0.0f;
            break;
        case 98 :
            red = 0.0;
            green = 0.0;
            blue = 1.0;
            break;
        case 103 :
            red = 0.0;
            green = 1.0;
            blue = 0.0;
            break;
        case 114 :
            red = 1.0;
            green = 0.0;
            blue = 0.0;
            break;
        case 119:
            red = 1.0f;
            green = 1.0f;
            blue = 1.0f;
            break;
        default:
            printf("%i ", key);
    }
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            angleZ += 1.0f;
            break;
        case GLUT_KEY_LEFT:
            angleZ -= 1.0f;
            break;
        case GLUT_KEY_UP:
            angleX += 1.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX -= 1.0f;
            break;
    }
}

int main(int argc, char **argv) {

    for(int i = 0; i < 128; i++) {
        printf("%i: %c\n", i, i);
    }
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,640);
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
