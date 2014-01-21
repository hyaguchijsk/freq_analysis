/// @file wavelet_converter.hpp
/// @brief Converter using gabor wavelets with various frequencies
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#ifndef FREQ_ANALYSIS_WAVELET_CONVERTER_
#define FREQ_ANALYSIS_WAVELET_CONVERTER_

#include <iostream>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "freq_analysis/gabor_wavelet.hpp"

namespace freq_analysis {

class WaveletConverter {
 public:
  WaveletConverter(float start, float step, size_t length,
                   size_t max_buf_length, float center_t,
                   float sigma = 2.0);

  void AddValue(float time, float value);
  void ClearValue();
  void Convert(std::vector<float>& result);
  void Frequencies(std::vector<float>& result);

 private:
  std::vector<GaborFilterPtr> filter_list_;
  std::vector<float> freq_list_;
  std::list<float> time_list_;
  std::list<float> value_list_;

  size_t max_buf_length_;
  float center_t_;
};

typedef boost::shared_ptr<WaveletConverter> WaveletConverterPtr;

}  // namespace

#endif  // FREQ_ANALYSIS_WAVELET_CONVERTER_
