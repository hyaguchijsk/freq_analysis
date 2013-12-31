#include "freq_analysis/gabor_wavelet.hpp"

#include <iostream>
#include <vector>
#include <string>

#include <math.h>

namespace freq_analysis {
GaborFilter::GaborFilter(float freq, float sigma) :
    freq_(freq), sigma_(sigma) {

  time_step_ = 0.01;  // 100hz

  init_table_();
}

void GaborFilter::init_table_() {
  window_width_ = (1.0 / freq_) * sigma_ * sqrt(-2.0 * log(0.01));
  table_size_ = static_cast<uint32_t>(window_width_ / time_step_);

  // initialize gabor table
  float sigma2 = sigma_ * sigma_ * 2.0;
  gabor_table_r_.resize(table_size_ * 2 + 1);
  gabor_table_i_.resize(table_size_ * 2 + 1);
  for (size_t i = 0; i < gabor_table_r_.size(); i++) {
    // t == 0 at gobor_table_[table_size_]
    float t = (static_cast<float>(i) - static_cast<float>(table_size_))
        * time_step_ * freq_;
    float gauss = 1.0 / sqrt(sigma2 * M_PI) * exp(-t * t / sigma2);
    float omega = 2.0 * M_PI * t;
    gabor_table_r_[i] = gauss * cos(omega);
    gabor_table_i_[i] = gauss * sin(omega);
  }
}

std::pair<float, float> GaborFilter::approx_value(float time) {
  int32_t idx = static_cast<int32_t>(time / time_step_
                                     + static_cast<float>(table_size_));
  float a = (time / time_step_ + static_cast<float>(table_size_))
      - static_cast<float>(idx);
  std::pair<float, float> value(0.0, 0.0);
  
  if (idx >= 0 && idx < gabor_table_r_.size() - 1) {
    float v0_r = gabor_table_r_[idx];
    float v1_r = gabor_table_r_[idx + 1];
    value.first = v0_r * (1.0 - a) + v1_r * a;

    float v0_i = gabor_table_i_[idx];
    float v1_i = gabor_table_i_[idx + 1];
    value.second = v0_i * (1.0 - a) + v1_i * a;
  }
  return value;
}



float GaborFilter::filter(const std::list<float> time_list,
                          const std::list<float> value_list,
                          float time_offset) {
  std::list<float>::const_iterator time_iter = time_list.begin();
  std::list<float>::const_iterator value_iter = value_list.begin();

  while (time_iter != time_list.end() && value_iter != value_list.end()) {
    float time = *time_iter - time_offset;
    float value = *value_iter;
    
    time_iter++;
    value_iter++;
  }
  
  return 0.0;
}

void GaborFilter::status() {
  std::cout << "freq: " << freq_ << std::endl;
  std::cout << "sigma: " << sigma_ << std::endl;
  std::cout << "window width: " << window_width_ << std::endl;
  std::cout << "time step: " << time_step_ << std::endl;

  std::cout << std::endl;
  
  std::cout << "table size: " << table_size_ << std::endl;
  std::cout << "gabor_table_r_.size(): " << gabor_table_r_.size() << std::endl;
  std::cout << "gabor_table_i_.size(): " << gabor_table_i_.size() << std::endl;
}


}  // namespace
