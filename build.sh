#!/bin/sh

# source /bin/emsdk/emsdk_env.sh
em++ -O3 -o wasm.js main.cpp cube.cpp solving.cpp graphics.cpp -sEXPORTED_FUNCTIONS=_getImageDataBuffer,_draw,_setRotation,_getCubieColors,_init,_executeTurn,_solveCube,_getSolveBuffer -sWASM=1 -sTOTAL_MEMORY=64MB
