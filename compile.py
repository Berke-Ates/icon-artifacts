import dace
import shutil
import os

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
    "get_albedos": "main_get_albedos.cc",
    "solver_mcica_lw": "main_solver_mcica_lw.cc",
    "solver_mcica_sw": "main_solver_mcica_sw.cc",
}


# Choose the SDFG to run
path = "sdfgs/add_aerosol_optics_simplified_dbg22.sdfgz"
# path = "sdfgs/calc_surface_spectral_simplified_dbg22.sdfgz"
# path = "sdfgs/cloud_optics_fn_438_simplified_dbg22.sdfgz"
# path = "sdfgs/crop_cloud_fraction_simplified_dbg22.sdfgz"
# path = "sdfgs/gas_optics_simplified_dbg22.sdfgz"
# path = "sdfgs/get_albedos_simplified_dbg22.sdfgz"
# path = "sdfgs/solver_mcica_lw_simplified_dbg22.sdfgz"
# path = "sdfgs/solver_mcica_sw_simplified_dbg22.sdfgz"

# Load SDFG
sdfg = dace.SDFG.from_file(path)


################################################################################
### Apply Optimizations
################################################################################

# TODO: Add Optimizations here for each SDFG


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
os.system(
    f"c++ {build_loc}/src/cpu/{sdfg_name}.cpp {build_loc}/src/cpu/{main_name} -I {build_loc}/include -I {dace_include} -std=c++17 -O0 -ggdb -o {sdfg_name}"
)
