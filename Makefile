exported_functions := _getImageDataBuffer,_draw,_setRotation,_getCubieColors,_init,_executeTurn,_solveCube,_getSolveBuffer
source_files := src/main.cpp src/cube.cpp src/solving.cpp src/graphics.cpp

bin/wasm.js bin/wasm.wasm: $(source_files)
	em++ -O3 -o bin/wasm.js $(source_files) -sEXPORTED_FUNCTIONS=$(exported_functions) -sWASM=1 -sTOTAL_MEMORY=64MB
