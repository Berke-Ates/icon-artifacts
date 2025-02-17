Steps to numerically validate individual SDFGs:

1. Compile the SDFG
2. Place the main* file corresponding to the SDFG in the /src/cpu folder of your dace build folder
3. Place the serde* file corresponding to the SDFG in the /include/ folder of your dace build folder
4. Get the inputs for the SDFG and adapt the path in main* to the path where you saved the inputs
5. Compile, adding the include to dace.h such as ```c++ solver_mcica_sw.cpp main_solver_mcica_sw.cc -I../../include -I/<pathtodace>/dace/runtime/include/ -std=c++17 -O0 -ggdb```
6. Execute
7. Diff the *.want and *got files. (currently bit-accurate, we can do something more clever if necessary)
