#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

static float g_aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
float GROUND_HEIGHT = 150.0f;

struct SceneColor {
    float r, g, b;
};

// Smooth color blend
SceneColor blendSceneColor(SceneColor day, SceneColor night) {
    return {
        lerp(day.r, night.r, dayTime),
        lerp(day.g, night.g, dayTime),
        lerp(day.b, night.b, dayTime)
    };
}

// Pick blended color
SceneColor pickSceneColor(SceneColor day, SceneColor night) {
    return blendSceneColor(day, night);
}

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

// Draw train wheel
void drawWheel(float cx, float cy, float r, float rotation){
    SceneColor outer = pickSceneColor({0.2f, 0.0f, 0.0f}, {0.10f, 0.03f, 0.04f});
    SceneColor inner = pickSceneColor({0.85f, 0.1f, 0.1f}, {0.46f, 0.08f, 0.10f});
    SceneColor hub = pickSceneColor({0.55f, 0.0f, 0.0f}, {0.22f, 0.02f, 0.05f});
    SceneColor spoke = pickSceneColor({0.96f, 0.79f, 0.22f}, {0.63f, 0.52f, 0.18f});

    glColor3f(outer.r, outer.g, outer.b);
    drawTrainCircle(cx, cy, r, 48);

    glColor3f(inner.r, inner.g, inner.b);
    drawTrainCircle(cx, cy, r * 0.55f, 36);

    glColor3f(hub.r, hub.g, hub.b);
    drawTrainCircle(cx, cy, r * 0.14f, 24);

    glPushMatrix();
    // Rotate spokes
    glTranslatef(cx, cy, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glColor3f(spoke.r, spoke.g, spoke.b);
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

// Draw smoke puff
void drawSmokePuff(float x, float y, float size, float alpha){
    SceneColor puffBase = pickSceneColor({0.85f, 0.85f, 0.88f}, {0.46f, 0.48f, 0.56f});
    SceneColor puffHighlight = pickSceneColor({0.92f, 0.92f, 0.95f}, {0.62f, 0.65f, 0.72f});

    glColor4f(puffBase.r, puffBase.g, puffBase.b, alpha);
    drawTrainCircle(x, y, size, 28);
    glColor4f(puffHighlight.r, puffHighlight.g, puffHighlight.b, alpha * 0.8f);
    drawTrainCircle(x + size * 0.45f, y + size * 0.15f, size * 0.72f, 24);
}

// Draw ground strip
void ground(){
    SceneColor bottom = blendSceneColor({0.0f, 1.0f, 0.0f}, {0.0f, 0.7f, 0.2f});
    SceneColor top = blendSceneColor({0.0f, 0.6f, 0.0f}, {0.0f, 0.4f, 0.2f});

    glBegin(GL_QUADS);
    glColor3f(bottom.r, bottom.g, bottom.b);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(WINDOW_WIDTH, 0.0f);

    glColor3f(top.r, top.g, top.b);
    glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);
    glVertex2f(0.0f, GROUND_HEIGHT);
    glEnd();
}

// Draw axis-aligned rectangle
void drawRect(float x, float y, float w, float h){
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// Draw railway tracks
void rail(){

    float railTopY = 150.0;
    float railBottomY = 130.0;
    float left = -50.0;
    float right = WINDOW_WIDTH + 50.0;

    // Track sleepers
    SceneColor sleeper = pickSceneColor({0.4f, 0.25f, 0.15f}, {0.20f, 0.14f, 0.11f});
    SceneColor railMetal = pickSceneColor({0.15f, 0.15f, 0.15f}, {0.32f, 0.34f, 0.38f});

    glColor3f(sleeper.r, sleeper.g, sleeper.b);
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

    
    glColor3f(railMetal.r, railMetal.g, railMetal.b);
    drawRect(left, railBottomY, right - left, railHeight);
    drawRect(left, railTopY, right - left, railHeight);
}

// Draw simple quad
void drawQuad(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// Draw train carriage
void drawCarriage(float x, float y, float scale = 1.0) {
    SceneColor carriageBody = pickSceneColor({0.75f, 0.12f, 0.12f}, {0.35f, 0.06f, 0.09f});
    SceneColor stripe = pickSceneColor({0.95f, 0.85f, 0.2f}, {0.63f, 0.53f, 0.16f});
    SceneColor roof = pickSceneColor({0.0f, 0.0f, 0.0f}, {0.08f, 0.08f, 0.10f});
    SceneColor windowGlass = pickSceneColor({0.79f, 0.92f, 0.98f}, {0.97f, 0.78f, 0.32f});
    SceneColor frame = pickSceneColor({0.02f, 0.02f, 0.02f}, {0.12f, 0.12f, 0.16f});

    // Carriage sizing
    float bodyW = 8.0 * scale;
    float bodyH = 1.6 * scale;
    float wheelR = 0.45 * scale;

    float carriageLowerOffset = 0.22f * scale;
    float bodyY = y + wheelR * 2.0f - carriageLowerOffset;

    // Main body
    glColor3f(carriageBody.r, carriageBody.g, carriageBody.b);
    drawQuad(x, bodyY, bodyW, bodyH);

    // Side stripe
    glColor3f(stripe.r, stripe.g, stripe.b);
    drawQuad(x, bodyY + 0.14f * scale, bodyW, 0.12f * scale);

    // Carriage roof
    float roofH = 0.32f * scale;
    float roofInset = 0.10f * scale;
    float roofX = x - roofInset;
    float roofW = bodyW + roofInset * 2.0f;
    float roofY = bodyY + bodyH;
    glColor3f(roof.r, roof.g, roof.b);
    drawQuad(roofX, roofY, roofW, roofH);

    // Window row
    int windowCount = 6;
    float padding = 0.25f * scale;
    float availableW = bodyW - padding * 2.0f;
    float ww = (availableW - (windowCount - 1) * (0.15f * scale)) / windowCount;
    float wh = 0.6f * scale;
    float wy = bodyY + 0.45f * scale;
    glColor3f(windowGlass.r, windowGlass.g, windowGlass.b);
    for (int i = 0; i < windowCount; ++i) {
        float wx = x + padding + i * (ww + 0.15f * scale);
        drawQuad(wx, wy, ww, wh);

        // Window frame
        glColor3f(frame.r, frame.g, frame.b);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(wx, wy);
        glVertex2f(wx + ww, wy);
        glVertex2f(wx + ww, wy + wh);
        glVertex2f(wx, wy + wh);
        glEnd();

        glColor3f(windowGlass.r, windowGlass.g, windowGlass.b);
    }

    // Lower skirt
    float brimHeight = 0.22f * scale;
    float brimInset = 0.05 * scale;
    float brimX = x - brimInset;
    float brimW = bodyW + brimInset * 2.0;
    float brimTop = bodyY;
    float brimBottom = brimTop - brimHeight;
    glColor3f(roof.r, roof.g, roof.b);
    drawQuad(brimX, brimBottom, brimW, brimHeight);

    // Wheel setup
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float wheelRotation = -t * 280.0f;

    float leftWheelX = x + 0.9 * scale;
    float middleWheelX = x + bodyW * 0.5f;
    float rightWheelX = x + bodyW - 0.9 * scale;
    float wheelY = y + wheelR * 0.98;

    drawWheel(leftWheelX, wheelY, wheelR, wheelRotation);
    drawWheel(middleWheelX, wheelY, wheelR, wheelRotation);
    drawWheel(rightWheelX, wheelY, wheelR, wheelRotation);
}

// Handle resize
void reshape(int w, int h) {
    if (h == 0) h = 1;
    g_aspect = float(w) / float(h);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 10.0 * g_aspect, 0.0, 6.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keep drawing
void idle() {
    glutPostRedisplay();
}

// Draw locomotive
void train() {
    SceneColor connector = pickSceneColor({0.15f, 0.15f, 0.15f}, {0.24f, 0.26f, 0.30f});
    SceneColor cabin = pickSceneColor({0.78f, 0.06f, 0.10f}, {0.34f, 0.05f, 0.08f});
    SceneColor cabinRoof = pickSceneColor({0.20f, 0.03f, 0.05f}, {0.10f, 0.05f, 0.08f});
    SceneColor cabinWindow = pickSceneColor({0.79f, 0.92f, 0.98f}, {0.97f, 0.78f, 0.32f});
    SceneColor nose = pickSceneColor({0.88f, 0.10f, 0.14f}, {0.42f, 0.07f, 0.10f});
    SceneColor chimney = pickSceneColor({0.18f, 0.18f, 0.20f}, {0.28f, 0.30f, 0.35f});
    SceneColor gold = pickSceneColor({0.96f, 0.79f, 0.22f}, {0.63f, 0.53f, 0.16f});
    SceneColor bumper = pickSceneColor({0.1f, 0.10f, 0.1f}, {0.20f, 0.20f, 0.24f});
    SceneColor noseFront = pickSceneColor({0.70f, 0.04f, 0.08f}, {0.28f, 0.05f, 0.08f});
    SceneColor rod = pickSceneColor({0.85f, 0.85f, 0.85f}, {0.50f, 0.52f, 0.58f});

    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float wheelRotation = -t * 280.0f;
    
    // Train position
    float baseX = (WINDOW_WIDTH * 0.5f) - 130.0f; 
    float baseY = 155.0f; // Rail height

    glPushMatrix();
    glTranslatef(baseX, baseY, 0.0f);

    // Smoke trail
    for (int i = 0; i < 6; ++i) {
        float life = fmodf(t * 0.42f + i * 0.17f, 1.0f);
        float rise = life * 90.0f;
        float drift = -(life * 40.0f) + sinf(life * 6.283f + i) * 5.0f;
        float size = 8.0f + life * 15.0f;
        float alpha = 0.5f * (1.0f - life);
        drawSmokePuff(185.0f + drift, 110.0f + rise, size, alpha);
    }

    // Main chassis
    glColor3f(connector.r, connector.g, connector.b);
    drawRect(-5.0f, 5.0f, 195.0f, 22.0f);

    // Front chassis
    drawRect(120.0f,-10.0f,70.0f,15.0f);

    // Rear coupler
    drawRect(-15.0f,15.0f,10.0f,22.0f);

    // Cabin shell
    glColor3f(cabin.r, cabin.g, cabin.b);
    drawRect(5.0f, 27.0f, 80.0f, 75.0f);
    
    // Cabin roof
    glColor3f(cabinRoof.r, cabinRoof.g, cabinRoof.b);
    drawRect(0.0f, 102.0f, 90.0f, 8.0f);
    
    // Cabin window
    glColor3f(cabinWindow.r, cabinWindow.g, cabinWindow.b);
    drawRect(25.0f, 65.0f, 40.0f, 25.0f);

    // Engine nose
    glColor3f(nose.r, nose.g, nose.b);
    drawRect(85.0f, 27.0f, 110.0f, 55.0f);

    // Smoke stack
    glColor3f(chimney.r, chimney.g, chimney.b);
    drawRect(175.0f, 82.0f, 18.0f, 30.0f); // Stack stem
    glColor3f(gold.r, gold.g, gold.b);
    drawRect(170.0f, 112.0f, 28.0f, 6.0f);

    // Front bumper
    glColor3f(bumper.r, bumper.g, bumper.b);
    glBegin(GL_TRIANGLES);
        glVertex2f(190.0f, 27.0f);
        glVertex2f(235.0f, -10.0f);
        glVertex2f(190.0f, -10.0f);
    glEnd();

    // Gold trim
    glColor3f(gold.r, gold.g, gold.b);
    drawRect(85.0f, 35.0f, 110.0f, 4.0f);
    drawRect(85.0f, 40.0f, 110.0f, 4.0f);

    // Rounded nose
    glColor3f(noseFront.r, noseFront.g, noseFront.b);
    glBegin(GL_POLYGON);
        for(int i = 0; i <= 360; i += 10) {
            float rad = i * 3.14159 / 180.0;
            glVertex2f(195.0f + cos(rad) * 15.0f, 54.5f + sin(rad) * 27.5f);
        }
    glEnd();

    // Large wheels
    drawWheel(35.0f, 5.0f, 30.0f, wheelRotation);  
    drawWheel(100.0f, 5.0f, 30.0f, wheelRotation); 

    // Small wheels
    drawWheel(145.0f,-5.0f,15.0f,wheelRotation);
    drawWheel(180.0f,-5.0f,15.0f,wheelRotation); 

    // Main rod
    float rodOffset = sin(t * 5.0f) * 5.0f; // Rod motion
    glColor3f(rod.r, rod.g, rod.b);
    drawRect(30.0f + rodOffset, 3.0f, 80.0f, 5.0f);

    // Front rod
    float rodOff = sin(t*-5.0f)*-5.0f;
    drawRect(145.0f+rodOff,-7.0f,40.0f,5.0f);

    drawCarriage(-420, -25, 50);

    // Carriage connector
    glColor3f(connector.r, connector.g, connector.b);
    drawRect(-20.0f, 25.0f, 20.0f, 10.0f);

    glPopMatrix();
}