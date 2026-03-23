#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const float GROUND_HEIGHT = 150.0f;

static float g_aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);

struct SceneColor {
    float r, g, b;
};
struct vertex {
    GLfloat x, y;
};
struct color {
    GLfloat r, g, b;
};

GLfloat scrollX = 0.0f;
GLfloat moveSpeed = 3.0f;
GLfloat resetPoint = 7600.0f;
GLfloat mountainScrollX = 0.0f;
GLfloat mountainSpeed = 1.2f;
GLint numberOfBuildings = 12;
GLint numberOfSkyscrapers = 4;
GLint numberofMountains = 2;
GLint numberOfTrees = 25;

const float CLOUD_START1 = 0.0f;
const float CLOUD_START2 = 250.0f;
const float CLOUD_START3 = 520.0f;

float cloudOffset1 = CLOUD_START1;
float cloudOffset2 = CLOUD_START2;
float cloudOffset3 = CLOUD_START3;
float sunRotation = 0.0f;

float sunX = 150.0f;
float sunY = 500.0f;
float sunR = 29.0f;

string time = "day";
float dayTime = 0.0f;
bool transitioning = false;
int transitionDir = 1;

int starX[30], starY[30];

// Linear blend
float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

// Randomize star positions
void initStars(){
    for(int i=0; i<30; i++){
        starX[i] = (rand() % WINDOW_WIDTH) + 10;
        starY[i] = (rand() % WINDOW_HEIGHT) + 10;
    }
}

// Check sun hover
bool onSun(int mouseX, int mouseY){
    float x = mouseX;
    float y = WINDOW_HEIGHT - mouseY;

    float dx = x - sunX;
    float dy = y - sunY;

    return (dx*dx + dy*dy <= sunR*sunR);
}

// Update cursor icon
void onMouseMove(int x, int y){
    if(onSun(x, y)){
        glutSetCursor(GLUT_CURSOR_INFO);
    }else{
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

// Handle sun clicks
void onMouseClick(int btn, int state, int x, int y){
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && onSun(x, y)){
        if(time == "day"){
            initStars();
            transitionDir = 1;      // To night
        }else{
            cloudOffset1 = CLOUD_START1;
            cloudOffset2 = CLOUD_START2;
            cloudOffset3 = CLOUD_START3;
            transitionDir = -1;     // To day
        }
        transitioning = true;
    }
}

// Draw filled circle
void drawCircle(float centerX, float centerY, float radius, int segments = 40){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= segments; ++i){
        float angle = 2.0f * 3.1415926f * i / segments;
        glVertex2f(centerX + cos(angle) * radius, centerY + sin(angle) * radius);
    }
    glEnd();
}

// Draw one cloud
void cloud(float x, float y, float scale, float alpha){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawCircle(x, y, 28.0f * scale);
    drawCircle(x + 28.0f * scale, y + 12.0f * scale, 34.0f * scale);
    drawCircle(x + 60.0f * scale, y + 4.0f * scale, 30.0f * scale);
    drawCircle(x + 90.0f * scale, y + 10.0f * scale, 24.0f * scale);
}

// Draw one ray
void drawRay(int x, int y){
    glBegin(GL_TRIANGLES);
    glVertex2i(x, y);
    glVertex2i(x + 14, y);
    glVertex2i(x + 7, y + 34);
    glEnd();
}

// Draw glowing sun
void sun(float r, float g, float b, float alpha){
    const float sunX = 150.0f;
    const float sunY = 500.0f;
    const float sunRadius = 29.0f;

    glColor4f(r, g, b, alpha);
    drawCircle(sunX, sunY, sunRadius);

    glColor4f(r, g, b, 0.8f * alpha);
    for (int i = 0; i < 12; ++i){
        glPushMatrix();
        glTranslatef(sunX, sunY, 0.0f);
        glRotatef(sunRotation + i * 30.0f, 0.0f, 0.0f, 1.0f);
        drawRay(-7, 34);
        glPopMatrix();
    }
}

// Draw star field
void stars(float alpha, float yOffset){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for(int i=0; i<30; i++){
        glVertex2f(starX[i], starY[i] + yOffset);
    }
    glEnd();
}

// Draw moon disc
void moon(float alpha){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawCircle(sunX, sunY, sunR);
}

