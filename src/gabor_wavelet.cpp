#include "freq_analysis/gabor_wavelet.hpp"

#include <vector>
#include <string>

#include <math.h>

namespace freq_analysis {
  GaborFilter::Gaborfilter(float freq, float sigma) :
    freq_(freq), sigma_(sigma) {

    time_step_ = 0.02;  // 50hz

    a_ = 1.0 / freq_;
    window_width = a_ * sigma_ * sqrt(-2.0 * log(0.01));
    table_size_ = static_cast<uint32_t>(window_width_ / time_step_);

    // initialize gabor table
    float sigma2 = sigma_ * sigma_ * 2.0;
    gabor_table_.resize(table_size_ * 2 + 1);
    for (size_t i = 0; i < gabor_table_.size(); i++) {
      float t = (static_cast<float>(i) - static_cast<float>(table_size_)) * time_step_ * freq_;  // (t-b)/a
      gabor_table_[i] = 1.0 / sqrt(sigma2 * M_PI) * exp(-t * t / sigma2);
    }



  }


}  // namespace
