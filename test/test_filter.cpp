/// @file test_filter.cpp
/// @brief Test for GaborWavelet.Filter()
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <list>

#include <math.h>

#include "freq_analysis/gabor_wavelet.hpp"

using freq_analysis::GaborFilter;

int main(int argc, char** argv) {
  // Create filter with; freq = 20.0[Hz], sigma - 1.0, timestep = 0.005[s].
  GaborFilter gabor(20.0, 1.0, 0.005);

  // Show status
  // gabor.Status();

  // Filter sin waves with various freqency
  for (float freq = 5.0; freq <= 80.0; freq *= 2.0) {
    std::list<float> time_list;
    std::list<float> value_list;
    
    float omega = 2.0 * M_PI * freq;
    float result = 0.0;
  
    for (float t = -0.5; t < 0.5; t += 0.003) {
      time_list.push_back(t);
      value_list.push_back(sin(omega * t));
    }

    // Filter using GaborFilter.Filter()
    result = gabor.Filter(time_list, value_list, 0.0);
    std::cout << "freq: " << freq << " , value = " << result << std::endl;  
    }
  
  return 0;
}
