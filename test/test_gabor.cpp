#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>

#include "freq_analysis/gabor_wavelet.hpp"

using freq_analysis::GaborFilter;

int main(int argc, char** argv) {
  GaborFilter gabor(20.0, 1.0);

  // gabor.status();
  
  for (float t = -0.5; t < 0.5; t += 0.003) {
    std::pair<float, float> value = gabor.approx_value(t);
    std::cout << t << " "
              << value.first << " "
              << value.second << std::endl;
  }
  
  return 0;
}
