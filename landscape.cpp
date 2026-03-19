GLfloat scrollX = 0.0f;
GLfloat moveSpeed = 3.0f;
GLfloat resetPoint = 6500.0f;
GLint numberOfBuildings = 12;
GLint numberOfSkyscrapers = 4;
GLint numberofMountains = 2;
GLint numberOfTrees = 25;
struct vertex {
    GLfloat x, y;
};

struct color {
    GLfloat r, g, b;
};

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
    GLfloat ground_y = 150.0f;
    GLfloat gap = 800.0f;
    

    for (GLint i = 0; i < numberofMountains; i++) {
        GLfloat width = 500.0f; 
        GLfloat height = 480.0f;

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

        //adding the snow
        glColor3f(snow.r, snow.g, snow.b);
        glBegin(GL_POLYGON);
        glVertex2f(current_x + 120.5f, height - 50.0f);
        glVertex2f(current_x + 150.0f, height);
        glVertex2d(current_x + 210.0f, height);
        glVertex2f(current_x + 255.0f, height - 50.0f);
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



void landscape() {
    scrollX -= moveSpeed; 

    // Reset loop
    if (scrollX < -resetPoint) {
        scrollX = 0.0f;
    }

    glPushMatrix(); 
    glTranslatef(scrollX, 0.0f, 0.0f); 

    // First instance
    moutain();
    skyscraper();
    building();
    tree();

    // Repeat instance for seamless scroll
    glTranslatef(resetPoint, 0.0f, 0.0f);
    moutain();
    skyscraper();
    building();

    glPopMatrix(); 
}