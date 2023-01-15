const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const CANVAS_WIDTH = 300;
const CANVAS_HEIGHT = 300;

const BLACK = [0, 0, 0, 255];
const RED = [255, 0, 0, 255];

canvas.setAttribute("width", "" + CANVAS_WIDTH);
canvas.setAttribute("height", "" + CANVAS_HEIGHT);

/*
 * Frame:
 * Represents a (CANVAS_WIDTH x CANVAS_HEIGHT) matrix of pixels that can be drawn onto the canvas.
 */
class Frame { 
    constructor(width, height, fillColor = [255, 255, 255, 255]) {
        this.width = width;
        this.height = height;
        this.imageData = ctx.createImageData(CANVAS_WIDTH, CANVAS_HEIGHT);

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

        this.imageData.data[r * CANVAS_WIDTH * 4 + c * 4 + 0] = color[0];
        this.imageData.data[r * CANVAS_WIDTH * 4 + c * 4 + 1] = color[1];
        this.imageData.data[r * CANVAS_WIDTH * 4 + c * 4 + 2] = color[2];
        this.imageData.data[r * CANVAS_WIDTH * 4 + c * 4 + 3] = color[3];
    }
}

function drawFrame() {
    let time = Date.now();
    _draw(); // call draw from webasm
    // console.log("time to cpp: " + (Date.now() - time));
    time = Date.now();
    imageData = new ImageData(
        new Uint8ClampedArray(
            Module.HEAPU32.buffer,
            _getImageDataBuffer(),
            4 * CANVAS_WIDTH * CANVAS_HEIGHT
        ),
        CANVAS_WIDTH
    );

    ctx.putImageData(imageData, 0, 0);
    // console.log("time to js: " + (Date.now() - time));
}

let xRot = 0.15;
let yRot = 0.15;

function update() {
    _setRotation(xRot, yRot);
    drawFrame();
}

// TODO test
function main() {
    setInterval(update, 125);
}

Module.onRuntimeInitialized = function() { main(); };
