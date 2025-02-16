#include <fstream>
#include <iostream>
#include <string>


#include "solver_mcica_lw.h"
#include "serdesl.h"

constexpr char ROOT[] = "/home/alex/backup-icon-model/experiments/exclaim_ape_R2B09/solver_mcica_lw/";

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
  cloud_type cloud;
  {
    std::ifstream data(std::string(ROOT) + "cloud.10.data");
    serde::deserialize(&cloud, data);
  }
  flux_type flux;
  {
    std::ifstream data(std::string(ROOT) + "flux.10.data");
    serde::deserialize(&flux, data);
  }
  
  printf("Here2\n");
  fflush(stdout);
     
  
  std::ifstream data1(std::string(ROOT) + "od_lw.10.data");
  auto [m,od_lw]=serde::read_array<double>(data1);
  
  std::ifstream data2(std::string(ROOT) + "od_lw_cloud.10.data");
  auto [m2,od_lw_cloud]=serde::read_array<double>(data2);
  
  std::ifstream data3(std::string(ROOT) + "planck_hl.10.data");
  auto [m3,planck_hl]=serde::read_array<double>(data3);
  

  //=(double *) malloc(403200*sizeof(double));
  
  // double* od_lw_cloud=(double *) malloc(46080*sizeof(double));
  
  // double* planck_hl=(double *) malloc(407680*sizeof(double));

  double ssa_lw[1];
  double g_lw[1];
  double ssa_lw_cloud[1];
  double g_lw_cloud[1];
  // double *lw_emission=(double *) malloc(12600*sizeof(double));
  // double *lw_albedo=(double *) malloc(12600*sizeof(double));
  // printf("Here2\n");
  // fflush(stdout);
  // {
  //   std::ifstream data(std::string(ROOT) + "od_lw.10.data");
  //   std::string line;
  //   std::getline(data, line);
  //   for (int i = 0; i < 403200; ++i) {
  //     std::getline(data, line);
  //     od_lw[i] = std::stod(line);
  //   }
  // }
  // printf("Here22\n");
  // fflush(stdout);
  // {
  //   std::ifstream data(std::string(ROOT) + "planck_hl.10.data");
  //   std::string line;
  //   std::getline(data, line);
  //   for (int i = 0; i < 407680; ++i) {
  //     std::getline(data, line);
  //     planck_hl[i] = std::stod(line);
  //   }
  // }

  // printf("Here222\n");
  // fflush(stdout);
  // {
  //   std::ifstream data(std::string(ROOT) + "od_lw_cloud.10.data");
  //   std::string line;
  //   std::getline(data, line);
  //   for (int i = 0; i < 46080; ++i) {
  //     std::getline(data, line);
  //     od_lw_cloud[i] = std::stod(line);
  //   }
  // }
  // printf("Here2222\n");
  // fflush(stdout);
  // {
  //   std::ifstream data(std::string(ROOT) + "lw_emission.10.data");
  //   std::string line;
  //   std::getline(data, line);
  //   for (int i = 0; i < 4480; ++i) {
  //     std::getline(data, line);
  //     lw_emission[i] = std::stod(line);
  //   }
  // }
  // printf("Here22222\n");
  // fflush(stdout);
  // {
  //   std::ifstream data(std::string(ROOT) + "lw_albedo.10.data");
  //   std::string line;
  //   std::getline(data, line);
  //   for (int i = 0; i < 4480; ++i) {
  //     std::getline(data, line);
  //     lw_albedo[i] = std::stod(line);
  //   }
  // }
  // printf("Here3\n");
  // fflush(stdout);
  
  std::ifstream data4(std::string(ROOT) + "lw_albedo.10.data");
  auto [m4,lw_albedo]=serde::read_array<double>(data4);
  std::ifstream data5(std::string(ROOT) + "lw_emission.10.data");
  auto [m5,lw_emission]=serde::read_array<double>(data5);
 printf("Here3\n");
   fflush(stdout);

  auto* h = __dace_init_solver_mcica_lw(lw_albedo,&cloud,&config,lw_emission,&flux,g_lw_cloud,g_lw,od_lw_cloud,od_lw,planck_hl,&single_level,ssa_lw_cloud,ssa_lw,32,1,90,90);
  
  printf("Here443\n");
  fflush(stdout);
  __program_solver_mcica_lw(h,lw_albedo,&cloud,&config,lw_emission,&flux,g_lw_cloud,g_lw,od_lw_cloud,od_lw,planck_hl,&single_level,ssa_lw_cloud,ssa_lw,32,1,90,90);
  
  printf("Here5\n");
  fflush(stdout);
  __dace_exit_solver_mcica_lw(h);
  
  flux_type flux2;
  {
    std::ifstream data(std::string(ROOT) + "flux.11.data");
    serde::deserialize(&flux2, data);
  }
  {
    std::ofstream data("flux.got");
    data << serde::serialize(&flux)<<std::endl;
  }
  {
    std::ofstream data("flux.want");
    data << serde::serialize(&flux2)<<std::endl;
  }

  

  return EXIT_SUCCESS;
}
