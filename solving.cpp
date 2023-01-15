#include "solver.h"

/* ~ ~ ~ ~ Solving ~ ~ ~ ~ */

// turn IDs 
const byte TURN_U  = 0;
const byte TURN_L  = 1;
const byte TURN_F  = 2;
const byte TURN_R  = 3;
const byte TURN_B  = 4;
const byte TURN_D  = 5;
const byte TURN_UP = 6;
const byte TURN_LP = 7;
const byte TURN_FP = 8;
const byte TURN_RP = 9;
const byte TURN_BP = 10;
const byte TURN_DP = 11;

// inverse: given a turn ID, the inverse turn ID is returned.
byte inverse(const byte tid) {
    return (tid + 6) % 12;
}

// executes the turn function corresponding to the given turnID
PocketCube& turn(PocketCube& c, int turnId) {
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

// explore: given a node (a PocketCube state) and a parent-map, add all 
// unexplored (without-a-parent) neighboring nodes to the q, making the current
// state their new parent.
void explore(const PocketCube& node, unordered_set<PocketCube, PocketCube::hash>& vis, 
             unordered_map<PocketCube, int, PocketCube::hash>& parents,
             queue<PocketCube>& q) {

    vis.insert(node);

    for(int tid = 0; tid < 12; tid++) {        // for each turn-function,
        PocketCube neigh = node;               // execute the function;
        turn(neigh, tid);

        if(parents.find(neigh) != parents.end()) continue;   

                                               // if the resultant state hasn't been seen,
        q.push(neigh);                         // add it to the q
        parents[neigh] = tid;                  // and assign it a parent-move
    }
}


// solve: uses meet-in-the-middle-bfs to determine the shortest path from the 
// current state to the solved state, and returns the corresponding series of moves.
vector<byte> solve(const PocketCube& startNode) {
    PocketCube endNode; // solved state

    unordered_map<PocketCube, int, PocketCube::hash> parents1, parents2; // parent-move mappings for both trees
    unordered_set<PocketCube, PocketCube::hash> vis1, vis2; // sets of explored nodes for both trees
    queue<PocketCube> q1, q2; // queues of (potentially) unexplored nodes for both trees

    q1.push(startNode);
    q2.push(endNode);
    parents1[startNode] = -1;
    parents2[endNode] = -1;

    PocketCube currentNode; // temporary variable to hold the currently explored node;
                            // the trees' insersection will be stored here when the while-loop terminates

    while(true) {
        // explore one new state from the unsolved tree
        currentNode = q1.front(); q1.pop();
        if(parents2.find(currentNode) != parents2.end()) break;
        if(vis1.find(currentNode) == vis1.end()) explore(currentNode, vis1, parents1, q1);

        // explore one new state from the solved tree
        currentNode = q2.front(); q2.pop();
        if(parents1.find(currentNode) != parents1.end()) break;
        if(vis2.find(currentNode) == vis2.end()) explore(currentNode, vis2, parents2, q2);
    }

    // stich together the path by walking along the solved and unsolved trees
    vector<byte> solvedPath, unsolvedPath;
    PocketCube intersection = currentNode;

    while(parents1[currentNode] != -1) { // unsolved tree
        unsolvedPath.push_back(parents1[currentNode]);
        turn(currentNode, inverse(parents1[currentNode]));
    }

    currentNode = intersection;
    while(parents2[currentNode] != -1) { // solved tree
        solvedPath.push_back(inverse(parents2[currentNode])); // (add inversed turn, because solved tree is backwards)
        turn(currentNode, inverse(parents2[currentNode]));
    }

    // path = the entire solved path
    vector<byte> path(unsolvedPath.rbegin(), unsolvedPath.rend()); // path = { reverse(unsolvedPath} }
    path.insert(path.end(), solvedPath.begin(), solvedPath.end()); // path += solvedPath

    cout << " unsolved: " << endl;
    for(byte& move : unsolvedPath) {
        cout << (int) move << endl;
    }
    cout << " solved: " << endl;
    for(byte& move : solvedPath) {
        cout << (int) move << endl;
    }
    cout << endl;

    return path;
}
