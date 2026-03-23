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
    glColor3f(0.2f, 0.0f, 0.0f);
    drawTrainCircle(cx, cy, r, 48);

    glColor3f(0.85f, 0.1f, 0.1f);
    drawTrainCircle(cx, cy, r * 0.55f, 36);

    glColor3f(0.55f, 0.0f, 0.0f);
    drawTrainCircle(cx, cy, r * 0.14f, 24);

    glPushMatrix();
    // Spin wheel spokes
    glTranslatef(cx, cy, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glColor3f(0.96f, 0.79f, 0.22f);
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


void train() {
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float wheelRotation = -t * 280.0f;
    
    // Position the train on the tracks
    float baseX = (WINDOW_WIDTH * 0.5f) - 130.0f; 
    float baseY = 155.0f; // Adjusted to sit better on the rails

    glPushMatrix();
    glTranslatef(baseX, baseY, 0.0f);

    //smoke w
    for (int i = 0; i < 6; ++i) {
        float life = fmodf(t * 0.42f + i * 0.17f, 1.0f);
        float rise = life * 90.0f;
        float drift = -(life * 40.0f) + sinf(life * 6.283f + i) * 5.0f;
        float size = 8.0f + life * 15.0f;
        float alpha = 0.5f * (1.0f - life);
        drawSmokePuff(185.0f + drift, 110.0f + rise, size, alpha);
    }

    //the connector with the big wheels
    glColor3f(0.15f, 0.15f, 0.15f);
    drawRect(-5.0f, 5.0f, 195.0f, 22.0f);

    //the Connector with the small wheels
    drawRect(120.0f,-10.0f,70.0f,15.0f);

    //the coupler(el changal)
    drawRect(-15.0f,15.0f,10.0f,22.0f);

    // Rear cabin 
    glColor3f(0.78f, 0.06f, 0.10f); // Red
    drawRect(5.0f, 27.0f, 80.0f, 75.0f);
    
    // Cabin Roof
    glColor3f(0.20f, 0.03f, 0.05f);
    drawRect(0.0f, 102.0f, 90.0f, 8.0f);
    
    // Cabin Window
    glColor3f(0.79f, 0.92f, 0.98f);
    drawRect(25.0f, 65.0f, 40.0f, 25.0f);

    //the nose of the train 
    glColor3f(0.88f, 0.10f, 0.14f);
    drawRect(85.0f, 27.0f, 110.0f, 55.0f);

   // The front rounded 
    glColor3f(0.70f, 0.04f, 0.08f);
    glBegin(GL_POLYGON);
        for(int i = 0; i <= 360; i += 10) {
            float rad = i * 3.14159 / 180.0;
            glVertex2f(195.0f + cos(rad) * 15.0f, 54.5f + sin(rad) * 27.5f);
        }
    glEnd();

    //Chimney
    glColor3f(0.18f, 0.18f, 0.20f);
    drawRect(175.0f, 82.0f, 18.0f, 30.0f); // Stem
    glColor3f(0.96f, 0.79f, 0.22f); // Gold top
    drawRect(170.0f, 112.0f, 28.0f, 6.0f);

    // the little red bumper
    glColor3f(0.88f, 0.10f, 0.14f);
    glBegin(GL_TRIANGLES);
        glVertex2f(190.0f, 27.0f);
        glVertex2f(235.0f, -10.0f);
        glVertex2f(190.0f, -10.0f);
    glEnd();

    // the yellow line
    glColor3f(0.96f, 0.79f, 0.22f);
    drawRect(85.0f, 35.0f, 95.0f, 4.0f);

    // the big wheels
    drawWheel(35.0f, 5.0f, 30.0f, wheelRotation);  
    drawWheel(100.0f, 5.0f, 30.0f, wheelRotation); 

    //the small wheels 
    drawWheel(145.0f,-5.0f,15.0f,wheelRotation);
    drawWheel(180.0f,-5.0f,15.0f,wheelRotation); 

    // the big wheels rod
    float rodOffset = sin(t * 5.0f) * 5.0f; // Simple animation for the rod
    glColor3f(0.85f, 0.85f, 0.85f);
    drawRect(35.0f + rodOffset, 12.0f, 80.0f, 5.0f);

    //the small wheels rod
    float rodOff = sin(t*-5.0f)*-5.0f;
    drawRect(145.0f+rodOff,-5.0f,40.0f,5.0f);

    glPopMatrix();
}