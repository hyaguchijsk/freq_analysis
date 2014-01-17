/// @file wavelet_converter.cpp
/// @brief Converter using gabor wavelets with various frequencies
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include "freq_analysis/wavelet_converter.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace freq_analysis {

WaveletConverter::WaveletConverter(float start, float step, size_t length,
                                   size_t max_buf_length, float center_t) :
    max_buf_length_(max_buf_length), center_t_(center_t) {
  float freq = start;
  for (size_t i = 0; i < length; i++) {
    GaborFilterPtr gabor(new GaborFilter(freq, 2.0, 0.005));
    filter_list_.push_back(gabor);
    freq_list_.push_back(freq);
    freq *= step;
  }
}

void WaveletConverter::AddValue(float time, float value) {
  if (time_list_.size() >= max_buf_length_ &&
      value_list_.size() >= max_buf_length_) {
    time_list_.pop_front();
    value_list_.pop_front();
  }
  time_list_.push_back(time);
  value_list_.push_back(value);
}

void WaveletConverter::ClearValue() {
  time_list_.clear();
  value_list_.clear();
}

/// @brief convert time series values into frequency space
/// @param result result for each filters
void WaveletConverter::Convert(std::vector<float>& result) {
  result.clear();
  float time = time_list_.back() - center_t_;
  for (size_t i = 0; i < filter_list_.size(); i++) {
    GaborFilterPtr& gabor = filter_list_[i];
    float value = gabor->Filter(time_list_, value_list_, time);
    result.push_back(value);
  }
}

/// @brief getter of frequencies for each filters
/// @param result result frequencies
void WaveletConverter::Frequencies(std::vector<float>& result) {
  result.assign(freq_list_.begin(), freq_list_.end());
}

}  // namespace