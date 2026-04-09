#include <GL/glut.h>
#include <cmath>
#include <algorithm>

const int WIDTH  = 1000;
const int HEIGHT = 650;
const float PI   = 3.1415926f;

float cloudOffset   = 0.0f;
float birdOffset    = 0.0f;
float boatOffset    = 0.0f;
float windmillAngle = 0.0f;
float smokePhase    = 0.0f;
float birdPhase     = 0.0f;
float waterShift    = 0.0f;
float sunPulse      = 0.0f;
float speed         = 1.0f;

bool isNight = false;

void drawCircle(float cx, float cy, float r, int segments = 80) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void drawSky() {
    glBegin(GL_QUADS);
    if (!isNight) {
        glColor3f(0.20f, 0.62f, 0.98f); glVertex2f(0, HEIGHT);
        glColor3f(0.20f, 0.62f, 0.98f); glVertex2f(WIDTH, HEIGHT);
        glColor3f(0.92f, 0.96f, 1.00f); glVertex2f(WIDTH, 200);
        glColor3f(0.92f, 0.96f, 1.00f); glVertex2f(0, 200);
    } else {
        glColor3f(0.03f, 0.05f, 0.16f); glVertex2f(0, HEIGHT);
        glColor3f(0.03f, 0.05f, 0.16f); glVertex2f(WIDTH, HEIGHT);
        glColor3f(0.16f, 0.18f, 0.30f); glVertex2f(WIDTH, 200);
        glColor3f(0.16f, 0.18f, 0.30f); glVertex2f(0, 200);
    }
    glEnd();
}

void drawStars() {
    if (!isNight) return;

    const float stars[][2] = {
        {80, 590}, {160, 560}, {230, 610}, {310, 545}, {390, 600},
        {470, 565}, {550, 615}, {640, 550}, {710, 600}, {790, 560},
        {860, 610}, {930, 580}, {120, 510}, {285, 500}, {455, 520},
        {615, 495}, {770, 515}, {910, 500}
    };

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.9f);
    for (auto &s : stars) glVertex2f(s[0], s[1]);
    glEnd();
}

void drawSunOrMoon() {
    if (!isNight) {
        float pulse = 1.0f + 0.04f * sin(sunPulse);

        // Glow
        glColor4f(1.0f, 0.88f, 0.35f, 0.25f);
        drawCircle(120, 530, 55 * pulse);

        // Sun
        glColor3f(1.0f, 0.85f, 0.10f);
        drawCircle(120, 530, 34 * pulse);

        // Rays
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.78f, 0.10f);
        for (int i = 0; i < 12; i++) {
            float a = 2.0f * PI * i / 12.0f;
            float x1 = 120 + cos(a) * 44.0f;
            float y1 = 530 + sin(a) * 44.0f;
            float x2 = 120 + cos(a) * 62.0f;
            float y2 = 530 + sin(a) * 62.0f;
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
        glEnd();
        glLineWidth(1.0f);
    } else {
        glColor4f(0.95f, 0.95f, 1.0f, 0.18f);
        drawCircle(120, 530, 46);

        glColor3f(0.92f, 0.92f, 0.98f);
        drawCircle(120, 530, 28);

        // Crescent cut
        glColor3f(0.08f, 0.10f, 0.20f);
        drawCircle(132, 536, 24);
    }
}

void drawMountains() {
    glBegin(GL_TRIANGLES);

    if (!isNight) glColor3f(0.42f, 0.39f, 0.42f);
    else          glColor3f(0.18f, 0.18f, 0.24f);
    glVertex2f(0, 200); glVertex2f(160, 390); glVertex2f(320, 200);

    if (!isNight) glColor3f(0.36f, 0.34f, 0.38f);
    else          glColor3f(0.14f, 0.14f, 0.20f);
    glVertex2f(180, 200); glVertex2f(370, 340); glVertex2f(560, 200);

    if (!isNight) glColor3f(0.40f, 0.36f, 0.40f);
    else          glColor3f(0.16f, 0.16f, 0.21f);
    glVertex2f(430, 200); glVertex2f(670, 430); glVertex2f(900, 200);

    if (!isNight) glColor3f(0.34f, 0.31f, 0.36f);
    else          glColor3f(0.12f, 0.12f, 0.18f);
    glVertex2f(720, 200); glVertex2f(920, 340); glVertex2f(1100, 200);

    glEnd();
}

