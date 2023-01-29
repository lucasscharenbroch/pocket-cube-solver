let mouseIsDown = false;

function mouseDown() {
    mouseIsDown = true;
}

function mouseUp() {
    mouseIsDown = false;
}

function mouseMove(e) {
    if(!mouseIsDown) return;

    xRot -= e.movementY / 100;
    yRot -= e.movementX / 100;

    xRot %= Math.PI * 2;
    yRot %= Math.PI * 2;
}

function doubleClick(e) {
    // reset rotation
    xRot = 0;
    yRot = 0;
}


canvas.onmousemove = function(e) { mouseMove(e) };
canvas.ondblclick = function(e) { doubleClick(e) };
document.onmousedown = function(e) { mouseDown() };
document.onmouseup = function(e) { mouseUp() };
