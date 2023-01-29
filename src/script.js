const CANVAS_WIDTH = 300;
const CANVAS_HEIGHT = 300;

/* ~ ~ ~ ~ Setup / Runtime ~ ~ ~ ~ */

Module.onRuntimeInitialized = function() { main(); }; // run main once WASM is ready

function main() {
    _init();
    canvas.setAttribute("width", "" + CANVAS_WIDTH);
    canvas.setAttribute("height", "" + CANVAS_HEIGHT);
    cubeMeshSetup();
    updateCubeMesh();
    setInterval(update, 125);
}

/* ~ ~ ~ ~ Graphics ~ ~ ~ ~ */

const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

/*
 * Frame:
 * Represents a (CANVAS_WIDTH x CANVAS_HEIGHT) matrix of pixels that can be drawn onto the canvas.
 */
class Frame { 
    constructor(width, height, fillColor = [255, 255, 255, 255]) {
        this.width = width;
        this.height = height;
        this.imageData = ctx.createImageData(width, height);

        this.fill(fillColor);
    }

    fill(color) {
        for(let i = 0; i < this.width; i++) {
            for(let j = 0; j < this.height; j++) {
                this.drawPixel(i, j, color);
            }
        }
    }

    drawPixel(r, c, color) {
        r = Math.floor(r);
        c = Math.floor(c);

        this.imageData.data[r * this.width * 4 + c * 4 + 0] = color[0];
        this.imageData.data[r * this.width * 4 + c * 4 + 1] = color[1];
        this.imageData.data[r * this.width * 4 + c * 4 + 2] = color[2];
        this.imageData.data[r * this.width * 4 + c * 4 + 3] = color[3];
    }
}

function drawFrame() {
    _draw(); // call draw from WASM

    imageData = new ImageData(
        new Uint8ClampedArray(
            Module.HEAPU32.buffer,
            _getImageDataBuffer(),
            4 * CANVAS_WIDTH * CANVAS_HEIGHT
        ),
        CANVAS_WIDTH
    );

    ctx.putImageData(imageData, 0, 0);
}

let xRot = -0.15;
let yRot = -0.15;

function update() {
    _setRotation(xRot, yRot);
    drawFrame();
}

// Color IDs
const BLUE    = 0;
const GREEN   = 1;
const ORANGE  = 2;
const RED     = 3;
const WHITE   = 4;
const YELLOW  = 5;

function colorIdToString(colorId) {
    switch(colorId) {
        case RED:    return "#FF0000";
        case BLUE:   return "#0000FF";
        case GREEN:  return "#00FF00";
        case WHITE:  return "#FFFFFF";
        case ORANGE: return "#FFA500";
        case YELLOW: return "#FFFF00";
    }

    return "#000000"; // default = black;
}

// TURN IDs
const TURN_U  = 0;
const TURN_L  = 1;
const TURN_F  = 2;
const TURN_R  = 3;
const TURN_B  = 4;
const TURN_D  = 5;
const TURN_UP = 6;
const TURN_LP = 7;
const TURN_FP = 8;
const TURN_RP = 9;
const TURN_BP = 10;
const TURN_DP = 11;

let MOVE_NAMES = ["U", "L", "F", "R", "B", "D", "U'", "L'", "F'", "R'", "B'", "D'"];

// ORIENTATION IDs
const ORIENT_LEFT = 0;
const ORIENT_RIGHT = 1;
const ORIENT_UP = 2;
const ORIENT_DOWN = 3;

function turn(turnId) {
    _executeTurn(turnId);
    updateCubeMesh();
    updateSolution();
}

function orient(orientationId) {
    switch(orientationId) {
        case ORIENT_LEFT:
            turn(TURN_U);
            turn(TURN_DP);
            break;
        case ORIENT_RIGHT:
            turn(TURN_UP);
            turn(TURN_D);
            break;
        case ORIENT_UP:
            turn(TURN_R);
            turn(TURN_LP);
            break;
        case ORIENT_DOWN:
            turn(TURN_RP);
            turn(TURN_L);
            break;
    }
}

