let mouseIsDown = false;

function mouseDown() {
    mouseIsDown = true;
}

function mouseUp() {
    mouseIsDown = false;
}

function mouseMove(e) {
    if(!mouseIsDown) return;

    xRot += e.movementX / 100;
    yRot += e.movementY / 100;

    xRot %= Math.PI * 2;
    yRot %= Math.PI * 2;
}

canvas.onmousemove = function(e) { mouseMove(e) };
document.onmousedown = function(e) { mouseDown() };
document.onmouseup = function(e) { mouseUp() };
