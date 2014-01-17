/// @file test_gabor.cpp
/// @brief Test for wavelet generation of GaborWavelet
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>

#include "freq_analysis/gabor_wavelet.hpp"

using freq_analysis::GaborFilter;

int main(int argc, char** argv) {
  // Create filter with; freq = 20.0[Hz], sigma - 1.0, timestep = 0.005[s].
  GaborFilter gabor(20.0, 1.0, 0.005);

  // Show status
  // gabor.Status();

  // Read values of GaborFilter
  // this data can plot using gnuplot
  for (float t = -0.5; t < 0.5; t += 0.003) {
    std::pair<float, float> value = gabor.ApproxValue(t);
    std::cout << t << " "
              << value.first << " "
              << value.second << std::endl;
  }
  
  return 0;
}