// Render sky layer
void sky(){
    float bottomR = lerp(0.55f, 0.0f, dayTime);
    float bottomG = lerp(0.82f, 0.0f, dayTime);
    float bottomB = lerp(1.0f, 0.7f, dayTime);

    float topR = lerp(0.08f, 0.1f, dayTime);
    float topG = lerp(0.4f, 0.1f, dayTime);
    float topB = lerp(0.88f, 0.1f, dayTime);

    float cloudTransitionOffset = -950.0f * dayTime;
    if(transitioning && transitionDir == -1){
        cloudTransitionOffset = 950.0f * dayTime;
    }
    float cloudAlpha = 1.0f - dayTime;
    float starAlpha = dayTime;
    float starYOffset = lerp(150.0f, 0.0f, dayTime);
    float sunAlpha = 1.0f - dayTime;
    float moonAlpha = dayTime;

    glBegin(GL_QUADS);
    glColor3f(bottomR, bottomG, bottomB);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(WINDOW_WIDTH, 0.0f);

    glColor3f(topR, topG, topB);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0.0f, WINDOW_HEIGHT);
    glEnd();

    stars(starAlpha, starYOffset);
    sun(1.0f, 1.0f, 0.0f, sunAlpha);
    moon(moonAlpha);
    
    cloud(cloudOffset1 + cloudTransitionOffset, 500.0f, 0.5f, cloudAlpha);
    cloud(cloudOffset2 + cloudTransitionOffset, 530.0f, 0.45f, cloudAlpha);
    cloud(cloudOffset3 + cloudTransitionOffset, 475.0f, 0.3f, cloudAlpha);
}

// Scroll one cloud
void moveCloud(float &cloudX, float speed, float scale){
    cloudX -= speed;
    if (cloudX < -(140.0f * scale)){
        cloudX = WINDOW_WIDTH + 50.0f;
    }
}

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

// Smooth color blend
color blendLandscapeColor(color day, color night) {
    return {
        lerp(day.r, night.r, dayTime),
        lerp(day.g, night.g, dayTime),
        lerp(day.b, night.b, dayTime)
    };
}

// Pick blended color
color pickLandscapeColor(color day, color night) {
    return blendLandscapeColor(day, night);
}

// Draw filled circle
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