/* ~ ~ ~ ~ CUBE MESH ~ ~ ~ ~ */

const cubeMeshTable = document.getElementById("cube-mesh");
const TABLE_WIDTH = 8;
const TABLE_HEIGHT = 6;

function cubeMeshSetup() {
    for(let r = 0; r < TABLE_HEIGHT; r++) {
        let row = document.createElement("tr");
        for(let c = 0; c < TABLE_WIDTH; c++) {
            let col = document.createElement("td");
            row.appendChild(col);
        }
        cubeMeshTable.appendChild(row);
    }
}

const mesh = document.querySelector("table");

function updateCubeMesh() {
    let colorData = new Uint8ClampedArray(
                    Module.HEAPU32.buffer,
                    _getCubieColors(),
                    24);

    // U
    mesh.children[0].children[2].bgColor = colorIdToString(colorData[0]);
    mesh.children[0].children[3].bgColor = colorIdToString(colorData[1]);
    mesh.children[1].children[2].bgColor = colorIdToString(colorData[2]);
    mesh.children[1].children[3].bgColor = colorIdToString(colorData[3]);

    // L
    mesh.children[2].children[0].bgColor = colorIdToString(colorData[4]);
    mesh.children[2].children[1].bgColor = colorIdToString(colorData[5]);
    mesh.children[3].children[0].bgColor = colorIdToString(colorData[6]);
    mesh.children[3].children[1].bgColor = colorIdToString(colorData[7]);

    // F
    mesh.children[2].children[2].bgColor = colorIdToString(colorData[8]);
    mesh.children[2].children[3].bgColor = colorIdToString(colorData[9]);
    mesh.children[3].children[2].bgColor = colorIdToString(colorData[10]);
    mesh.children[3].children[3].bgColor = colorIdToString(colorData[11]);

    // R
    mesh.children[2].children[4].bgColor = colorIdToString(colorData[12]);
    mesh.children[2].children[5].bgColor = colorIdToString(colorData[13]);
    mesh.children[3].children[4].bgColor = colorIdToString(colorData[14]);
    mesh.children[3].children[5].bgColor = colorIdToString(colorData[15]);

    // B
    mesh.children[2].children[6].bgColor = colorIdToString(colorData[16]);
    mesh.children[2].children[7].bgColor = colorIdToString(colorData[17]);
    mesh.children[3].children[6].bgColor = colorIdToString(colorData[18]);
    mesh.children[3].children[7].bgColor = colorIdToString(colorData[19]);

    // D
    mesh.children[4].children[2].bgColor = colorIdToString(colorData[20]);
    mesh.children[4].children[3].bgColor = colorIdToString(colorData[21]);
    mesh.children[5].children[2].bgColor = colorIdToString(colorData[22]);
    mesh.children[5].children[3].bgColor = colorIdToString(colorData[23]);
}

const solveCheckbox = document.getElementById("solve-checkbox");
const solutionTextBox = document.getElementById("solution-box");

function updateSolution() {

    if(!solveCheckbox.checked) {
        solutionTextBox.innerHTML = " (Check \"Solve\" To Find Solution) ";
        return;
    }

    let numMoves = _solveCube(); // solve the cube and get the number of moves in the solution
    let solution = new Uint8ClampedArray(
                    Module.HEAPU32.buffer,
                    _getSolveBuffer(),
                    numMoves);

    let solutionString = numMoves == 0 ? "(Solved)" : "";

    for(let i = 0; i < numMoves; i++) {
        solutionString += MOVE_NAMES[solution[i]];
        if(i + 1 != numMoves) solutionString += ", ";
    }

    solutionTextBox.innerHTML = solutionString;
}

const SCRAMBLE_NUM_MOVES = 50;

function scrambleCube() {
    for(let i = 1; i <= SCRAMBLE_NUM_MOVES; i++) {
        let turnId = Math.floor(Math.random() * 12); // random int [0, 11] (random TurnId)
        if(i == SCRAMBLE_NUM_MOVES) turn(turnId);    // only update mesh and solution on last turn
        else _executeTurn(turnId);                   // otherwise call WASM turn function
    }
}
