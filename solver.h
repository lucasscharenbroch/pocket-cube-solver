#ifndef SOLVER
#define SOLVER

#include <iostream> // TODO remove
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

using std::string;
using std::vector;
using std::numeric_limits;
using std::unordered_map;
using std::unordered_set;
using std::queue;
using std::cout;    // TODO remove
using std::endl;    // TODO remove
using std::ostream; // TODO remove

typedef unsigned char byte;
typedef unsigned __int128 uint128_t;

/* ~ ~ ~ ~ Constants/Enumerations ~ ~ ~ ~ */

// RGBA Color Values
constexpr int RED_RGBA = 0xFF0000FF;
constexpr int YELLOW_RGBA = 0xFF00FFFF;
constexpr int ORANGE_RGBA = 0xFF00A5FF;
constexpr int GREEN_RGBA = 0xFF00FF00;
constexpr int BLUE_RGBA = 0xFFFF0000;
constexpr int BLACK_RGBA = 0xFF000000;
constexpr int WHITE_RGBA = 0xFFFFFFFF;

// Color IDs
const byte BLUE    = 0;
const byte GREEN   = 1;
const byte ORANGE  = 2;
const byte RED     = 3;
const byte WHITE   = 4;
const byte YELLOW  = 5;

const char colorIdToChar[6] = {'B', 'G', 'O', 'R', 'W', 'Y'};


int colorIdToRGBA(byte id); // converts the given color Id (byte) to a RGBA value (int)

// Face IDs
const byte U = 0;
const byte L = 1;
const byte F = 2;
const byte R = 3;
const byte B = 4;
const byte D = 5;

// Cubie Location IDs (the funny indexing is so bitwise rotations can be used to rotate entire faces)
//  23
//  10
const byte BOT_RIGHT = 0;
const byte BOT_LEFT = 1;
const byte TOP_LEFT = 2;
const byte TOP_RIGHT = 3;

/* ~ ~ ~ ~ Geometric Structures ~ ~ ~ ~ */

struct Point {
    double x, y, z;
    int color; // for simplicity, all points are assigned a color (whether used or not).
    static const Point notAPoint; // a constant Point ({NaN, NaN, NaN}) that represents a null Point.

    Point(); // default constructor
    Point(const double x, const double y, const double z, const int color); // constructor (x, y, z, color = 0)
    Point(const Point& other); // constructor (point)
    Point(const Point& other, int color); // constructor (point, color overridden)
    Point& operator=(const Point& other); // copy assignment
    bool operator==(const Point& other) const; // equality operator
    bool operator!=(const Point& other) const; // inequality operator
    double dist(const Point& other) const; // distance
};

struct Vector {
    double dx, dy, dz;

    Vector(); // default constructor
    Vector(const double dx, const double dy, const double dz); // constructor (dx, dy, dz)
    Vector(const Vector& other); // copy constructor
    inline Vector(const Point& p1, const Point& p2); // constructor (point, point)
    Vector& operator=(const Vector& other); // copy assigment
    inline double dot(const Vector& other) const; // dot product
    inline Vector cross(const Vector& other) const; // cross product
};

struct Line {
    const Point p1;
    const Vector v;

    inline Line(const Point& p1, const Vector& v); // constructor (point, vector)
    inline Line(const Point& p1, const Point& p2); // constructor (point, point)
};

struct Plane {
    Point p1;
    Vector norm;

    Plane(); // default constructor
    Plane(const Point& p1, const Point& p2, const Point& p3); // constructor (point, point point)
    Point lineIntersection(const Line& l) const; // line-plane intersection
};

struct Rect {
    vector<Point> corners;
    Plane plane;

    Rect(); // default constructor
    Rect(const Point& a, const Point& b, const Point& c, const Point& d); // constructor (4 points)
    Rect(const Point& a, const Point& b, const Point& c, const Point& d,
               const int ca, const int cb, const int cc, const int cd); // constructor (4 points, overridden colors)
    Point lineIntersection(const Line& l) const; // line-rect intersection
};

struct POV {
    Point viewpoint;
    Rect screen;
    double width, height;
    double setback; // the distance between the viewpoint and the screen (screen.z - setback = viewpoint.z)

    POV(const double width, const double height, const double setback, const double viewDistance); // constructor
    Point renderPoint(const Point& p) const; // renders the given point
    Point getScreenPoint(const double x, const double y) const; // converts 2-d point on screen to 3-d point screen
    Point rectRaycast(const Rect& r, const double x, const double y) const; // finds the collision of the rect and the viewpoint ray
};

struct Cube {
    vector<Point> corners;

    static vector<int> cubieColors; // Up(4) Left(4) Front(4) Right(4) Back(4) Down(4)

    static void updateCubieColors();   // sets Cube::cubieColors according to the cubeState (PocketCube variable)

    Cube(const double x, const double y, const double z, const double s); // constructor (x0, y0, c0, side_length)
    Cube(const vector<Point>& corners); // constructor (from vector<point>)
    Cube rotate(const double xRad, const double yRad) const; // returns the cube, rotated by the given degrees
    vector<Rect> getFaces() const; // returns an array of face rects
};

/* ~ ~ ~ ~ Cube Interface ~ ~ ~ ~ */

class PocketCube {
public:
    vector<short> state; // {(U)p, (L)eft, (F)ront, (R)ight, (B)ack, (D)own}

    static short faceColor(byte topLeft, byte topRight, byte botLeft, byte botRight); // generates a face bitmask with the given colors
    static vector<byte> extractFaceColors(short face); // given a face bitset, the corresponding Color IDs are returned

    struct hash {
        size_t operator()(const PocketCube& c) const;
    };

    PocketCube(); // default constructor (init to solved state)
    PocketCube(short u, short l, short f, short r, short b, short d); // literal constructor
    PocketCube(const PocketCube& other); // copy constructor

    bool operator==(const PocketCube& other) const;
    bool operator!=(const PocketCube& other) const;
    PocketCube& operator=(const PocketCube& other); // copy assignment

    // (in-place) turning methods
    void turnU();
    void turnUP();
    void turnD();
    void turnDP();
    void turnF();
    void turnFP();
    void turnB();
    void turnBP();
    void turnL();
    void turnLP();
    void turnR();
    void turnRP();

    static const PocketCube solved; // solved state // TODO move this back to private
private:

    // face manipulation
    void rotateC(byte face);
    void rotateCC(byte face);
    byte shiftUpperCells(byte face, byte carry);
    byte shiftLowerCells(byte face, byte carry);
    byte shiftLeftCells(byte face, byte carry);
    byte shiftRightCells(byte face, byte carry);
};

/* ~ ~ ~ ~ Solving ~ ~ ~ ~ */

vector<byte> solve(const PocketCube& startNode);

/* ~ ~ ~ ~ Debug ~ ~ ~ ~ */

ostream& operator<<(ostream& os, const PocketCube& pc);

/* ~ ~ ~ ~ Runtime Variables ~ ~ ~ ~ */

// SOLVING (state)
extern PocketCube cubeState;

/* ~ ~ ~ ~ Exported Functions  ~ ~ ~ ~ */

extern "C" {
    int *getImageDataBuffer();
    void draw();
    void setRotation(double, double);
}

#endif // SOLVER
