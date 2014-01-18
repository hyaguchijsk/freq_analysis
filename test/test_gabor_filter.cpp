/// @file test_gabor_filter.cpp
/// @brief Test for GaborWavelet filtering
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>

#include <math.h>

#include "freq_analysis/gabor_wavelet.hpp"

#include "gtest/gtest.h"

using freq_analysis::GaborFilter;

class GaborFilterTest : public testing::Test {
 protected:
  void FilterTest() {
    float filter_freq = 20.0;
    GaborFilter gabor(filter_freq, 1.0, 0.005);

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
      if (fabs(filter_freq - freq) < 1e-5) {
        // if filter_freq == freq then result indicates peak
        EXPECT_TRUE(result > 10.0);
      } else {
        // else result = 0
        EXPECT_TRUE(result < 10.0);
      }
    }
  }
  
};

TEST_F(GaborFilterTest, PeakFrequency) {
  FilterTest();
}