void drawGround() {
    if (!isNight) glColor3f(0.20f, 0.68f, 0.22f);
    else          glColor3f(0.10f, 0.35f, 0.12f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WIDTH, 0);
    glVertex2f(WIDTH, 210);
    glVertex2f(0, 210);
    glEnd();

    // Small grass spikes
    if (!isNight) glColor3f(0.12f, 0.52f, 0.14f);
    else          glColor3f(0.08f, 0.24f, 0.09f);

    for (int i = 0; i < WIDTH; i += 25) {
        glBegin(GL_TRIANGLES);
        glVertex2f(i, 210);
        glVertex2f(i + 6, 224);
        glVertex2f(i + 12, 210);
        glEnd();
    }
}

void drawRiver() {
    if (!isNight) glColor3f(0.10f, 0.42f, 0.88f);
    else          glColor3f(0.06f, 0.18f, 0.38f);

    glBegin(GL_QUADS);
    glVertex2f(0, 70);
    glVertex2f(WIDTH, 70);
    glVertex2f(WIDTH, 155);
    glVertex2f(0, 155);
    glEnd();

    // Water shine / ripple
    glColor4f(1.0f, 1.0f, 1.0f, isNight ? 0.12f : 0.28f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = -100; i < WIDTH + 100; i += 45) {
        float x = i + fmod(waterShift, 45.0f);
        glVertex2f(x, 95);
        glVertex2f(x + 26, 112);

        glVertex2f(x + 10, 128);
        glVertex2f(x + 34, 142);
    }
    glEnd();
    glLineWidth(1.0f);
}

void drawPath() {
    glColor3f(0.78f, 0.68f, 0.46f);
    glBegin(GL_QUADS);
    glVertex2f(470, 155);
    glVertex2f(560, 155);
    glVertex2f(625, 210);
    glVertex2f(505, 210);
    glEnd();
}

void drawFence(float x1, float x2, float y) {
    glColor3f(0.55f, 0.32f, 0.12f);

    for (float x = x1; x <= x2; x += 18.0f) {
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 6, y);
        glVertex2f(x + 6, y + 26);
        glVertex2f(x, y + 26);
        glEnd();
    }

    glBegin(GL_QUADS);
    glVertex2f(x1 - 4, y + 7);
    glVertex2f(x2 + 10, y + 7);
    glVertex2f(x2 + 10, y + 11);
    glVertex2f(x1 - 4, y + 11);

    glVertex2f(x1 - 4, y + 17);
    glVertex2f(x2 + 10, y + 17);
    glVertex2f(x2 + 10, y + 21);
    glVertex2f(x1 - 4, y + 21);
    glEnd();
}

void drawSmoke(float x, float y) {
    glColor4f(0.78f, 0.78f, 0.78f, 0.55f);
    drawCircle(x + 0.0f,  y + 0.0f  + 10 * sin(smokePhase), 8);
    drawCircle(x - 8.0f,  y + 18.0f + 12 * sin(smokePhase + 0.6f), 11);
    drawCircle(x + 4.0f,  y + 38.0f + 14 * sin(smokePhase + 1.1f), 9);
    drawCircle(x - 6.0f,  y + 57.0f + 15 * sin(smokePhase + 1.5f), 7);
}

