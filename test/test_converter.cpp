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
#include "freq_analysis/wavelet_converter.hpp"

using freq_analysis::WaveletConverter;

int main(int argc, char** argv) {
  // Create converter with
  // freq: 5.0 to 80.0 [Hz],
  //       increasing in a geometric progression with ratio = 2.0
  // max buffer length: 1024
  // center of t[s]: 0.0 because time series include negative values
  WaveletConverter conv(5.0, 2.0, 5, 1024, 0.0);

  // Generate time series and sin waves mized with 5.0[Hz] and 40.0[Hz] in
  float omega_5 = 2.0 * M_PI * 5.0;
  float omega_40 = 2.0 * M_PI * 40.0;
  
  for (float t = -0.5; t < 0.5; t += 0.003) {
    float value = sin(omega_5 * t) + cos(omega_40 * t);
    conv.AddValue(t, value);
  }

  std::vector<float> result;
  std::vector<float> freqs;

  conv.Convert(result);
  conv.Frequencies(freqs);

  for(size_t i = 0; i < result.size(); i++) {
    std::cout << "freq: " << freqs[i]
              << " , value = " << result[i] << std::endl;
  }
  
  return 0;
}

