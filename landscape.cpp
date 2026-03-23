GLfloat scrollX = 0.0f;
GLfloat moveSpeed = 3.0f;
GLfloat resetPoint = 7600.0f;
GLfloat mountainScrollX = 0.0f;
GLfloat mountainSpeed = 1.2f;
GLint numberOfBuildings = 12;
GLint numberOfSkyscrapers = 4;
GLint numberofMountains = 2;
GLint numberOfTrees = 25;
#define M_PI 3.141592653
struct vertex {
    GLfloat x, y;
};

struct color {
    GLfloat r, g, b;
};

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

void drawHill(){
    const GLfloat baseY = 150.0f;
    const GLfloat leftX = -30.0f;
    const GLfloat rightX = 360.0f;

    glBegin(GL_POLYGON);
    // Base shadow
    glColor3f(0.16f, 0.43f, 0.18f);
    glVertex2f(leftX, baseY);
    glVertex2f(rightX, baseY);

    // Soft upper slope
    glColor3f(0.28f, 0.67f, 0.31f);
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
    // Front highlight
    glColor4f(0.42f, 0.78f, 0.37f, 0.55f);
    glVertex2f(50.0f, baseY);
    glVertex2f(255.0f, baseY);
    glColor4f(0.66f, 0.89f, 0.52f, 0.12f);
    glVertex2f(195.0f, 280.0f);
    glVertex2f(120.0f, 250.0f);
    glEnd();
}

//Drawing the components of a building

