// main land, rail & train

#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

float GROUND_HEIGHT = 150.0f;

// Circle helper
void drawTrainCircle(float cx, float cy, float r, int segments = 40){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.1415926f * i / segments;
        glVertex2f(cx + cosf(angle) * r, cy + sinf(angle) * r);
    }
    glEnd();
}

// Wheel spokes
void drawWheel(float cx, float cy, float r, float rotation){
    glColor3f(0.10f, 0.10f, 0.12f);
    drawTrainCircle(cx, cy, r, 48);

    glColor3f(0.65f, 0.65f, 0.70f);
    drawTrainCircle(cx, cy, r * 0.55f, 36);

    glColor3f(0.85f, 0.85f, 0.88f);
    drawTrainCircle(cx, cy, r * 0.14f, 24);

    glPushMatrix();
    // Spin wheel spokes
    glTranslatef(cx, cy, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glColor3f(0.92f, 0.92f, 0.95f);
    for (int i = 0; i < 6; ++i) {
        glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-1.5f, 0.0f);
        glVertex2f(1.5f, 0.0f);
        glVertex2f(1.5f, r * 0.82f);
        glVertex2f(-1.5f, r * 0.82f);
        glEnd();
    }
    glPopMatrix();
}

// Smoke puff
void drawSmokePuff(float x, float y, float size, float alpha){
    glColor4f(0.85f, 0.85f, 0.88f, alpha);
    drawTrainCircle(x, y, size, 28);
    glColor4f(0.92f, 0.92f, 0.95f, alpha * 0.8f);
    drawTrainCircle(x + size * 0.45f, y + size * 0.15f, size * 0.72f, 24);
}

void ground(){
    glBegin(GL_QUADS);
    if(time == "day"){
        glColor3f(0.0, 1.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(WINDOW_WIDTH, 0.0f);

        glColor3f(0.0f, 0.6f, 0.0f);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);
        glVertex2f(0.0f, GROUND_HEIGHT);
        glEnd();
    }else{
        glColor3f(0.0, 0.7f, 0.2f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(WINDOW_WIDTH, 0.0f);

        glColor3f(0.0f, 0.4f, 0.2f);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);
        glVertex2f(0.0f, GROUND_HEIGHT);
        glEnd();
    }
}

void drawRect(float x, float y, float w, float h){
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void rail(){

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

void train(){
    // Time drives animation
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    // Wheel rotation angle
    float wheelRotation = -t * 280.0f;
    // Train base position
    float baseX = (WINDOW_WIDTH * 0.5f) - 120.0f;
    float baseY = 164.0f;

    glPushMatrix();
    glTranslatef(baseX, baseY, 0.0f);

    // Smoke
    for (int i = 0; i < 6; ++i) {
        float life = fmodf(t * 0.42f + i * 0.17f, 1.0f);
        float rise = life * 90.0f;
        float drift = -(life * 34.0f) + sinf(life * 6.283f + i) * 5.0f;
        float size = 10.0f + life * 12.0f;
        float alpha = 0.42f * (1.0f - life);
        drawSmokePuff(150.0f + drift, 118.0f + rise, size, alpha);
    }

    // Main body color
    glColor3f(0.78f, 0.06f, 0.10f);
    drawRect(0.0f, 18.0f, 210.0f, 58.0f);

    // Boiler section
    glColor3f(0.88f, 0.10f, 0.14f);
    drawRect(18.0f, 48.0f, 118.0f, 34.0f);

    // Front nose
    glBegin(GL_POLYGON);
    glColor3f(0.70f, 0.04f, 0.08f);
    glVertex2f(210.0f, 18.0f);
    glVertex2f(238.0f, 28.0f);
    glVertex2f(238.0f, 68.0f);
    glVertex2f(210.0f, 76.0f);
    glEnd();

    // Driver cabin
    glColor3f(0.74f, 0.05f, 0.09f);
    drawRect(126.0f, 58.0f, 56.0f, 50.0f);

    // Roof shade
    glColor3f(0.20f, 0.03f, 0.05f);
    drawRect(118.0f, 106.0f, 72.0f, 10.0f);

    // Gold trim
    glColor3f(0.96f, 0.79f, 0.22f);
    drawRect(10.0f, 36.0f, 220.0f, 5.0f);
    drawRect(24.0f, 82.0f, 104.0f, 4.0f);

    // Front bumper
    glBegin(GL_TRIANGLES);
    glColor3f(0.24f, 0.24f, 0.26f);
    glVertex2f(228.0f, 18.0f);
    glVertex2f(252.0f, 18.0f);
    glVertex2f(240.0f, 2.0f);
    glEnd();

    // Chimney stack
    glColor3f(0.18f, 0.18f, 0.20f);
    drawRect(138.0f, 88.0f, 18.0f, 34.0f);
    glColor3f(0.28f, 0.28f, 0.30f);
    drawRect(132.0f, 118.0f, 30.0f, 8.0f);

    // Cabin windows
    glColor3f(0.79f, 0.92f, 0.98f);
    drawRect(135.0f, 70.0f, 18.0f, 22.0f);
    drawRect(158.0f, 70.0f, 18.0f, 22.0f);

    // Headlight glow
    glColor4f(1.0f, 0.93f, 0.55f, 0.30f);
    drawTrainCircle(232.0f, 50.0f, 14.0f, 28);
    glColor3f(1.0f, 0.90f, 0.42f);
    drawTrainCircle(232.0f, 50.0f, 7.0f, 24);

    // Connecting rod
    glColor3f(0.82f, 0.82f, 0.86f);
    glBegin(GL_QUADS);
    glVertex2f(52.0f, 23.0f);
    glVertex2f(160.0f, 23.0f);
    glVertex2f(160.0f, 29.0f);
    glVertex2f(52.0f, 29.0f);
    glEnd();

    // Front wheel
    drawWheel(62.0f, 18.0f, 22.0f, wheelRotation);
    // Middle wheel
    drawWheel(122.0f, 18.0f, 22.0f, wheelRotation);
    // Rear wheel
    drawWheel(182.0f, 18.0f, 22.0f, wheelRotation);

    // Small guide wheel
    drawWheel(216.0f, 16.0f, 14.0f, wheelRotation * 1.2f);

    glPopMatrix();
}
