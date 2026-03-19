#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

float cloudOffset1 = 0.0f;
float cloudOffset2 = 250.0f;
float cloudOffset3 = 520.0f;
float sunRotation = 0.0f;

float sunX = 150.0f;
float sunY = 500.0f;
float sunR = 29.0f;

string time = "day";

int starX[30], starY[30];

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
            time = "night";
        }else{
            time = "day";
        }
        glutPostRedisplay();
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

void cloud(float x, float y, float scale){
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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

void sun(float r, float g, float b){
    const float sunX = 150.0f;
    const float sunY = 500.0f;
    const float sunRadius = 29.0f;

    glColor4f(r, g, b, 1.0f);
    drawCircle(sunX, sunY, sunRadius);

    glColor4f(r, g, b, 0.8f);
    for (int i = 0; i < 12; ++i){
        glPushMatrix();
        glTranslatef(sunX, sunY, 0.0f);
        glRotatef(sunRotation + i * 30.0f, 0.0f, 0.0f, 1.0f);
        drawRay(-7, 34);
        glPopMatrix();
    }
}

void stars(){
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for(int i=0; i<30; i++){
        glVertex2i(starX[i], starY[i]);
    }
    glEnd();
}

void moon(){
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(sunX, sunY, sunR);
}

void sky(){
    glBegin(GL_QUADS);
    if(time == "day"){
        glColor3f(0.55f, 0.82f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(WINDOW_WIDTH, 0.0f);

        glColor3f(0.08f, 0.4f, 0.88f);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0.0f, WINDOW_HEIGHT);
        glEnd();

        sun(1.0, 1.0, 0.0);

        cloud(cloudOffset1, 500.0f, 0.5f);
        cloud(cloudOffset2, 530.0f, 0.45f);
        cloud(cloudOffset3, 475.0f, 0.3f);

    }else{
        glColor3f(0.0f, 0.0f, 0.7f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(WINDOW_WIDTH, 0.0f);

        glColor3f(0.1f, 0.1f, 0.1f);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0.0f, WINDOW_HEIGHT);
        glEnd();

        stars();
        moon(); 
    }
    
}

void moveCloud(float &cloudX, float speed, float scale){
    cloudX -= speed;
    if (cloudX < -(140.0f * scale)){
        cloudX = WINDOW_WIDTH + 50.0f;
    }
}

void update(int){
    moveCloud(cloudOffset1, 1.1f, 1.2f);
    moveCloud(cloudOffset2, 0.8f, 0.95f);
    moveCloud(cloudOffset3, 1.4f, 1.05f);
    sunRotation += 0.4f;
    if (sunRotation >= 360.0f){
        sunRotation -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}