void drawHut(float x, float y, float scale = 1.0f, bool chimney = false) {
    float w = 95 * scale;
    float h = 68 * scale;

    // Body
    glColor3f(0.88f, 0.60f, 0.34f);
    glBegin(GL_QUADS);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();

    // Roof
    glColor3f(0.56f, 0.16f, 0.10f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 12 * scale,      y + h);
    glVertex2f(x + w / 2.0f,        y + h + 54 * scale);
    glVertex2f(x + w + 12 * scale,  y + h);
    glEnd();

    // Door
    glColor3f(0.36f, 0.20f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(x + 36 * scale, y);
    glVertex2f(x + 58 * scale, y);
    glVertex2f(x + 58 * scale, y + 42 * scale);
    glVertex2f(x + 36 * scale, y + 42 * scale);
    glEnd();

    // Window
    glColor3f(0.96f, 0.90f, 0.60f);
    glBegin(GL_QUADS);
    glVertex2f(x + 68 * scale, y + 28 * scale);
    glVertex2f(x + 84 * scale, y + 28 * scale);
    glVertex2f(x + 84 * scale, y + 46 * scale);
    glVertex2f(x + 68 * scale, y + 46 * scale);
    glEnd();

    // Window cross
    glColor3f(0.55f, 0.34f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(x + 75 * scale, y + 28 * scale);
    glVertex2f(x + 77 * scale, y + 28 * scale);
    glVertex2f(x + 77 * scale, y + 46 * scale);
    glVertex2f(x + 75 * scale, y + 46 * scale);

    glVertex2f(x + 68 * scale, y + 36 * scale);
    glVertex2f(x + 84 * scale, y + 36 * scale);
    glVertex2f(x + 84 * scale, y + 38 * scale);
    glVertex2f(x + 68 * scale, y + 38 * scale);
    glEnd();

    // Roof lines
    glColor3f(0.40f, 0.10f, 0.07f);
    for (int i = 0; i < 5; i++) {
        float xx = x + 8 * scale + i * 18 * scale;
        glBegin(GL_LINES);
        glVertex2f(xx, y + h + 5 * scale);
        glVertex2f(x + w / 2.0f, y + h + 48 * scale);
        glEnd();
    }

    // Chimney
    if (chimney) {
        glColor3f(0.42f, 0.24f, 0.16f);
        glBegin(GL_QUADS);
        glVertex2f(x + 62 * scale, y + h + 20 * scale);
        glVertex2f(x + 74 * scale, y + h + 20 * scale);
        glVertex2f(x + 74 * scale, y + h + 52 * scale);
        glVertex2f(x + 62 * scale, y + h + 52 * scale);
        glEnd();

        drawSmoke(x + 68 * scale, y + h + 62 * scale);
    }
}

void drawTree(float x, float y, float scale = 1.0f) {
    // Trunk
    glColor3f(0.44f, 0.26f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(x,                 y);
    glVertex2f(x + 18 * scale,    y);
    glVertex2f(x + 18 * scale,    y + 70 * scale);
    glVertex2f(x,                 y + 70 * scale);
    glEnd();

    // Leaves
    if (!isNight) glColor3f(0.06f, 0.55f, 0.12f);
    else          glColor3f(0.08f, 0.28f, 0.12f);

    drawCircle(x + 9 * scale,  y + 80 * scale, 28 * scale);
    drawCircle(x - 6 * scale,  y + 98 * scale, 24 * scale);
    drawCircle(x + 24 * scale, y + 100 * scale, 25 * scale);
    drawCircle(x + 9 * scale,  y + 120 * scale, 20 * scale);
}

void drawCloud(float x, float y, float scale = 1.0f) {
    glColor4f(1.0f, 1.0f, 1.0f, isNight ? 0.75f : 0.95f);
    drawCircle(x,                  y,                  22 * scale);
    drawCircle(x + 24 * scale,     y + 10 * scale,     28 * scale);
    drawCircle(x + 54 * scale,     y + 2  * scale,     24 * scale);
    drawCircle(x + 36 * scale,     y - 9  * scale,     18 * scale);
}

void drawBird(float x, float y, float flap) {
    glColor3f(0.04f, 0.04f, 0.06f);
    glLineWidth(2.2f);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 16, y + 7 + flap);

    glVertex2f(x + 16, y + 7 + flap);
    glVertex2f(x + 32, y);

    glVertex2f(x, y);
    glVertex2f(x - 7, y + 3);
    glEnd();
    glLineWidth(1.0f);
}

void drawBoat(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Boat body
    glColor3f(0.46f, 0.22f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(60, 0);
    glVertex2f(50, 18);
    glVertex2f(10, 18);
    glEnd();

    // Mast
    glColor3f(0.30f, 0.20f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(28, 18);
    glVertex2f(32, 18);
    glVertex2f(32, 58);
    glVertex2f(28, 58);
    glEnd();

    // Sail
    glColor3f(0.98f, 0.96f, 0.90f);
    glBegin(GL_TRIANGLES);
    glVertex2f(32, 56);
    glVertex2f(32, 24);
    glVertex2f(58, 40);
    glEnd();

    glPopMatrix();
}

void drawWindmill(float x, float y, float scale = 1.0f) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Stand
    glColor3f(0.56f, 0.34f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(-10 * scale, 0);
    glVertex2f( 10 * scale, 0);
    glVertex2f( 18 * scale, 115 * scale);
    glVertex2f(-18 * scale, 115 * scale);
    glEnd();

    // Hub
    glTranslatef(0, 115 * scale, 0);
    glColor3f(0.85f, 0.78f, 0.60f);
    drawCircle(0, 0, 7 * scale);

    glRotatef(windmillAngle, 0, 0, 1);

    for (int i = 0; i < 4; i++) {
        glRotatef(90, 0, 0, 1);
        glBegin(GL_TRIANGLES);
        glColor3f(0.95f, 0.95f, 0.95f);
        glVertex2f(0, 0);
        glVertex2f(55 * scale, 10 * scale);
        glVertex2f(55 * scale, -10 * scale);
        glEnd();
    }

    glPopMatrix();
}

void drawBush(float x, float y, float scale = 1.0f) {
    if (!isNight) glColor3f(0.12f, 0.48f, 0.12f);
    else          glColor3f(0.08f, 0.24f, 0.10f);

    drawCircle(x, y, 20 * scale);
    drawCircle(x + 18 * scale, y + 6 * scale, 18 * scale);
    drawCircle(x + 34 * scale, y, 16 * scale);
}

void drawSceneObjects() {
    drawMountains();
    drawSunOrMoon();
    drawGround();
    drawRiver();
    drawPath();

    // Fences
    drawFence(30, 220, 210);
    drawFence(315, 455, 210);
    drawFence(650, 900, 210);

    // Huts
    drawHut(70, 210, 1.0f, true);
    drawHut(330, 210, 1.05f, false);
    drawHut(700, 210, 0.95f, false);

    // Trees
    drawTree(220, 210, 1.0f);
    drawTree(520, 210, 1.15f);
    drawTree(620, 210, 0.95f);
    drawTree(910, 210, 1.0f);

    // Bushes
    drawBush(275, 218, 1.0f);
    drawBush(575, 220, 0.9f);
    drawBush(840, 220, 1.1f);

    // Windmill
    drawWindmill(610, 230, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawStars();
    drawSceneObjects();

    // Moving clouds
    glPushMatrix();
    glTranslatef(cloudOffset, 0, 0);
    drawCloud(40,  510, 1.05f);
    drawCloud(300, 560, 0.95f);
    drawCloud(610, 500, 1.15f);
    glPopMatrix();

    // Moving birds
    glPushMatrix();
    glTranslatef(birdOffset, 0, 0);
    drawBird(70,  450, 6.0f * sin(birdPhase));
    drawBird(140, 485, 7.0f * sin(birdPhase + 0.7f));
    drawBird(220, 455, 5.5f * sin(birdPhase + 1.2f));
    drawBird(320, 475, 6.5f * sin(birdPhase + 1.9f));
    glPopMatrix();

    // Moving boat
    drawBoat(boatOffset, 102);

    glutSwapBuffers();
}

void update(int value) {
    cloudOffset   += 0.35f * speed;
    birdOffset    += 1.45f * speed;
    boatOffset    += 0.65f * speed;
    windmillAngle -= 3.5f * speed;
    smokePhase    += 0.045f * speed;
    birdPhase     += 0.22f * speed;
    waterShift    += 1.20f * speed;
    sunPulse      += 0.03f * speed;

    if (cloudOffset > WIDTH + 140) cloudOffset = -350.0f;
    if (birdOffset  > WIDTH + 120) birdOffset  = -420.0f;
    if (boatOffset  > WIDTH + 80)  boatOffset  = -120.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '+':
        case '=':
            speed += 0.2f;
            if (speed > 5.0f) speed = 5.0f;
            break;

        case '-':
        case '_':
            speed -= 0.2f;
            if (speed < 0.2f) speed = 0.2f;
            break;

        case 'r':
        case 'R':
            cloudOffset = 0.0f;
            birdOffset = 0.0f;
            boatOffset = 0.0f;
            windmillAngle = 0.0f;
            smokePhase = 0.0f;
            birdPhase = 0.0f;
            waterShift = 0.0f;
            sunPulse = 0.0f;
            speed = 1.0f;
            isNight = false;
            break;

        case 'n':
        case 'N':
            isNight = !isNight;
            break;

        case 27: // ESC
            std::exit(0);
            break;
    }
}

void init() {
    glClearColor(0.85f, 0.92f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Innovative Village Scenery - GLUT");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
