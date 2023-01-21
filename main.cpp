#include "solver.h"

/* ~ ~ ~ ~ Graphics / JS Interface ~ ~ ~ ~ */

constexpr int WIDTH = 300;
constexpr int HEIGHT = 300;
int buffer[WIDTH * HEIGHT];         // 1d array representing 2d RGBA grid; (x, y) => buffer[y * WIDTH + x]
double xRotRad = 0, yRotRad = 0;    // rotation of cube (in radians)
POV pov(WIDTH, HEIGHT, 300, 3000);  // point-of-view for rendering
Cube cube(-100, -100, -100, 200);   // geometric cube to be rendered
PocketCube cubeState;               // state of the cube during program execution

int *getImageDataBuffer() {
    return buffer;
}

// draw: renders the cube and writes its color data in the buffer.
void draw() {
    Cube c = cube.rotate(xRotRad, yRotRad);

    Cube::updateCubieColors();
    vector<Rect> faces = c.getFaces();
    Point collision;

    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            double color = WHITE_RGBA;
            double bestZ = 1e10; // z-value of point closest to the viewpoint
            for(int f = 0; f < faces.size(); f++) {
                collision = pov.rectRaycast(faces[f], j, HEIGHT - i - 1);

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

void setRotation(double x, double y) {
    xRotRad = x;
    yRotRad = y;
}

// init: called once upon page-start
void init() {
    cubeState = PocketCube::solved; // copy again here in case solved was initialized after cubeState
}

// executeTurn: executes the given turn ID
void executeTurn(int turnId) {
    switch(turnId) {
        case 0:  cubeState.turnU();  return;
        case 1:  cubeState.turnL();  return;
        case 2:  cubeState.turnF();  return;
        case 3:  cubeState.turnR();  return;
        case 4:  cubeState.turnB();  return;
        case 5:  cubeState.turnD();  return;
        case 6:  cubeState.turnUP(); return;
        case 7:  cubeState.turnLP(); return;
        case 8:  cubeState.turnFP(); return;
        case 9:  cubeState.turnRP(); return;
        case 10: cubeState.turnBP(); return;
        case 11: cubeState.turnDP(); return;
    }
}

vector<byte> cubieColorBuffer;

byte *getCubieColors() {
    int i = 0;
    for(const short& face : cubeState.state) {
        for(const byte& colorId : PocketCube::extractFaceColors(face)) {
            cubieColorBuffer[i++] = colorId;
        }
    }

    return cubieColorBuffer.data();
}

vector<byte> solveBuffer; // contains solution moves that can be transfered to js

int solveCube() {
    solveBuffer = solve(cubeState);
    return solveBuffer.size();
}

byte *getSolveBuffer() {
    return solveBuffer.data();
}

// TODO here: 
// - clean code
// - polish