// Draw front hill
void drawHill(){
    const GLfloat baseY = 150.0f;
    const GLfloat leftX = -30.0f;
    const GLfloat rightX = 360.0f;
    color baseShadow = pickLandscapeColor({0.16f, 0.43f, 0.18f}, {0.06f, 0.18f, 0.10f});
    color upperSlope = pickLandscapeColor({0.28f, 0.67f, 0.31f}, {0.10f, 0.28f, 0.15f});
    color frontHighlight = pickLandscapeColor({0.42f, 0.78f, 0.37f}, {0.16f, 0.34f, 0.20f});
    color frontGlow = pickLandscapeColor({0.66f, 0.89f, 0.52f}, {0.24f, 0.42f, 0.26f});

    glBegin(GL_POLYGON);
    // Hill base
    glColor3f(baseShadow.r, baseShadow.g, baseShadow.b);
    glVertex2f(leftX, baseY);
    glVertex2f(rightX, baseY);

    // Upper slope
    glColor3f(upperSlope.r, upperSlope.g, upperSlope.b);
    glVertex2f(320.0f, 175.0f);
    glVertex2f(280.0f, 230.0f);
    glVertex2f(235.0f, 290.0f);
    glVertex2f(185.0f, 335.0f);
    glVertex2f(135.0f, 315.0f);
    glVertex2f(90.0f, 260.0f);
    glVertex2f(45.0f, 205.0f);
    glVertex2f(5.0f, 170.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // Front glow
    glColor4f(frontHighlight.r, frontHighlight.g, frontHighlight.b, 0.55f);
    glVertex2f(50.0f, baseY);
    glVertex2f(255.0f, baseY);
    glColor4f(frontGlow.r, frontGlow.g, frontGlow.b, 0.12f);
    glVertex2f(195.0f, 280.0f);
    glVertex2f(120.0f, 250.0f);
    glEnd();
}

// Draw building wall
void buildingWall(vertex v1, vertex v2) {
    color wall = blendLandscapeColor({0.95f, 0.85f, 0.7f}, {0.1f, 0.1f, 0.2f});
    glBegin(GL_POLYGON);
    glColor3f(wall.r, wall.g, wall.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw building roof
void buildingRoof(vertex v1, vertex v2) {
    color roof = blendLandscapeColor({0.7f, 0.25f, 0.15f}, {0.05f, 0.05f, 0.1f});
    glBegin(GL_POLYGON);
    glColor3f(roof.r, roof.g, roof.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x + 125.0f, v2.y);
    glVertex2f(v1.x + 145.0f, v1.y);
    glEnd();
}

// Draw building window
void buildingWindow(vertex v1, vertex v2) {
    color window = blendLandscapeColor({0.7f, 0.9f, 1.0f}, {1.0f, 0.85f, 0.3f});
    glBegin(GL_POLYGON);
    glColor3f(window.r, window.g, window.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw building door
void buildingDoor(vertex v1, vertex v2) {
    color door = blendLandscapeColor({0.60f, 0.40f, 0.20f}, {0.10f, 0.08f, 0.12f});
    glBegin(GL_POLYGON);
    glColor3f(door.r, door.g, door.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw building rows
void building() {
    GLfloat current_x = 25.0f;
    GLfloat ground_y = 150.0f;
    GLfloat gap = 230.0f;

    // First row
    for (GLint i = 0; i < numberOfBuildings; i++) {
        GLfloat width = 125.0f; 
        GLfloat height = 170.0f;

        // Wall panel
        vertex vWall1 = { current_x, ground_y }; 
        vertex vWall2 = { current_x + width, ground_y + height };
        buildingWall(vWall1, vWall2);

        // Roof panel
        vertex vRoof1 = { current_x - 10.0f, ground_y + height }; 
        vertex vRoof2 = { current_x, ground_y + height + 20.0f };
        buildingRoof(vRoof1, vRoof2);

        // Door panel
        vertex vDoor1 = { current_x + 62.5f - 10.0f, ground_y }; 
        vertex vDoor2 = { current_x + 62.5f + 10.0f, ground_y + 30.0f };
        buildingDoor(vDoor1, vDoor2);

        // Window pairs
        GLfloat winWidth = 20.0f; 
        GLfloat winHeight = 25.0f; 
        GLfloat floorSpacing = 40.0f;

        for (GLfloat h = 30.0f; h < height - 30.0f; h += floorSpacing) {
            vertex vWinL1 = { current_x + 20.0f, ground_y + h }; 
            vertex vWinL2 = { current_x + 20.0f + winWidth, ground_y + h + winHeight };
            buildingWindow(vWinL1, vWinL2);

            vertex vWinR1 = { current_x + width - 40.0f, ground_y + h }; 
            vertex vWinR2 = { current_x + width - 40.0f + winWidth, ground_y + h + winHeight };
            buildingWindow(vWinR1, vWinR2);
        }
        current_x += width + gap;
    }

    current_x = 175.0f; 
    gap += 40;

    for (GLint i = 0; i < numberOfBuildings; i++) {
        GLfloat width = 125.0f; 
        GLfloat height = 230.0f;

        vertex vWall1 = { current_x, ground_y }; 
        vertex vWall2 = { current_x + width, ground_y + height };
        buildingWall(vWall1, vWall2);

        vertex vRoof1 = { current_x - 10.0f, ground_y + height }; 
        vertex vRoof2 = { current_x, ground_y + height + 20.0f };
        buildingRoof(vRoof1, vRoof2);

        vertex vDoor1 = { current_x + 62.5f - 10.0f, ground_y }; 
        vertex vDoor2 = { current_x + 62.5f + 10.0f, ground_y + 30.0f };
        buildingDoor(vDoor1, vDoor2);

        GLfloat winWidth = 20.0f; 
        GLfloat winHeight = 25.0f; 
        GLfloat floorSpacing = 40.0f;

        for (GLfloat h = 30.0f; h < height - 30.0f; h += floorSpacing) {
            vertex vWinL1 = { current_x + 20.0f, ground_y + h }; 
            vertex vWinL2 = { current_x + 20.0f + winWidth, ground_y + h + winHeight };
            buildingWindow(vWinL1, vWinL2);

            vertex vWinR1 = { current_x + width - 40.0f, ground_y + h }; 
            vertex vWinR2 = { current_x + width - 40.0f + winWidth, ground_y + h + winHeight };
            buildingWindow(vWinR1, vWinR2);
        }
        current_x += width + gap;
    }
}

// Draw tower wall
void skyscraperWall(vertex v1, vertex v2) {
    color wall = blendLandscapeColor({0.75f, 0.75f, 0.75f}, {0.10f, 0.10f, 0.15f});
    glBegin(GL_POLYGON);
    glColor3f(wall.r, wall.g, wall.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw tower roof
void skyscraperRoof(vertex v1, vertex v2) {
    color first = blendLandscapeColor({0.50f, 0.50f, 0.50f}, {0.05f, 0.05f, 0.10f});
    color second = blendLandscapeColor({0.60f, 0.60f, 0.60f}, {0.08f, 0.08f, 0.15f});
    color antenna = blendLandscapeColor({0.75f, 0.75f, 0.75f}, {0.02f, 0.02f, 0.05f});

    // First tier
    glBegin(GL_POLYGON);
    glColor3f(first.r, first.g, first.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x + 10, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x + 10, v1.y);
    glEnd();

    // Second tier
    glBegin(GL_POLYGON);
    glColor3f(second.r, second.g, second.b);
    glVertex2f(v1.x + 15, v2.y);
    glVertex2f(v1.x + 20, v2.y + 10);
    glVertex2f(v2.x - 10, v2.y + 10);
    glVertex2f(v2.x - 5, v2.y);
    glEnd();

    // Roof antenna
    glBegin(GL_POLYGON);
    glColor3f(antenna.r, antenna.g, antenna.b);
    glVertex2f(((v2.x - v1.x + 10) / 2) - 5 + v1.x, v2.y + 10);
    glVertex2f(v1.x + (v2.x - v1.x + 10) / 2, v2.y + 80.0f);
    glVertex2f(((v2.x - v1.x + 10) / 2) + 5 + v1.x, v2.y + 10);
    glEnd();
}

// Draw tower window
void skyscraperWindow(vertex v1, vertex v2) {
    color window = blendLandscapeColor({0.65f, 0.85f, 0.95f}, {1.00f, 0.90f, 0.60f});
    glBegin(GL_POLYGON);
    glColor3f(window.r, window.g, window.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw skyscrapers
void skyscraper() {
    GLfloat current_x = 290.0f;
    GLfloat ground_y = 150.0f;
    GLfloat gap = 430.0f;
    

    for (GLint i = 0; i < numberOfSkyscrapers; i++) {
        GLfloat width = 97.9f; 
        GLfloat height = 360.0f;

        // Wall panel
        vertex vWall1 = { current_x, ground_y }; 
        vertex vWall2 = { current_x + width, ground_y + height };
        skyscraperWall(vWall1, vWall2);

        // Roof section
        vertex vRoof1 = { current_x, ground_y + height }; 
        vertex vRoof2 = { current_x + width - 10.0f, ground_y + height + 10.0f };
        skyscraperRoof(vRoof1, vRoof2);

        // Triple windows
        GLfloat winWidth = 20.0f; 
        GLfloat winHeight = 25.0f; 
        GLfloat floorSpacing = 40.0f;

        for (GLfloat h = 30.0f; h < height - 10.0f; h += floorSpacing) {
            // Left window
            vertex vWinL1 = { current_x + 10.0f, ground_y + h }; 
            vertex vWinL2 = { current_x + 10.0f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinL1, vWinL2);

            // Mid window
            vertex vWinM1 = { current_x + width - 57.9f, ground_y + h }; 
            vertex vWinM2 = { current_x + width - 57.9f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinM1, vWinM2);

            // Right window
            vertex vWinR1 = { current_x + width - 30.0f, ground_y + h }; 
            vertex vWinR2 = { current_x + width - 30.0f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinR1, vWinR2);
        }
        current_x += width + gap;
    }
}

// Draw mountain row
void moutain() {
    color body = blendLandscapeColor({0.25f, 0.28f, 0.35f}, {0.05f, 0.05f, 0.12f});
    color snow = blendLandscapeColor({0.95f, 0.98f, 1.00f}, {0.30f, 0.40f, 0.60f});

    GLfloat current_x = 0.0f;
    GLfloat ground_y = 120.0f;
    GLfloat gap = 800.0f;
    

    for (GLint i = 0; i < numberofMountains; i++) {
        GLfloat width = 440.0f; 
        GLfloat height = 400.0f;

        // Mountain body
        glColor3f(body.r, body.g, body.b);
        glBegin(GL_POLYGON);
        glVertex2f(current_x, ground_y);
        glVertex2f(current_x + 65.0f, height - 80.0f);
        glVertex2f(current_x + 85.0f, height - 110.0f);
        glVertex2f(current_x + 150.0f, height);
        glVertex2d(current_x + 210.0f, height);
        glVertex2f(current_x + width, ground_y);
        glEnd();

        // Snow cap
        glColor3f(snow.r, snow.g, snow.b);
        glBegin(GL_POLYGON);
        glVertex2f(current_x + 120.5f, height - 50.0f);
        glVertex2f(current_x + 150.0f, height);
        glVertex2d(current_x + 210.0f, height);
        glVertex2f(current_x + 250.0f, height - 50.0f);
        glEnd();
        
        current_x += gap + width;
    }
}

// Draw tree trunk
void treeTrunk(vertex v1, vertex v2) {
    color trunk = blendLandscapeColor({0.45f, 0.30f, 0.15f}, {0.05f, 0.03f, 0.02f});
    glBegin(GL_POLYGON);
    glColor3f(trunk.r, trunk.g, trunk.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Draw tree canopy
void treeLeaves(vertex v1, vertex v2, vertex v3) {
    color leaves = blendLandscapeColor({0.15f, 0.50f, 0.20f}, {0.02f, 0.10f, 0.05f});
    glBegin(GL_TRIANGLES);
    glColor3f(leaves.r, leaves.g, leaves.b);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v3.x, v3.y);
    glEnd();
}

// Draw tree row
void tree() {
    GLfloat current_x = 100.0f; 
    GLfloat ground_y = 150.0f; 
    GLfloat gap = 300.0f;      
    

    for (GLint i = 0; i < numberOfTrees; i++) {
        GLfloat trunkWidth = 20.0f;
        GLfloat trunkHeight = 40.0f;
        GLfloat leafWidth = 80.0f;
        GLfloat leafHeight = 100.0f;

        
        vertex vTrunk1 = { current_x, ground_y };
        vertex vTrunk2 = { current_x + trunkWidth, ground_y + trunkHeight };
        treeTrunk(vTrunk1, vTrunk2);

        
        vertex vLeaf1 = { (current_x + trunkWidth / 2) - leafWidth / 2, ground_y + trunkHeight };
        vertex vLeaf2 = { (current_x + trunkWidth / 2), ground_y + trunkHeight + leafHeight };
        vertex vLeaf3 = { (current_x + trunkWidth / 2) + leafWidth / 2, ground_y + trunkHeight };
        treeLeaves(vLeaf1, vLeaf2, vLeaf3);

        current_x += gap;
    }
}

// Draw station
void drawStation(){
    color platform = blendLandscapeColor({0.8, 0.78, 0.72}, {0.18, 0.22, 0.32});
    color edge = blendLandscapeColor({0.95, 0.9, 0.2}, {0.5, 0.45, 0.2});
    color building = blendLandscapeColor({0.86, 0.6, 0.45}, {0.25, 0.18, 0.22});
    color roof = blendLandscapeColor({0.55, 0.13, 0.13}, {0.15, 0.05, 0.08});
    color window = blendLandscapeColor({0.95, 0.95, 1.0}, {1.0, 0.85, 0.3});
    color winLines = blendLandscapeColor({0.2, 0.2, 0.25}, {0.05, 0.05, 0.12});
    color door = blendLandscapeColor({0.35, 0.18, 0.15}, {0.12, 0.06, 0.04});
    color billboard = blendLandscapeColor({0.98, 0.98, 0.95}, {0.2, 0.22, 0.3});
    color text = blendLandscapeColor({0.1, 0.1, 0.15}, {0.9, 0.85, 0.5});

    // Platform base
    glColor3f(platform.r,platform.g,platform.b);
    drawRect(5200.0, 150.0, 760.0, 40.0);

    // Edge stripe
    glColor3f(edge.r,edge.g,edge.b);
    drawRect(5200.0, 190.0, 760.0, 6.0);

    // Station shell
    float bx = 5440.0, by = 190.0;
    float bw = 280.0, bh = 110.0;
    glColor3f(building.r,building.g,building.b);
    drawRect(bx, by, bw, bh);

    // Station roof
    glColor3f(roof.r,roof.g,roof.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 10.0, by + bh);
    glVertex2f(bx + bw + 10.0, by + bh);
    glVertex2f(bx + bw / 2.0, by + bh + 60.0);
    glEnd();

    // Sign board
    glColor3f(billboard.r,billboard.g,billboard.b);
    drawRect(bx+40,by+90,bw-80,bh-70);
    glColor3f(text.r,text.g,text.b);
    glRasterPos2i(bx+50, by+100);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'I');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'I');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
    

    // Station windows
    glColor3f(window.r,window.g,window.b);
    float ww = 40.0, wh = 50.0;
    float wy = by + 30.0;
    float doorCenterX = bx + bw / 2.0;
    float windowCenterOffset = 80.0;
    float wxLeft = doorCenterX - windowCenterOffset - ww / 2.0;
    float wxRight = doorCenterX + windowCenterOffset - ww / 2.0;

    // Left window
    drawRect(wxLeft, wy, ww, wh);
    glColor3f(winLines.r,winLines.g,winLines.b);
    glBegin(GL_LINES);
    glVertex2f(wxLeft + ww / 2.0, wy);
    glVertex2f(wxLeft + ww / 2.0, wy + wh);
    glVertex2f(wxLeft, wy + wh / 2.0);
    glVertex2f(wxLeft + ww, wy + wh / 2.0);
    glEnd();

    // Right window
    glColor3f(window.r,window.g,window.b);
    drawRect(wxRight, wy, ww, wh);
    glColor3f(winLines.r,winLines.g,winLines.b);
    glBegin(GL_LINES);
    glVertex2f(wxRight + ww / 2.0, wy);
    glVertex2f(wxRight + ww / 2.0, wy + wh);
    glVertex2f(wxRight, wy + wh / 2.0);
    glVertex2f(wxRight + ww, wy + wh / 2.0);
    glEnd();

    // Station door
    glColor3f(door.r,door.g,door.b);
    drawRect(bx + bw / 2.0 - 22.0, by, 44.0, 70.0);
}
// Draw scrolling world
void landscape() {
    // Scroll foreground
    scrollX -= moveSpeed; 
    // Scroll mountains
    mountainScrollX -= mountainSpeed;

    // Wrap foreground
    if (scrollX < -resetPoint) {
        scrollX = 0.0f;
    }
    // Wrap mountains
    if (mountainScrollX < -resetPoint) {
        mountainScrollX = 0.0f;
    }

    glPushMatrix();
    // Move mountains
    glTranslatef(mountainScrollX, 0.0f, 0.0f);
    // First pass
    moutain();
    // Second pass
    glTranslatef(resetPoint, 0.0f, 0.0f);
    moutain();
    glPopMatrix();

    glPushMatrix(); 
    // Move foreground
    glTranslatef(scrollX, 0.0f, 0.0f); 

    // First pass
    skyscraper();
    building();
    tree();
    drawStation();

    // Second pass
    glTranslatef(resetPoint, 0.0f, 0.0f);
    skyscraper();
    building();
    tree();
    drawStation();

    glPopMatrix(); 
}

// Advance animation state
void update(int){
    if(transitioning){
        dayTime += 0.01f * transitionDir;

        if(dayTime >= 1.0f){
            dayTime = 1.0f;
            time = "night";
            transitioning = false;  
        }
        if(dayTime <= 0.0f){
            dayTime = 0.0f;
            time = "day";
            transitioning = false;
        } 
    }
    if(!(transitioning && transitionDir == -1)){
        moveCloud(cloudOffset1, 1.1f, 1.2f);
        moveCloud(cloudOffset2, 0.8f, 0.95f);
        moveCloud(cloudOffset3, 1.4f, 1.05f);
    }
    sunRotation += 0.4f;
    if (sunRotation >= 360.0f){
        sunRotation -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

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
    drawHill();
    landscape();
    ground();
    rail();
    train();

    glutSwapBuffers();
}

int main(int argc, char **argv){
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