void buildingWall(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.95f, 0.85f, 0.7f);
    } else {
        glColor3f(0.1f, 0.1f, 0.2f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

void buildingRoof(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.7f, 0.25f, 0.15f);
    } else {
        glColor3f(0.05f, 0.05f, 0.1f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x + 125.0f, v2.y);
    glVertex2f(v1.x + 145.0f, v1.y);
    glEnd();
}

void buildingWindow(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.7f, 0.9f, 1.0f);
    } else {
        glColor3f(1.0f, 0.85f, 0.3f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

void buildingDoor(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.60f, 0.40f, 0.20f);
    } else {
        glColor3f(0.10f, 0.08f, 0.12f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

void building() {
    GLfloat current_x = 25.0f;
    GLfloat ground_y = 150.0f;
    GLfloat gap = 230.0f;

    //drawing the first set of buildings
    for (GLint i = 0; i < numberOfBuildings; i++) {
        GLfloat width = 125.0f; 
        GLfloat height = 170.0f;

        //Drawing the wall
        vertex vWall1 = { current_x, ground_y }; 
        vertex vWall2 = { current_x + width, ground_y + height };
        buildingWall(vWall1, vWall2);

        //Drawing the roof
        vertex vRoof1 = { current_x - 10.0f, ground_y + height }; 
        vertex vRoof2 = { current_x, ground_y + height + 20.0f };
        buildingRoof(vRoof1, vRoof2);

        //Drawing the door
        vertex vDoor1 = { current_x + 62.5f - 10.0f, ground_y }; 
        vertex vDoor2 = { current_x + 62.5f + 10.0f, ground_y + 30.0f };
        buildingDoor(vDoor1, vDoor2);

        //Drawing pairs of window
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

//Drawing the components of a skyscraper
void skyscraperWall(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.75f, 0.75f, 0.75f);
    } else {
        glColor3f(0.10f, 0.10f, 0.15f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

void skyscraperRoof(vertex v1, vertex v2) {
    // First Level
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.50f, 0.50f, 0.50f);
    } else {
        glColor3f(0.05f, 0.05f, 0.10f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x + 10, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x + 10, v1.y);
    glEnd();

    // Second Level
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.60f, 0.60f, 0.60f);
    } else {
        glColor3f(0.08f, 0.08f, 0.15f);
    }
    glVertex2f(v1.x + 15, v2.y);
    glVertex2f(v1.x + 20, v2.y + 10);
    glVertex2f(v2.x - 10, v2.y + 10);
    glVertex2f(v2.x - 5, v2.y);
    glEnd();

    // Antenna
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.75f, 0.75f, 0.75f);
    } else {
        glColor3f(0.02f, 0.02f, 0.05f);
    }
    glVertex2f(((v2.x - v1.x + 10) / 2) - 5 + v1.x, v2.y + 10);
    glVertex2f(v1.x + (v2.x - v1.x + 10) / 2, v2.y + 80.0f);
    glVertex2f(((v2.x - v1.x + 10) / 2) + 5 + v1.x, v2.y + 10);
    glEnd();
}

void skyscraperWindow(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.65f, 0.85f, 0.95f);
    } else {
        glColor3f(1.00f, 0.90f, 0.60f);
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

void skyscraper() {
    GLfloat current_x = 290.0f;
    GLfloat ground_y = 150.0f;
    GLfloat gap = 430.0f;
    

    for (GLint i = 0; i < numberOfSkyscrapers; i++) {
        GLfloat width = 97.9f; 
        GLfloat height = 360.0f;

        //drawing the wall
        vertex vWall1 = { current_x, ground_y }; 
        vertex vWall2 = { current_x + width, ground_y + height };
        skyscraperWall(vWall1, vWall2);

        //drawing the roof with the antenna
        vertex vRoof1 = { current_x, ground_y + height }; 
        vertex vRoof2 = { current_x + width - 10.0f, ground_y + height + 10.0f };
        skyscraperRoof(vRoof1, vRoof2);

        //drawing a group of 3 windowa
        GLfloat winWidth = 20.0f; 
        GLfloat winHeight = 25.0f; 
        GLfloat floorSpacing = 40.0f;

        for (GLfloat h = 30.0f; h < height - 10.0f; h += floorSpacing) {
            //left window
            vertex vWinL1 = { current_x + 10.0f, ground_y + h }; 
            vertex vWinL2 = { current_x + 10.0f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinL1, vWinL2);

            //mid window
            vertex vWinM1 = { current_x + width - 57.9f, ground_y + h }; 
            vertex vWinM2 = { current_x + width - 57.9f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinM1, vWinM2);

            //right window
            vertex vWinR1 = { current_x + width - 30.0f, ground_y + h }; 
            vertex vWinR2 = { current_x + width - 30.0f + winWidth, ground_y + h + winHeight };
            skyscraperWindow(vWinR1, vWinR2);
        }
        current_x += width + gap;
    }
}

void moutain() {
    color body, snow;
    if (time == "day") {
        body = { 0.25f, 0.28f, 0.35f }; 
        snow = { 0.95f, 0.98f, 1.00f };
    } else {
        body = { 0.05f, 0.05f, 0.12f }; 
        snow = { 0.30f, 0.40f, 0.60f };
    }

    GLfloat current_x = 0.0f;
    GLfloat ground_y = 120.0f;
    GLfloat gap = 800.0f;
    

    for (GLint i = 0; i < numberofMountains; i++) {
        GLfloat width = 440.0f; 
        GLfloat height = 400.0f;

        //drawing the body of the mountain
        glColor3f(body.r, body.g, body.b);
        glBegin(GL_POLYGON);
        glVertex2f(current_x, ground_y);
        glVertex2f(current_x + 65.0f, height - 80.0f);
        glVertex2f(current_x + 85.0f, height - 110.0f);
        glVertex2f(current_x + 150.0f, height);
        glVertex2d(current_x + 210.0f, height);
        glVertex2f(current_x + width, ground_y);
        glEnd();

        // Snow fits peak
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

// Drawing the Trunk of the tree
void treeTrunk(vertex v1, vertex v2) {
    glBegin(GL_POLYGON);
    if (time == "day") {
        glColor3f(0.45f, 0.30f, 0.15f); // Brown
    } else {
        glColor3f(0.05f, 0.03f, 0.02f); // Dark Shadow
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v2.x, v1.y);
    glEnd();
}

// Drawing the Leaves (Canopy)
void treeLeaves(vertex v1, vertex v2, vertex v3) {
    glBegin(GL_TRIANGLES);
    if (time == "day") {
        glColor3f(0.15f, 0.50f, 0.20f); // Green
    } else {
        glColor3f(0.02f, 0.10f, 0.05f); // Dark Green/Black
    }
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v3.x, v3.y);
    glEnd();
}

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

void drawStation(){
    color platform,edge,building,roof,window,winLines,door,billboard,text;

    if(time == "day"){
        platform = {0.8, 0.78, 0.72};
        edge = {0.95, 0.9, 0.2};
        building = {0.86, 0.6, 0.45};
        roof = {0.55, 0.13, 0.13};
        window = {0.95, 0.95, 1.0};
        winLines={0.2, 0.2, 0.25};
        door = {0.35, 0.18, 0.15};
    }
    if(time == "day"){
        platform = {0.8, 0.78, 0.72};
        edge = {0.95, 0.9, 0.2};
        building = {0.86, 0.6, 0.45};
        roof = {0.55, 0.13, 0.13};
        window = {0.95, 0.95, 1.0};
        winLines={0.2, 0.2, 0.25};
        door = {0.35, 0.18, 0.15};
        billboard = {0.98, 0.98, 0.95};
        text = {0.1, 0.1, 0.15};
    }
    else {
        platform = {0.18, 0.22, 0.32};
        edge = {0.5, 0.45, 0.2};
        building = {0.25, 0.18, 0.22};
        roof = {0.15, 0.05, 0.08};
        window = {1.0, 0.85, 0.3};
        winLines = {0.05, 0.05, 0.12};
        door = {0.12, 0.06, 0.04};
        billboard = {0.2, 0.22, 0.3};
        text = {0.9, 0.85, 0.5};
    }

    // Platform
    glColor3f(platform.r,platform.g,platform.b);
    drawRect(5200.0, 150.0, 760.0, 40.0);

    // Platform edge stripe
    glColor3f(edge.r,edge.g,edge.b);
    drawRect(5200.0, 190.0, 760.0, 6.0);

    // Station building base
    float bx = 5440.0, by = 190.0;
    float bw = 280.0, bh = 110.0;
    glColor3f(building.r,building.g,building.b);
    drawRect(bx, by, bw, bh);

    // Roof
    glColor3f(roof.r,roof.g,roof.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 10.0, by + bh);
    glVertex2f(bx + bw + 10.0, by + bh);
    glVertex2f(bx + bw / 2.0, by + bh + 60.0);
    glEnd();

    //billboard
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
    

    // Windows
    glColor3f(window.r,window.g,window.b);
    float ww = 40.0, wh = 50.0;
    float wy = by + 30.0;
    float doorCenterX = bx + bw / 2.0;
    float windowCenterOffset = 80.0;
    float wxLeft = doorCenterX - windowCenterOffset - ww / 2.0;
    float wxRight = doorCenterX + windowCenterOffset - ww / 2.0;

    // left window
    drawRect(wxLeft, wy, ww, wh);
    glColor3f(winLines.r,winLines.g,winLines.b);
    glBegin(GL_LINES);
    glVertex2f(wxLeft + ww / 2.0, wy);
    glVertex2f(wxLeft + ww / 2.0, wy + wh);
    glVertex2f(wxLeft, wy + wh / 2.0);
    glVertex2f(wxLeft + ww, wy + wh / 2.0);
    glEnd();

    // right window
    glColor3f(window.r,window.g,window.b);
    drawRect(wxRight, wy, ww, wh);
    glColor3f(winLines.r,winLines.g,winLines.b);
    glBegin(GL_LINES);
    glVertex2f(wxRight + ww / 2.0, wy);
    glVertex2f(wxRight + ww / 2.0, wy + wh);
    glVertex2f(wxRight, wy + wh / 2.0);
    glVertex2f(wxRight + ww, wy + wh / 2.0);
    glEnd();

    // Door
    glColor3f(door.r,door.g,door.b);
    drawRect(bx + bw / 2.0 - 22.0, by, 44.0, 70.0);
}



void landscape() {
    // Foreground scroll
    scrollX -= moveSpeed; 
    // Background scroll
    mountainScrollX -= mountainSpeed;

    // Reset loop
    if (scrollX < -resetPoint) {
        scrollX = 0.0f;
    }
    // Reset mountain loop
    if (mountainScrollX < -resetPoint) {
        mountainScrollX = 0.0f;
    }

    glPushMatrix();
    // Mountains move slower
    glTranslatef(mountainScrollX, 0.0f, 0.0f);
    // First mountain copy
    moutain();
    // Second mountain copy
    glTranslatef(resetPoint, 0.0f, 0.0f);
    moutain();
    glPopMatrix();

    glPushMatrix(); 
    // Buildings move faster
    glTranslatef(scrollX, 0.0f, 0.0f); 

    // First instance
    skyscraper();
    building();
    tree();
    drawStation();

    // Repeat instance for seamless scroll
    glTranslatef(resetPoint, 0.0f, 0.0f);
    skyscraper();
    building();
    tree();

    glPopMatrix(); 
}
