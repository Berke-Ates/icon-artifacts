#include <fstream>
#include <iostream>
#include <filesystem>

#include "serde.h"

#include "get_albedos.h"

int main(int argc, char** argv)
{
  if(argc != 2)
    return 1;

  std::filesystem::path ROOT{argv[1]};

  int istartcol, iendcol;
  {
    std::ifstream data(ROOT / "istartcol.data");
    serde::deserialize(&istartcol, data);
  }

  {
    std::ifstream data(ROOT / "iendcol.data");
    serde::deserialize(&iendcol, data);
  }

  config_type config;
  {
    std::ifstream data(ROOT / "config.data");
    serde::deserialize(&config, data);
  }

  single_level_type single_level;
  {
    std::ifstream data(ROOT / "single_level.data");
    serde::deserialize(&single_level, data);
  }

  std::ifstream data_albedo_direct(ROOT / "sw_albedo_direct.data");
  auto [sw_alb_direct_meta, sw_albedo_direct] = serde::read_array<double>(data_albedo_direct);

  std::ifstream data_albedo_diffuse(ROOT / "sw_albedo_diffuse.data");
  auto [sw_alb_diff_meta, sw_albedo_diffuse] = serde::read_array<double>(data_albedo_diffuse);

  std::ifstream data_lw_albedo(ROOT / "lw_albedo.data");
  auto [lw_alb_meta, lw_albedo] = serde::read_array<double>(data_lw_albedo);

  auto* state = __dace_init_get_albedos(
      &config,
      lw_albedo,
      sw_albedo_diffuse,
      sw_albedo_direct,
      &single_level,
      1,
      iendcol,
      istartcol,
      iendcol,
      istartcol
  );

  __program_get_albedos(
      state,
      &config,
      lw_albedo,
      sw_albedo_diffuse,
      sw_albedo_direct,
      &single_level,
      1,
      iendcol,
      istartcol,
      iendcol,
      istartcol
  );

  __dace_exit_get_albedos(state);


  std::ifstream data_albedo_direct_after(ROOT / "sw_albedo_direct.after.data");
  auto [_, sw_albedo_direct_after] = serde::read_array<double>(data_albedo_direct_after);

  std::cerr << "Validate sw_albedo_direct " << sw_alb_direct_meta.size[0] << "x" << sw_alb_direct_meta.size[1] << std::endl;
  for(int i = 0; i < sw_alb_direct_meta.size[0]; ++i) {
    for(int j = 0; j < sw_alb_direct_meta.size[1]; ++j) {

      double original = sw_albedo_direct_after[sw_alb_direct_meta.size[1]*i + j];
      double ours = sw_albedo_direct[sw_alb_direct_meta.size[1]*i + j];

      if(fabs(ours - original) > 10e-9) {
        std::cerr << "Error! " << original << " " << ours << std::endl;
      }
    }
  }

  std::ifstream data_albedo_diffuse_after(ROOT / "sw_albedo_diffuse.after.data");
  auto [__, sw_albedo_diffuse_after] = serde::read_array<double>(data_albedo_diffuse_after);

  std::cerr << "Validate sw_albedo_diffuse " << sw_alb_diff_meta.size[0] << "x" << sw_alb_diff_meta.size[1] << std::endl;
  for(int i = 0; i < sw_alb_diff_meta.size[0]; ++i) {
    for(int j = 0; j < sw_alb_diff_meta.size[1]; ++j) {

      double original = sw_albedo_diffuse_after[sw_alb_diff_meta.size[1]*i + j];
      double ours = sw_albedo_diffuse[sw_alb_diff_meta.size[1]*i + j];

      if(fabs(ours - original) > 10e-9) {
        std::cerr << "Error! " << original << " " << ours << std::endl;
      }
    }
  }

  std::ifstream data_lw_albedo_after(ROOT / "lw_albedo.after.data");
  auto [___, lw_albedo_after] = serde::read_array<double>(data_lw_albedo_after);

  std::cerr << "Validate lw_albedo " << lw_alb_meta.size[0] << "x" << lw_alb_meta.size[1] << std::endl;
  for(int i = 0; i < lw_alb_meta.size[0]; ++i) {
    for(int j = 0; j < lw_alb_meta.size[1]; ++j) {

      double original = lw_albedo_after[lw_alb_meta.size[1]*i + j];
      double ours = lw_albedo[lw_alb_meta.size[1]*i + j];

      if(fabs(ours - original) > 10e-9) {
        std::cerr << "Error! " << original << " " << ours << std::endl;
      }
    }
  }

  //{
  //  std::ofstream data_albedo_direct("albedo_direct.out");
  //  for(int i = 0; i < alb_meta.size[0]; ++i) {
  //    for(int j = 0; j < alb_meta.size[1]; ++j) {
  //      data_albedo_direct << serde::serialize(sw_albedo_direct[alb_meta.size[1]*i + j]) << std::endl;
  //    }
  //  }

  //  std::ofstream data_albedo_diffuse("albedo_diffuse.out");
  //  data_albedo_diffuse << serde::serialize(sw_albedo_diffuse) << std::endl;

  //  std::ofstream data_lw_albedo("lw_albedo.out");
  //  data_lw_albedo << serde::serialize(lw_albedo) << std::endl;
  //}

  return 0;
}

