#include <fstream>
#include <iostream>
#include <string>


#include "solver_mcica_sw.h"
#include "serdess.h"

constexpr char ROOT[] = "/home/alex/backup-icon-model/experiments/exclaim_ape_R2B09/solver_mcica_sw/";

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
     
  
  std::ifstream data1(std::string(ROOT) + "od_sw.10.data");
  auto [m,od_sw]=serde::read_array<double>(data1);
  
  std::ifstream data2(std::string(ROOT) + "od_sw_cloud.10.data");
  auto [m2,od_sw_cloud]=serde::read_array<double>(data2);
  
  std::ifstream data3(std::string(ROOT) + "incoming_sw.10.data");
  auto [m3,incoming_sw]=serde::read_array<double>(data3);

  std::ifstream data11(std::string(ROOT) + "g_sw.10.data");
  auto [m11,g_sw]=serde::read_array<double>(data11);
  
  std::ifstream data12(std::string(ROOT) + "g_sw_cloud.10.data");
  auto [m12,g_sw_cloud]=serde::read_array<double>(data12);
  
  std::ifstream data13(std::string(ROOT) + "ssa_sw.10.data");
  auto [m13,ssa_sw]=serde::read_array<double>(data13);
  
  std::ifstream data14(std::string(ROOT) + "ssa_sw_cloud.10.data");
  auto [m14,ssa_sw_cloud]=serde::read_array<double>(data14);
  
  
  std::ifstream data4(std::string(ROOT) + "sw_albedo_diffuse.10.data");
  auto [m4,sw_albedo_diffuse]=serde::read_array<double>(data4);
  std::ifstream data5(std::string(ROOT) + "sw_albedo_direct.10.data");
  auto [m5,sw_albedo_direct]=serde::read_array<double>(data5);
 printf("Here3\n");
   fflush(stdout);

  auto* h = __dace_init_solver_mcica_sw(sw_albedo_diffuse,sw_albedo_direct,&cloud,&config,&flux,g_sw_cloud,g_sw,incoming_sw,od_sw_cloud,od_sw,&single_level,ssa_sw_cloud,ssa_sw,32,1,90,90);
  
  printf("Here443\n");
  fflush(stdout);
  __program_solver_mcica_sw(h,sw_albedo_diffuse,sw_albedo_direct,&cloud,&config,&flux,g_sw_cloud,g_sw,incoming_sw,od_sw_cloud,od_sw,&single_level,ssa_sw_cloud,ssa_sw,32,1,90,90);
  
  printf("Here5\n");
  fflush(stdout);
  __dace_exit_solver_mcica_sw(h);
  
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
