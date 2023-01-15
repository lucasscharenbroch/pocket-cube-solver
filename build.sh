#!/bin/sh

# source /bin/emsdk/emsdk_env.sh
em++ -O3 main.cpp cube.cpp solving.cpp graphics.cpp -sEXPORTED_FUNCTIONS=_getImageDataBuffer,_draw,_setRotation -sWASM=1
