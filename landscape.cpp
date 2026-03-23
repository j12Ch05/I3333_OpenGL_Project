GLfloat scrollX = 0.0f;
GLfloat moveSpeed = 3.0f;
GLfloat resetPoint = 7600.0f;
GLfloat mountainScrollX = 0.0f;
GLfloat mountainSpeed = 1.2f;
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