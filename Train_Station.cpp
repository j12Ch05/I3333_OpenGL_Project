#include <cmath>
#include <cstdlib>
#include <GL/glut.h>

#define M_PI 3.14

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;


void drawFilledCircle(float cx, float cy, float r, int segments = 64, float startAngle = 0.0, float sweep = 2.0 * M_PI){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float theta = startAngle + (sweep * i) / segments;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawRect(float x, float y, float w, float h){
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawRails(){

    float railTopY = 150.0;
    float railBottomY = 130.0;
    float left = -50.0;
    float right = WINDOW_WIDTH + 50.0;

    // Sleepers
    glColor3f(0.4, 0.25, 0.15);
    const int numSleepers = 40;
    float span = (right - left) / numSleepers;
    float sleeperW = 14.0;

    const float overlap = 2.0;
    float sleeperY = railBottomY - overlap;
    const float railHeight = 6.0;
    float sleeperH = (railTopY + railHeight) - sleeperY + overlap;

    for (int i = 0; i < numSleepers; ++i) {
        float sx = left + i * span - sleeperW / 2.0;
        drawRect(sx, sleeperY, sleeperW, sleeperH);
    }

    
    glColor3f(0.15, 0.15, 0.15);
    drawRect(left, railBottomY, right - left, railHeight);
    drawRect(left, railTopY, right - left, railHeight);
}

void drawHill(){

    glColor3f(0.18, 0.6, 0.25);
    drawFilledCircle(WINDOW_WIDTH * 0.5, -200.0, 700.0, 160, 0.0, M_PI);
}

void drawStation(){

    // Platform
    glColor3f(0.8, 0.78, 0.72);
    drawRect(120.0, 170.0, 760.0, 40.0);

    // Platform edge stripe
    glColor3f(0.95, 0.9, 0.2);
    drawRect(120.0, 208.0, 760.0, 6.0);

    // Station building base
    float bx = 360.0, by = 208.0;
    float bw = 280.0, bh = 110.0;
    glColor3f(0.86, 0.6, 0.45);
    drawRect(bx, by, bw, bh);

    // Roof
    glColor3f(0.55, 0.13, 0.13);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 10.0, by + bh);
    glVertex2f(bx + bw + 10.0, by + bh);
    glVertex2f(bx + bw / 2.0, by + bh + 60.0);
    glEnd();

    // Windows
    glColor3f(0.95, 0.95, 1.0);
    float ww = 40.0, wh = 50.0;
    float wy = by + 30.0;
    float doorCenterX = bx + bw / 2.0;
    float windowCenterOffset = 80.0;
    float wxLeft = doorCenterX - windowCenterOffset - ww / 2.0;
    float wxRight = doorCenterX + windowCenterOffset - ww / 2.0;

    // left window
    drawRect(wxLeft, wy, ww, wh);
    glColor3f(0.2, 0.2, 0.25);
    glBegin(GL_LINES);
    glVertex2f(wxLeft + ww / 2.0, wy);
    glVertex2f(wxLeft + ww / 2.0, wy + wh);
    glVertex2f(wxLeft, wy + wh / 2.0);
    glVertex2f(wxLeft + ww, wy + wh / 2.0);
    glEnd();

    // right window
    glColor3f(0.95, 0.95, 1.0);
    drawRect(wxRight, wy, ww, wh);
    glColor3f(0.2, 0.2, 0.25);
    glBegin(GL_LINES);
    glVertex2f(wxRight + ww / 2.0, wy);
    glVertex2f(wxRight + ww / 2.0, wy + wh);
    glVertex2f(wxRight, wy + wh / 2.0);
    glVertex2f(wxRight + ww, wy + wh / 2.0);
    glEnd();

    // Door
    glColor3f(0.35, 0.18, 0.15);
    drawRect(bx + bw / 2.0 - 22.0, by, 44.0, 70.0);
}

void drawSky(){
  
    glBegin(GL_QUADS);
    glColor3f(0.53, 0.8, 0.98);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnd();

    // Sun
    glColor3f(1.0, 0.95, 0.6);
    drawFilledCircle(WINDOW_WIDTH - 120.0, WINDOW_HEIGHT - 120.0, 40.0, 48);
}

void display(){

    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawHill();

    // A darker ground behind rails
    glColor3f(0.25, 0.5, 0.2);
    drawRect(0.0, 0.0, WINDOW_WIDTH, 140.0);

    drawRails();

    drawStation();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Train Station ");

    // Background color(sky)
    glClearColor(0.68, 0.9, 1.0, 1.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    return 0;
}