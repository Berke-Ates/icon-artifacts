import dace
import shutil
import os

from dace.transformation.interstate import LoopToMap

# load the sdfg
sdfg = dace.SDFG.from_file("sdfgs/solver_mcica_lw_simplified_dbg22.sdfgz")


# How many loops before?
loop_cnt = 0
for node, state in sdfg.all_nodes_recursive():
    if isinstance(state, dace.sdfg.state.LoopRegion):
        loop_cnt += 1

print(f"loops BEFORE loop2map: {loop_cnt}")

# apply loop2map transformation
num_apps=sdfg.apply_transformations_repeated(LoopToMap)
print(f"Applied {num_apps} LoopToMap transformations")

# How many loops after?
loop_cnt = 0
for node, state in sdfg.all_nodes_recursive():
    if isinstance(state, dace.sdfg.state.LoopRegion):
        loop_cnt += 1

print(f"loops AFTER loop2map: {loop_cnt}")

exit(0)

################################################################################
# Run the altered SDFG

# compile the sdfg
sdfg.compile()


# get build location and dace location
build_loc = sdfg.build_folder
sdfg_name = sdfg.name
dace_include = os.path.dirname(dace.__file__) + "/runtime/include/"

# copy main_cpp_files/main_solver_mcica_lw.cc to .dacecache/solver_mcica_lw/src/cpu/main_solver_mcica_lw.cc
shutil.copy(
    f"main_cpp_files/main_{sdfg_name}.cc", f"{build_loc}/src/cpu/main_{sdfg_name}.cc"
)

# change the line "constexpr char ROOT[] ="
# to contain "/home/xdb/icon-artifacts/inputs/solver_mcica_lw/"
with open(f"{build_loc}/src/cpu/main_{sdfg_name}.cc", "r") as file:
    lines = file.readlines()
with open(f"{build_loc}/src/cpu/main_{sdfg_name}.cc", "w") as file:
  for line in lines:
    if line.startswith('constexpr char ROOT[] ='):
      file.write(f'constexpr char ROOT[] = "/home/xdb/icon-artifacts/inputs/{sdfg_name}/";\n')
    else:
      file.write(line)
    

# copy headers/serdesl.h to .dacecache/solver_mcica_lw/include/serdesl.h
shutil.copy("headers/serdesl.h", f"{build_loc}/include/serdesl.h")

# compile c++ solver_mcica_lw.cpp main_solver_mcica_lw.cc -I../../include -I/<pathtodace>/dace/runtime/include/ -std=c++17 -O0 -ggdb
os.system(
    f"c++ {build_loc}/src/cpu/{sdfg_name}.cpp {build_loc}/src/cpu/main_{sdfg_name}.cc -I {build_loc}/include -I {dace_include} -std=c++17 -O0 -ggdb -o {sdfg_name}"
)
