#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

#include "calc_surface_spectral.h"
#include "serde.h"

constexpr char ROOT[] = "/DATA/FILE/LOCATION/";

int main() {
  // Data loading.
  int istartcol, iendcol;
  {
    std::ifstream data(std::string(ROOT) + "istartcol.33.data");
    serde::deserialize(&istartcol, data);
  }
  {
    std::ifstream data(std::string(ROOT) + "iendcol.33.data");
    serde::deserialize(&iendcol, data);
  }
  config_type config;
  {
    std::ifstream data(std::string(ROOT) + "config_t0.33.data");
    serde::deserialize(&config, data);
  }
  flux_type flux;
  {
    std::ifstream data(std::string(ROOT) + "flux_t0.33.data");
    serde::deserialize(&flux, data);
  }
  flux_type want_flux;
  {
    std::ifstream data(std::string(ROOT) + "flux_t1.33.data");
    serde::deserialize(&want_flux, data);
  }

  // Actual call.
  {
    auto* h = __dace_init_calc_surface_spectral(&config, &flux, iendcol, istartcol);
    __program_calc_surface_spectral(h, &config, &flux, iendcol, istartcol);
    assert(__dace_exit_calc_surface_spectral(h) == 0);
  }

  // Result writing.
  {
    std::ofstream data("flux.got");
    data << serde::serialize(&flux) << std::endl;
  }
  {
    std::ofstream data("flux.want");
    data << serde::serialize(&want_flux) << std::endl;
  }
  return EXIT_SUCCESS;
}
