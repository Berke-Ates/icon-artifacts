#include <fstream>
#include <iostream>
#include <string>


#include "gas_optics.h"
#include "serdego.h"

constexpr char ROOT[] = "/home/alex/backup-icon-model/experiments/exclaim_ape_R2B09/gas_optics/";

int main() {
  printf("Here\n");
  fflush(stdout);
  config_type config;
  {
    std::ifstream data(std::string(ROOT) + "config.10.data");
    serde::deserialize(&config, data);
  }
  single_level_type single_level;
  {
    std::ifstream data(std::string(ROOT) + "single_level.10.data");
    serde::deserialize(&single_level, data);
  }
  gas_type gas;
  {
    std::ifstream data(std::string(ROOT) + "gas.10.data");
    serde::deserialize(&gas, data);
  }
  thermodynamics_type thermodynamics;
  {
    std::ifstream data(std::string(ROOT) + "thermodynamics.10.data");
    serde::deserialize(&thermodynamics, data);
  }

  printf("Here2\n");
  fflush(stdout);
     
  
  std::ifstream data1(std::string(ROOT) + "lw_albedo.10.data");
  auto [m,lw_albedo]=serde::read_array<double>(data1);

  printf("Here3\n");
   fflush(stdout);
   double * od_lw=(double *) malloc(403200*sizeof(double));
  
   double* od_sw=(double *) malloc(322560*sizeof(double));
   
   double* ssa_sw=(double *) malloc(322560*sizeof(double));
 
  double* lw_emission=(double *) malloc(4480*sizeof(double));
  double* incoming_sw=(double *) malloc(3584*sizeof(double));
  
   double* planck_hl=(double *) malloc(407680*sizeof(double));
   //double *lw_albedo=(double *) malloc(4480*sizeof(double));
  //  config_type* config_var_457, gas_type* gas_var_460, double * __restrict__ incoming_sw_var_467, double * __restrict__ lw_albedo_var_461, 
  //  double * __restrict__ lw_emission_var_466, double * __restrict__ od_lw_var_462, double * __restrict__ od_sw_var_463, double * __restrict__ planck_hl_var_465,
  //   single_level_type* single_level_var_458, double * __restrict__ ssa_sw_var_464, thermodynamics_type* thermodynamics_var_459,
  //    int __f2dace_OPTIONAL_incoming_sw_var_467, int __f2dace_OPTIONAL_lw_albedo_var_461, int __f2dace_OPTIONAL_lw_emission_var_466, int __f2dace_OPTIONAL_planck_hl_var_465, 
  //    int iendcol_var_456, int istartcol_var_455, int layreffr_var_219, int layreffr_var_227, int layreffr_var_235, int layreffr_var_243, int layreffr_var_250,
  //    int layreffr_var_258, int layreffr_var_266, int layreffr_var_272, int layreffr_var_279, int layreffr_var_287, int layreffr_var_295, int layreffr_var_302, 
  //    int layreffr_var_307, int ncol_var_453,
  //     int nlev_var_454, int sym_iendcol_var_456, int sym_istartcol_var_455, int sym_ncol_var_453, int sym_nlev_var_454
  auto* h = __dace_init_gas_optics(&config,&gas,incoming_sw,lw_albedo,lw_emission,od_lw,od_sw,planck_hl,&single_level,ssa_sw,&thermodynamics,1,1,1,1,32,1,18,30,6,3,3,8,2,6,1,2,32,58,49,32,90,32,1,32,90);
  
  printf("Here443\n");
  fflush(stdout);
  __program_gas_optics(h,&config,&gas,incoming_sw,lw_albedo,lw_emission,od_lw,od_sw,planck_hl,&single_level,ssa_sw,&thermodynamics,1,1,1,1,32,1,18,30,6,3,3,8,2,6,1,2,32,58,49,32,90,32,1,32,90);
  
  printf("Here5\n");
  fflush(stdout);
  __dace_exit_gas_optics(h);
  
  printf("Here6\n");


  

  return EXIT_SUCCESS;
}
