/// @file gabor_wavelet.hpp
/// @brief Gabor wavelet filter
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#ifndef FREQ_ANALYSIS_GABOR_FILTER_HPP_
#define FREQ_ANALYSIS_GABOR_FILTER_HPP_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

namespace freq_analysis {

class GaborFilter {
public:
  GaborFilter(float freq, float sigma, float time_step);

  float Filter(const std::list<float> time_list,
               const std::list<float> value_list,
               float time_offset);

  std::pair<float, float> ApproxValue(float time);
  
  void Status();

  GaborFilter(const GaborFilter& obj);
  GaborFilter& operator=(const GaborFilter& obj);
  
private:
  float freq_;
  float sigma_;
  float window_width_;

  float time_step_;
  uint32_t table_size_;
  std::vector<float> gabor_table_r_;
  std::vector<float> gabor_table_i_;

  void InitTable_();
};

typedef boost::shared_ptr<GaborFilter> GaborFilterPtr;

}  // namespace

#endif  // FREQ_ANALYSIS_GABOR_FILTER_HPP_
