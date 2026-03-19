#include "sky.cpp"
#include "foreground.cpp"
#include "landscape.cpp"


void init(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    sky();

    // city landscape
    // mountain
    ground();
    rail();
    // train

    glutSwapBuffers();
}

int main(int argc, char **argv){
    //glutReshapeFunc()
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Graphics Project");
    glutPassiveMotionFunc(onMouseMove);
    glutMouseFunc(onMouseClick);
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}