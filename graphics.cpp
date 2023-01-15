#include "solver.h"

/* ~ ~ ~ ~ Geometric Structures ~ ~ ~ ~ */

// Point: represents a 3-dimensional point with a color (the latter is sometimes ignored)
// {
    // default constructor
    Point::Point() { }

    // constructor (x, y, z) => (x, y, z)
    Point::Point(const double x, const double y, const double z, const int color = 0) : 
                 x(x), y(y), z(z), color(color) { }

    // copy constructor
    Point::Point(const Point& other) : x(other.x), y(other.y), z(other.z), color(other.color) { }

    // copy constructor with color overridden
    Point::Point(const Point& other, int color) : x(other.x), y(other.y), z(other.z), color(color) { }

    // copy assignment
    Point& Point::operator=(const Point& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        color = other.color;

        return *this;
    }

    // equality operator (only compares (x, y, z) (NOT color))
    bool Point::operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // inequality operator (opposite of equality)
    bool Point::operator!=(const Point& other) const {
        return !(*this == other);
    }

    // dist(other): computes the 3-dimensional distance between *this and other
    double Point::dist(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;

        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    const Point Point::notAPoint{NAN, NAN, NAN};
// }

// Vector: a 3-dimensional vector
// {
    // default constructor
    Vector::Vector() { }

    // constructor
    Vector::Vector(const double dx, const double dy, const double dz) : dx(dx), dy(dy), dz(dz) { }

    // copy constructor
    Vector::Vector(const Vector& other) : dx(other.dx), dy(other.dy), dz(other.dz) { }

    // from-points constructor
    inline Vector::Vector(const Point& p1, const Point& p2) : dx(p1.x - p2.x), 
                                                              dy(p1.y - p2.y),
                                                              dz(p1.z - p2.z) { }

    // copy assignment
    Vector& Vector::operator=(const Vector& other) {
        this->dx = other.dx;
        this->dy = other.dy;
        this->dz = other.dz;
        
        return *this;
    }

    // dot: computes the dot product between *this and other
    inline double Vector::dot(const Vector& other) const {
        return dx * other.dx + dy * other.dy + dz * other.dz;
    }

    // cross: computes the cross product between *this and other
    Vector Vector::cross(const Vector& other) const {
        return Vector(dy * other.dz - dz * other.dy,
                      dz * other.dx - dx * other.dz,
                      dx * other.dy - dy * other.dx);
    }
// } 

// Line: a 3-dimensional line (an initial point and direction vector)
// {
    // constructor (point and vector)
    inline Line::Line(const Point& p1, const Vector& v) : p1(p1), v(v) { }

    // constructor (two points)
    inline Line::Line(const Point& p1, const Point& p2) : p1(p1), v(p1, p2) { }
// }

// Plane: a 3-dimensional plane
// {
    // default constructor
    Plane::Plane() { }

    // constructor (three points)
    Plane::Plane(const Point& p1, const Point& p2, const Point& p3) : 
                 p1(p1), norm{Vector{p1, p2}.cross(Vector{p1, p3})} { }

    // lineIntersection: computes the intersection between the given line and *this, and returns
    // the resultant point (or Point::notAPoint if the line and *this are parallel)
    Point Plane::lineIntersection(const Line& l) const {
        if(l.v.dot(norm) == 0) return Point::notAPoint;

        const double A = norm.dx;
        const double B = norm.dy;
        const double C = norm.dz;
        const double D = A * p1.x + B * p1.y + C * p1.z;

        const double x0 = l.p1.x;
        const double y0 = l.p1.y;
        const double z0 = l.p1.z;

        // calculate t value in parametric equation of the line
        // x = x0 + dx * t
        // y = y0 + dy * t
        // z = z0 + dz * t
        //
        // Ax + By + Cz = D
        //
        // t = (D - (A*x0 + B*y0 + C*z0)) / (A*dx + B*dy + C*dz)
        
        const double t = (D - (A * x0 + B * y0 + C * z0)) / (A * l.v.dx + B * l.v.dy + C * l.v.dz);
        return Point{l.p1.x + l.v.dx * t, l.p1.y + l.v.dy * t, l.p1.z + l.v.dz * t};
    }
// }

// Rect: represents a 2-dimensional quadrilateral on a 3-dimensional plane (note that the angles
// between the sides are not checked, so Rects are not necessarily rectangles, though they are used
// as such; the plane is also unchecked with the forth point (it is constructed with the first three))
// {
    // default constructor
    Rect::Rect() { }

    // constructor (four points)
    Rect::Rect(const Point& a, const Point& b, const Point& c, const Point& d) : corners{a, b, c, d}, plane{a, b, c} { }

    // constructor (four points with overridden colors)
    Rect::Rect(const Point& a, const Point& b, const Point& c, const Point& d, 
               const int ca, const int cb, const int cc, const int cd) :
               corners{{a, ca}, {b, cb}, {c, cc}, {d, cd}}, plane{a, b, c} { }


    // lineIntersection: computes the point of intersection between *this and l; if they do not
    // intersect, Point::notAPoint is returned.
    Point Rect::lineIntersection(const Line& l) const {
        Point planeIntersection = plane.lineIntersection(l);
        if(planeIntersection == Point::notAPoint) return Point::notAPoint;

        // ensure that the point lies within the rect
        
        if(Vector{corners[0], corners[1]}.dot(Vector{corners[0], planeIntersection}) < 0) return Point::notAPoint;

        if(Vector{corners[1], corners[2]}.dot(Vector{corners[1], planeIntersection}) < 0) return Point::notAPoint;

        if(Vector{corners[2], corners[3]}.dot(Vector{corners[2], planeIntersection}) < 0) return Point::notAPoint;

        if(Vector{corners[3], corners[0]}.dot(Vector{corners[3], planeIntersection}) < 0) return Point::notAPoint;

        // set the color of the intersection point to the color of the nearest corner
        planeIntersection.color = corners[0].color;
        double minDist = corners[0].dist(planeIntersection);
        double currentDist;
        for(const Point& corner : corners) {
            if((currentDist = corner.dist(planeIntersection)) < minDist) {
                planeIntersection.color = corner.color;
                minDist = currentDist;
            }
        }

        return planeIntersection;
    }
// }

// POV: represents a viewpoint (3-d point) and screen (2-d rectangle); provides
// raycasting methods to render 3-d points into 2-d coordinates on the screen.
// {
    // constructor
    POV::POV(const double width, const double height, const double setback, const double viewDistance) {
        this->width = width;
        this->height = height;
        this->setback = setback;

        Point topLeft(-width / 2, height / 2, -setback);
        Point topRight(width / 2, height / 2, -setback);
        Point botRight(width / 2, -height / 2, -setback);
        Point botLeft(-width / 2, -height / 2, -setback);

        screen = {topLeft, topRight, botRight, botLeft};
        viewpoint = {0, 0, -setback - viewDistance};
    }

    // renderPoint: determines the 2-d coordinates (returned through a 3-d Point) of a 3-d
    // point rendered onto the screen (the line between the viewpoint and the given point is
    // calculated, and its intersection with the screen is returned)
    Point POV::renderPoint(const Point& p) const {
        Point i = screen.lineIntersection({p, viewpoint});
        if(i == Point::notAPoint) return Point::notAPoint;
        return {i.x + width / 2, i.y + height / 2, 0};
    }

    // returns 3d point on the screen corresponding to the 2d point given
    Point POV::getScreenPoint(const double x, const double y) const {
        return {x - width / 2, y - height / 2, -setback};
    }

    // returns a 3-d point on the given rect (with color) that responds to the 2-d on-screen point
    // given. (The line between the screen point and the viewpoint is calculated, and its interseciton
    // with the given rect is returned)
    Point POV::rectRaycast(const Rect& r, const double x, const double y) const {
        return r.lineIntersection({getScreenPoint(x, y), viewpoint});
    }
// }

// Cube: represents a 3-d object with 8 corners. Cubie-color is also held and
// used in rendering.
// {
    /* constructor (x0, y0, z0, s (side-length))
     *
     *   6 -- 7
     *  /|   /|
     * 2 ---3 |
     * | 4  | 5
     * 0----1
     *
     * y  z
     * | /
     * |/
     * /---- x
     *
     */
    Cube::Cube(const double x, const double y, const double z, const double s) {
             corners = {
                            {x, y, z}, {x + s, y, z},
                            {x, y + s, z}, {x + s, y + s, z},
                            {x, y, z + s}, {x + s, y, z + s},
                            {x, y + s, z + s}, {x + s, y + s, z + s} 
                       };
    }

    // constructor (from corners)
    Cube::Cube(const vector<Point>& corners): corners(corners) { }

    // rotate: returns a Cube rotated by the given degrees
    Cube Cube::rotate(const double xRad, const double yRad) const {
        vector<Point> rotatedCorners = corners;
        for(Point& corner : rotatedCorners) {
            double x = corner.x;
            double y = corner.y;
            double z = corner.z;
            double cosx = cos(xRad);
            double cosy = cos(yRad);
            double sinx = sin(xRad);
            double siny = sin(yRad);

            corner.x = x * cosy + z * siny;
            corner.y = x * sinx * siny + y * cosx - z * sinx * cosy;
            corner.z = -x * siny * cosx + y * sinx + z * cosx * cosy;
        }
        
        return {rotatedCorners};
    }

    vector<int> Cube::cubieColors(24); // temporary array of RGBA colors

    // updateCubieColors: sets Cube::cubieColors to the appropriate RGBA values according to the cubeState
    void Cube::updateCubieColors() {
        int i = 0;
        for(const short& face : cubeState.state) {
            for(const byte& colorId : PocketCube::extractFaceColors(face)) {
                cubieColors[i++] = colorIdToRGBA(colorId);
            }
        }
    }

    // getFaces: returns a vector of Rect objects with their corner colors corresponding to their
    // respective cubie colors.
    vector<Rect> Cube::getFaces() const {
        return {
                {corners[2], corners[6], corners[7], corners[3], cubieColors[0],  cubieColors[1],  cubieColors[2],  cubieColors[3]},  // Top
                {corners[4], corners[6], corners[2], corners[0], cubieColors[4],  cubieColors[5],  cubieColors[6],  cubieColors[7]},  // Left
                {corners[0], corners[2], corners[3], corners[1], cubieColors[8],  cubieColors[9],  cubieColors[10], cubieColors[11]}, // Front
                {corners[1], corners[3], corners[7], corners[5], cubieColors[12], cubieColors[13], cubieColors[14], cubieColors[15]}, // Right
                {corners[5], corners[7], corners[6], corners[4], cubieColors[16], cubieColors[17], cubieColors[18], cubieColors[19]}, // Back
                {corners[0], corners[4], corners[5], corners[1], cubieColors[20], cubieColors[21], cubieColors[22], cubieColors[23]}  // Bottom
        };
    }
// }
