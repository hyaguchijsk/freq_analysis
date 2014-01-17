#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <list>

#include <math.h>

#include "freq_analysis/gabor_wavelet.hpp"

using freq_analysis::GaborFilter;

int main(int argc, char** argv) {
  GaborFilter gabor(20.0, 1.0);

  // gabor.status();

  for (float freq = 5.0; freq <= 80.0; freq *= 2.0) {
    std::list<float> time_list;
    std::list<float> value_list;
    
    float omega = 2.0 * M_PI * freq;
    float result = 0.0;
  
    for (float t = -0.5; t < 0.5; t += 0.003) {
      time_list.push_back(t);
      value_list.push_back(sin(omega * t));
    }

    result = gabor.filter(time_list, value_list, 0.0);
    std::cout << "freq: " << freq << " , value = " << result << std::endl;  
    }
  
  return 0;
}
