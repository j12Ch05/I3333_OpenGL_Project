// main land, rail & train

GLfloat GROUND_HEIGHT = 150.0f;

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

void rail(){
    glBegin(GL_QUADS);
    if(time=="day"){
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex2f(0.0f, GROUND_HEIGHT);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);

        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT + 10.0f);
        glVertex2f(0.0f, GROUND_HEIGHT + 10.0f);
        glEnd();
        //drawConnectors();
    }else{
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(0.0f, GROUND_HEIGHT);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);

        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT + 10.0f);
        glVertex2f(0.0f, GROUND_HEIGHT + 10.0f);
        glEnd();
    }
}