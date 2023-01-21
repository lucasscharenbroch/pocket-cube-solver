#include "solver.h"

/* ~ ~ ~ ~ Cube Interface ~ ~ ~ ~ */

// class PocketCube
/* {
 * 
 * represents a 2x2x2 rubik's cube as an array of six shorts
 * 
 * U = Upward-Facing
 * F = Front-Facing
 * D = Downward-Facing
 * L = Left-Facing
 * R = Right-Facing
 * B = Back-Facing
 * 
 *         UU
 *         UU
 *      LL FF RR BB
 *      LL FF RR BB
 *         DD
 *         DD
 *         
 * 24 cubies with 6 possible states each. Each color-state can be represented in a bit-field of width 4.
 * 
 * B = Blue    = 0000
 * G = Green   = 0001
 * O = Orange  = 0010
 * R = Red     = 0011
 * W = White   = 0100
 * Y = Yellow  = 0101
 * 
 * (4 cubies) * (4 bits per cubie) => 16-bit short.
 * 
 * vector<short> state(6) = {UU UU, LL LL, FF FF, RR RR, BB BB, DD DD}
 */

    // constructors
    PocketCube::PocketCube(): state{PocketCube::solved.state} { }
    PocketCube::PocketCube(short u, short l, short f, short r, short b, short d): state{u, l, f, r, b, d} { }
    PocketCube::PocketCube(const PocketCube& other): state{other.state} { }

    // faceColor (helper): 
    // "casts" four 4-bit color IDs into a single 16-bit bit field of colors,
    // whose order is specified by the CELL IDs constants.
    short PocketCube::faceColor(byte topLeft, byte topRight, byte botLeft, byte botRight) {
        return (short)(
               (topLeft  << 4 * TOP_LEFT ) |
               (topRight << 4 * TOP_RIGHT) |
               (botLeft  << 4 * BOT_LEFT ) |
               (botRight << 4 * BOT_RIGHT)
               );
    }

    // extractFaceColors (helper):
    // given a face bitset, the colors of the cubies are returned in logical order
    // ( 0 1 )
    // ( 2 3 )
    vector<byte> PocketCube::extractFaceColors(short face) {
        byte botRight = (0b0000'0000'0000'1111 & face) >> 0;
        byte botLeft =  (0b0000'0000'1111'0000 & face) >> 4;
        byte topLeft =  (0b0000'1111'0000'0000 & face) >> 8;
        byte topRight = (0b1111'0000'0000'0000 & face) >> 12;

        return {topLeft, topRight, botLeft, botRight};
    }

    const PocketCube PocketCube::solved = {PocketCube::faceColor(YELLOW, YELLOW, YELLOW, YELLOW),
                                           PocketCube::faceColor(RED, RED, RED, RED),
                                           PocketCube::faceColor(GREEN, GREEN, GREEN, GREEN),
                                           PocketCube::faceColor(ORANGE, ORANGE, ORANGE, ORANGE),
                                           PocketCube::faceColor(BLUE, BLUE, BLUE, BLUE),
                                           PocketCube::faceColor(WHITE, WHITE, WHITE, WHITE)};

    bool PocketCube::operator==(const PocketCube& other) const {
        return state == other.state;
    }

    bool PocketCube::operator!=(const PocketCube& other) const {
        return !(*this == other);
    }
    
    // copy-assignment
    PocketCube& PocketCube::operator=(const PocketCube& other) {
        state = other.state;
        return *this;
    }

    /* ~ ~ ~ ~ Debug ~ ~ ~ ~ */

    /*
     * ostream& operator<<(ostream& os, const PocketCube& pc) {
     *     char s[1000];
     *     sprintf(s,
     *                   "   %c%c      \n"
     *                   "   %c%c      \n"
     *                   "%c%c %c%c %c%c %c%c\n"
     *                   "%c%c %c%c %c%c %c%c\n"
     *                   "   %c%c      \n"
     *                   "   %c%c      ",
     *                 colorIdToChar[(pc.state[U] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[U] >> TOP_RIGHT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[U] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[U] >> BOT_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[L] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[L] >> TOP_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[F] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[F] >> TOP_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[R] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[R] >> TOP_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[B] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[B] >> TOP_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[L] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[L] >> BOT_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[F] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[F] >> BOT_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[R] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[R] >> BOT_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[B] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[B] >> BOT_RIGHT * 4) & 0b1111],
     *
     *                 colorIdToChar[(pc.state[D] >> TOP_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[D] >> TOP_RIGHT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[D] >> BOT_LEFT * 4) & 0b1111],
     *                 colorIdToChar[(pc.state[D] >> BOT_RIGHT * 4) & 0b1111]
     *             );
     *
     *     return os << s;
     * }
     */

    /* ~ ~ ~ ~ Hash Function ~ ~ ~ ~ */

    size_t PocketCube::hash::operator()(const PocketCube& c) const {
        uint128_t i = 0; // store k's state in a 128-bit integer
        for(const short& s : c.state) i = (i << 16) + s;
        return std::hash<uint128_t>{}(i); // return the has of that integer
    }

    /* ~ ~ ~ ~ Face Manipulation ~ ~ ~ ~ */
    // P indicates "prime" (same face, turned opposite direction (counterclockwise, from
    //                                      perspective of looking at 3d cube, directly at that face))

    // rotates the given faceID clockwise
    void PocketCube::rotateC(byte face) {
        state[face] = (short)((state[face] << 4) | (state[face] >> 12));
    }

    // rotates the given faceID counterclockwise
    void PocketCube::rotateCC(byte face) {
        state[face] = (short)((state[face] >> 4) | (state[face] << 12));
    }

    /*
    * XX
    * --
    * 
    * top = carry,
    * bottom = bottom (no change)
    * 
    * return old values of top
    */
    byte PocketCube::shiftUpperCells(byte face, byte carry) {
        byte oldTop = (byte) (state[face] >> 8);

        state[face] &= 0b0000'0000'1111'1111; // clear bits on top
        state[face] |= carry << 8;            // set top to carry

        return oldTop;
    }

    /*
    * --
    * XX
    * 
    * top = top, (no change)
    * bottom = carry
    * 
    * return old values of bottom
    */
    byte PocketCube::shiftLowerCells(byte face, byte carry) {
        byte oldBot = (byte) state[face];

        state[face] &= 0b1111'1111'0000'0000; // clear bits on bottom
        state[face] |= carry;                 // set bottom to carry

        return oldBot;
    }

    /*
    * X-
    * X-
    * 
    * left = carry,
    * right = right (no change)
    * 
    * return old values of left
    */
    byte PocketCube::shiftLeftCells(byte face, byte carry) {
        byte oldLeft = (byte) (state[face] >> 4);

        state[face] &= 0b1111'0000'0000'1111; // clear left
        state[face] |= carry << 4;            // set left to carry

        return oldLeft;
    }

    /*
    * -X
    * -X
    * 
    * right = carry,
    * left = left (no change)
    * 
    * return old values of right
    */
    byte PocketCube::shiftRightCells(byte face, byte carry) {
        byte oldRight = (byte) ((state[face] & 0b1111) | ((state[face] >> 12) << 4));

        state[face] &= 0b0000'1111'1111'0000; // clear right
        state[face] |= carry & 0b1111;        // set cell 0
        state[face] |= (carry & 0b1111'0000) << 8; // set cell 3

        return oldRight;
    }

    // invert (helper):
    // given   0bAAAA'BBBB,
    // returns 0bBBBB'AAAA
    byte invert(byte b) {
        return (byte) ((b >> 4) | (b << 4));
    }

    /* ~ ~ ~ ~ Turning Methods ~ ~ ~ ~ */

    // rotates U face clockwise (and bordering cells on L, F, R, and B)
    void PocketCube::turnU() {
        rotateC(U);

        shiftUpperCells(L,
        shiftUpperCells(F,
        shiftUpperCells(R,
        shiftUpperCells(B, (byte) (state[L] >> 8)))));
    }

    // rotates U face counterclockwise (and bordering cells on L, F, R, and B)
    void PocketCube::turnUP() {
        rotateCC(U);

        shiftUpperCells(B,
        shiftUpperCells(R,
        shiftUpperCells(F,
        shiftUpperCells(L, (byte) (state[B] >> 8)))));
    }

    void PocketCube::turnD() {
        rotateC(D);

        shiftLowerCells(B,
        shiftLowerCells(R,
        shiftLowerCells(F,
        shiftLowerCells(L, (byte) (state[B])))));
    }

    void PocketCube::turnDP() {
        rotateCC(D);

        shiftLowerCells(L,
        shiftLowerCells(F,
        shiftLowerCells(R,
        shiftLowerCells(B, (byte) (state[L])))));
    }

    void PocketCube::turnF() {
        rotateC(F);

        shiftLowerCells(U,
        invert(shiftRightCells(L,
        invert(shiftUpperCells(D,
        shiftLeftCells(R, (byte) state[U]))))));
    }

    void PocketCube::turnFP() {
        rotateCC(F);

        shiftLowerCells(U,
        shiftLeftCells(R,
        shiftUpperCells(D,
        invert(shiftRightCells(L, invert((byte) state[U]))))));
    }

    void PocketCube::turnB() {
        rotateC(B);

        shiftUpperCells(U,
        invert(shiftRightCells(R,
        invert(shiftLowerCells(D, 
        shiftLeftCells(L, (byte) (state[U] >> 8)))))));
    }

    void PocketCube::turnBP() {
        rotateCC(B);

        shiftLowerCells(D,
        invert(shiftRightCells(R,
        invert(shiftUpperCells(U,
        shiftLeftCells(L, (byte) state[D]))))));
    }

    void PocketCube::turnL() {
        rotateC(L);

        shiftLeftCells(D,
        shiftLeftCells(F,
        shiftLeftCells(U,
        invert(shiftRightCells(B, invert((byte) (state[D] >> 4)))))));
    }

    void PocketCube::turnLP() {
        rotateCC(L);

        shiftLeftCells(U,
        shiftLeftCells(F,
        shiftLeftCells(D,
        invert(shiftRightCells(B, invert((byte) (state[U] >> 4)))))));
    }

    void PocketCube::turnR() {
        rotateC(R);

        shiftLeftCells(B,
        invert(shiftRightCells(U,
        shiftRightCells(F,
        shiftRightCells(D, invert((byte) (state[B] >> 4)))))));
    }

    void PocketCube::turnRP() {
        rotateCC(R);

        shiftLeftCells(B,
        invert(shiftRightCells(D,
        shiftRightCells(F,
        shiftRightCells(U, invert((byte) (state[B] >> 4)))))));
    }
// }
