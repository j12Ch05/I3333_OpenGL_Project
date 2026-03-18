#include <GL/glut.h>
#include <cmath>
using namespace std;

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

float cloudOffset1 = 0.0f;
float cloudOffset2 = 250.0f;
float cloudOffset3 = 520.0f;

void drawCircle(float centerX, float centerY, float radius, int segments = 40)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        glVertex2f(centerX + cos(angle) * radius, centerY + sin(angle) * radius);
    }
    glEnd();
}

void cloud(float x, float y, float scale)
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 28.0f * scale);
    drawCircle(x + 28.0f * scale, y + 12.0f * scale, 34.0f * scale);
    drawCircle(x + 60.0f * scale, y + 4.0f * scale, 30.0f * scale);
    drawCircle(x + 90.0f * scale, y + 10.0f * scale, 24.0f * scale);
    glEnd();
}

void sky(){
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.82f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(WINDOW_WIDTH, 0.0f);

    glColor3f(0.08f, 0.4f, 0.88f);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0.0f, WINDOW_HEIGHT);
    glEnd();

    cloud(cloudOffset1, 500.0f, 0.5f);
    cloud(cloudOffset2, 530.0f, 0.45f);
    cloud(cloudOffset3, 475.0f, 0.3f);
}

void moveCloud(float &cloudX, float speed, float scale)
{
    cloudX -= speed;
    if (cloudX < -(140.0f * scale))
    {
        cloudX = WINDOW_WIDTH + 50.0f;
    }
}

void update(int)
{
    moveCloud(cloudOffset1, 1.1f, 1.2f);
    moveCloud(cloudOffset2, 0.8f, 0.95f);
    moveCloud(cloudOffset3, 1.4f, 1.05f);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    sky();

    // city landscape
    // the SUN
    // mountain
    // ground
    // train

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    //glutReshapeFunc()
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Graphics Project");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}
