#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

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

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

void initStars(){
    for(int i=0; i<30; i++){
        starX[i] = (rand() % WINDOW_WIDTH) + 10;
        starY[i] = (rand() % WINDOW_HEIGHT) + 10;
    }
}

bool onSun(int mouseX, int mouseY){
    float x = mouseX;
    float y = WINDOW_HEIGHT - mouseY;

    float dx = x - sunX;
    float dy = y - sunY;

    return (dx*dx + dy*dy <= sunR*sunR);
}

// im used to skript events so i named it like this :>
void onMouseMove(int x, int y){
    if(onSun(x, y)){
        glutSetCursor(GLUT_CURSOR_INFO);
    }else{
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

void onMouseClick(int btn, int state, int x, int y){
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && onSun(x, y)){
        if(time == "day"){
            initStars();
            transitionDir = 1;      // day -> night
        }else{
            cloudOffset1 = CLOUD_START1;
            cloudOffset2 = CLOUD_START2;
            cloudOffset3 = CLOUD_START3;
            transitionDir = -1;     // night -> day
        }
        transitioning = true;
        //glutPostRedisplay();
    }
}

void drawCircle(float centerX, float centerY, float radius, int segments = 40){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= segments; ++i){
        float angle = 2.0f * 3.1415926f * i / segments;
        glVertex2f(centerX + cos(angle) * radius, centerY + sin(angle) * radius);
    }
    glEnd();
}

void cloud(float x, float y, float scale, float alpha){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawCircle(x, y, 28.0f * scale);
    drawCircle(x + 28.0f * scale, y + 12.0f * scale, 34.0f * scale);
    drawCircle(x + 60.0f * scale, y + 4.0f * scale, 30.0f * scale);
    drawCircle(x + 90.0f * scale, y + 10.0f * scale, 24.0f * scale);
}

void drawRay(int x, int y){
    glBegin(GL_TRIANGLES);
    glVertex2i(x, y);
    glVertex2i(x + 14, y);
    glVertex2i(x + 7, y + 34);
    glEnd();
}

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

void stars(float alpha, float yOffset){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for(int i=0; i<30; i++){
        glVertex2f(starX[i], starY[i] + yOffset);
    }
    glEnd();
}

void moon(float alpha){
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawCircle(sunX, sunY, sunR);
}

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

void moveCloud(float &cloudX, float speed, float scale){
    cloudX -= speed;
    if (cloudX < -(140.0f * scale)){
        cloudX = WINDOW_WIDTH + 50.0f;
    }
}

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