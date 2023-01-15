#include "solver.h"

constexpr int WIDTH = 300;
constexpr int HEIGHT = 300;

// TODO find suitable place for this function
// colorIdToRGBA: converts the given color Id (byte) to a RGBA value (int)
int colorIdToRGBA(byte id) {
    switch(id) {
        case RED:    return RED_RGBA;
        case BLUE:   return BLUE_RGBA;
        case GREEN:  return GREEN_RGBA;
        case WHITE:  return WHITE_RGBA;
        case ORANGE: return ORANGE_RGBA;
        case YELLOW: return YELLOW_RGBA;
    }

    return BLACK_RGBA; // invalid ID: return BLACK_RGBA
}

// GRAPHICS
int buffer[WIDTH * HEIGHT];
double xRotRad = 0, yRotRad = 0;
POV pov(WIDTH, HEIGHT, 300, 3000);
Cube cube(-100, -100, -100, 200);

// TODO update the below

int *getImageDataBuffer() {
    // cout << "getting image data buffer [from c++] [new]" << endl;
    return buffer;
}

void setRotation(double x, double y) {
    xRotRad = x;
    yRotRad = y;
}

void draw() {
    // cout << "draw function [from c++] [new]" << endl;
    // cout << "rotation: " << xRotRad << " " << yRotRad << endl;

    Cube c = cube.rotate(xRotRad, yRotRad);

    Cube::updateCubieColors();
    vector<Rect> faces = c.getFaces();
    Point collision;

    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            double color = BLACK_RGBA;
            double bestZ = numeric_limits<double>::max();
            for(int f = 0; f < faces.size(); f++) {

                collision = pov.rectRaycast(faces[f], HEIGHT - i - 1, j);

                if(collision == Point::notAPoint) continue;
                if(collision.z < bestZ) {
                    bestZ = collision.z;
                    color = collision.color;
                }
            }
            buffer[i * WIDTH + j] = color;
        }
    }
}

// SOLVING state
PocketCube cubeState;


// TODO test method: remove this later
PocketCube& turn1(PocketCube& c, int turnId) {
    switch(turnId) {
        case 0:  c.turnU();  return c;
        case 1:  c.turnL();  return c;
        case 2:  c.turnF();  return c;
        case 3:  c.turnR();  return c;
        case 4:  c.turnB();  return c;
        case 5:  c.turnD();  return c;
        case 6:  c.turnUP(); return c;
        case 7:  c.turnLP(); return c;
        case 8:  c.turnFP(); return c;
        case 9:  c.turnRP(); return c;
        case 10: c.turnBP(); return c;
        case 11: c.turnDP(); return c;
    }

    return c; // if turnId is invalid (which shouldn't happen), return c
}

// TODO test
const vector<string> moveNames = {"U", "L", "F", "R", "B", "D", "UP", "LP", "FP", "RP", "BP", "DP"};

// TODO test method: remove this later
int main() {

const byte BLUE    = 0;
const byte GREEN   = 1;
const byte ORANGE  = 2;
const byte RED     = 3;
const byte WHITE   = 4;
const byte YELLOW  = 5;

    PocketCube state = {PocketCube::faceColor(BLUE, BLUE, GREEN, YELLOW),
                        PocketCube::faceColor(WHITE, RED, BLUE, GREEN),
                        PocketCube::faceColor(WHITE, BLUE, YELLOW, GREEN),
                        PocketCube::faceColor(RED, WHITE, ORANGE, RED),
                        PocketCube::faceColor(RED, ORANGE, YELLOW, YELLOW),
                        PocketCube::faceColor(ORANGE, WHITE, ORANGE, GREEN)};

    cout << state << endl;

    vector<byte> moves = solve(state);
    for(const byte& move : moves) {
        cout << moveNames[(int) move] << endl;
        turn1(state, move);
    }

    cout << state << endl;

    // TODO here-o: clean things up, fix the obvious graphical problem.
}
