#ifndef FREQ_ANALYSIS_GABOR_FILTER_HPP_
#define FREQ_ANALYSIS_GABOR_FILTER_HPP_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>

namespace freq_analysis {

class GaborFilter {
public:
  GaborFilter(float freq, float sigma);

  float filter(const std::list<float> time_list,
               const std::list<float> value_list,
               float time_offset);

  std::pair<float, float> approx_value(float time);
  
  void status();
  
private:
  float freq_;
  float sigma_;
  float window_width_;

  float time_step_;
  uint32_t table_size_;
  std::vector<float> gabor_table_r_;
  std::vector<float> gabor_table_i_;

  void init_table_();
};

}  // namespace

#endif  // FREQ_ANALYSIS_GABOR_FILTER_HPP_
