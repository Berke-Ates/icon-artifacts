import dace
import shutil
import os
import filecmp

from dace.transformation.interstate import LoopToMap
from dace.transformation.passes.struct_to_container_group import StructToContainerGroups

# Mapping of SDFG -> headerfile
header_dict = {
    "add_aerosol_optics": "serdeae.h",
    "calc_surface_spectral": None,
    "cloud_optics_fn_438": "serdeco.h",
    "crop_cloud_fraction": "serdeccf.h",
    "gas_optics": "serdego.h",
    "get_albedos": "serde_get_albedos.h",
    "solver_mcica_lw": "serdesl.h",
    "solver_mcica_sw": "serdess.h",
}

# Mapping of SDFG -> main file
main_dict = {
    "add_aerosol_optics": "main_add_aerosol_optics.cc",
    "calc_surface_spectral": "main_calc_surface_spectral.cc",
    "cloud_optics_fn_438": "main_cloud_optics.cc",
    "crop_cloud_fraction": "main_crop_cloud_fraction.cc",
    "gas_optics": "main_gas_optics.cc",
    "get_albedos": "main_get_albedos.cpp",
    "solver_mcica_lw": "main_solver_mcica_lw.cc",
    "solver_mcica_sw": "main_solver_mcica_sw.cc",
}


# Choose the SDFG to run
path = "sdfgs/add_aerosol_optics_simplified_dbg22.sdfgz"  # ++ (segfault after S2CG)
# path = "sdfgs/calc_surface_spectral_simplified_dbg22.sdfgz" # x header missing
# path = "sdfgs/cloud_optics_fn_438_simplified_dbg22.sdfgz" # failed assertion
# path = "sdfgs/crop_cloud_fraction_simplified_dbg22.sdfgz" # ++++
# path = "sdfgs/gas_optics_simplified_dbg22.sdfgz" # x SDFG compilation killed
# path = "sdfgs/get_albedos_simplified_dbg22.sdfgz" # exit 1
# path = "sdfgs/solver_mcica_lw_simplified_dbg22.sdfgz" # unsorted double linked list corrupted
# path = "sdfgs/solver_mcica_sw_simplified_dbg22.sdfgz" # bad array new length

# Load SDFG
sdfg = dace.SDFG.from_file(path)


################################################################################
### Apply Optimizations
################################################################################

# TODO: Add Optimizations here for each SDFG

# Apply StructToContainerGroups
StructToContainerGroups().apply_pass(sdfg, {})

# Apply LoopToMap
sdfg.apply_transformations_repeated(LoopToMap)


################################################################################
### Compile the (optimized) SDFG with alterations
################################################################################

# compile the SDFG
sdfg.compile()

# get build location and dace location
build_loc = sdfg.build_folder
sdfg_name = sdfg.name
dace_include = os.path.dirname(dace.__file__) + "/runtime/include/"

# copy main_cpp_file to .dacecache/<name>/src/cpu/
main_name = main_dict[sdfg_name]
shutil.copy(f"main_cpp_files/{main_name}", f"{build_loc}/src/cpu/{main_name}")

# change the line "constexpr char ROOT[] ="
# to contain "<path to this script>/inputs/<name>/"
input_folder = os.path.dirname(os.path.realpath(__file__)) + "/inputs"
with open(f"{build_loc}/src/cpu/{main_name}", "r") as file:
    lines = file.readlines()
with open(f"{build_loc}/src/cpu/{main_name}", "w") as file:
    for line in lines:
        if line.startswith("constexpr char ROOT[] ="):
            file.write(f'constexpr char ROOT[] = "{input_folder}/{sdfg_name}/";\n')
        else:
            file.write(line)

# copy header to .dacecache/<name>/include/
header_name = header_dict[sdfg_name]
shutil.copy(f"headers/{header_name}", f"{build_loc}/include/{header_name}")

# compile c++ <SDFG cpp file> <main file> -I../../include -I/<pathtodace>/dace/runtime/include/ -std=c++17 -O0 -ggdb
exit_code = os.system(
    f"c++ {build_loc}/src/cpu/{sdfg_name}.cpp {build_loc}/src/cpu/{main_name} -I {build_loc}/include -I {dace_include} -std=c++17 -O0 -ggdb -o {sdfg_name}"
)

# check if compilation was successful
if exit_code != 0:
    print("Compilation failed")
    exit(1)


################################################################################
### Execute and diff .got and .want files
################################################################################

# execute the compiled program
exit_code = os.system(f"./{sdfg_name}")

# check if execution was successful
if exit_code != 0:
    print("Execution failed")
    exit(1)

# diff the .got and .want files
# Get list of .got and .want files
got_files = [f for f in os.listdir() if f.endswith(".got")]
want_files = [f.replace(".got", ".want") for f in got_files]

# Compare each .got file with its corresponding .want file
found_diff = False
for got, want in zip(got_files, want_files):
    if not filecmp.cmp(got, want, shallow=False):
        print(f"Numerical differences found between {got} and {want}")
        found_diff = True
    else:
        print(f"{got} and {want} are identical")

if not found_diff:
    print("No numerical differences found")


################################################################################
### Cleanup
################################################################################

# remove the compiled program
os.remove(sdfg_name)

# remove the .got files
for got in got_files:
    os.remove(got)

# remove the .want files
for want in want_files:
    os.remove(want)

# remove the .dacecache folder
shutil.rmtree(build_loc)
