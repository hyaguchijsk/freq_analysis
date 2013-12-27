#ifndef FREQ_ANALYSIS_GABOR_FILTER_HPP_
#define FREQ_ANALYSIS_GABOR_FILTER_HPP_

#include <vector>
#include <string>

namespace freq_analysis {

class GaborFilter {
public:
  GaborFilter(float freq, float sigma);

  void filter();

private:
  float freq_;
  /// @param a = 1/freq
  float a_;
  /// @param b means offset
  float b_;
  float sigma_;
  float window_width_;

  float time_step_;
  uint32_t table_size_;
  std::vector<float> gabor_table_;
};

}  // namespace

#endif  // FREQ_ANALYSIS_GABOR_FILTER_HPP_
