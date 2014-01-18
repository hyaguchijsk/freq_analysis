/// @file test_wavelet_converter.cpp
/// @brief Test for WaveletConverter
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>

#include <math.h>

#include "freq_analysis/wavelet_converter.hpp"

#include "gtest/gtest.h"

using freq_analysis::WaveletConverter;

class WaveletConverterTest : public testing::Test {
 protected:
  void ConverterTest() {
    // Create converter with
    // freq: 5.0 to 80.0 [Hz],
    //       increasing in a geometric progression with ratio = 2.0
    // max buffer length: 1024
    // center of t[s]: 0.0 because time series include negative values
    WaveletConverter conv(5.0, 2.0, 5, 1024, 0.0);

    float in_freq = 20.0;
    float omega = 2.0 * M_PI * in_freq;
    for (float t = -0.5; t < 0.5; t += 0.003) {
      conv.AddValue(t, sin(omega * t));
    }

    std::vector<float> result;
    std::vector<float> freqs;

    conv.Convert(result);
    conv.Frequencies(freqs);

    ASSERT_EQ(result.size(), freqs.size());
    
    for(size_t i = 0; i < result.size(); i++) {
      if (fabs(freqs[i] - in_freq) < 1e-5) {
        // if filter_freq == freq then result indicates peak
        EXPECT_TRUE(result[i] > 10.0);
      } else {
        // else result = 0
        EXPECT_TRUE(result[i] < 10.0);
      }
    }
  }
  
};

TEST_F(WaveletConverterTest, PeakFrequency) {
  ConverterTest();
}
