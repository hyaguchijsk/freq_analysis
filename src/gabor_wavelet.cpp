/// @file gabor_wavelet.cpp
/// @brief Gabor wavelet filter
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include "freq_analysis/gabor_wavelet.hpp"

#include <iostream>
#include <vector>
#include <string>

#include <math.h>

namespace freq_analysis {
/// @brief Constructor
/// @param freq Frequency [Hz]
/// @param sigma Sigma: variation of gaussian distribution
/// @param time_step Time step of value table [s]
GaborFilter::GaborFilter(float freq, float sigma, float time_step) :
    freq_(freq), sigma_(sigma), time_step_(time_step) {
  InitTable_();
}

/// @brief Copy constructor
/// @param obj Original instance
GaborFilter::GaborFilter(const GaborFilter& obj) {
  freq_ = obj.freq_;
  sigma_ = obj.sigma_;
  window_width_ = obj.window_width_;

  time_step_ = obj.time_step_;
  table_size_ = obj.table_size_;

  gabor_table_r_.assign(obj.gabor_table_r_.begin(),
                        obj.gabor_table_r_.end());
  gabor_table_i_.assign(obj.gabor_table_i_.begin(),
                        obj.gabor_table_i_.end());
}

/// @brief operator =
/// @param obj Original instance
GaborFilter& GaborFilter::operator=(const GaborFilter& obj) {
  freq_ = obj.freq_;
  sigma_ = obj.sigma_;
  window_width_ = obj.window_width_;

  time_step_ = obj.time_step_;
  table_size_ = obj.table_size_;

  gabor_table_r_.assign(obj.gabor_table_r_.begin(),
                        obj.gabor_table_r_.end());
  gabor_table_i_.assign(obj.gabor_table_i_.begin(),
                        obj.gabor_table_i_.end());
  
  return *this;
}

/// @brief Initialize value table, call at once from constructor
void GaborFilter::InitTable_() {
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

/// @brief Approximate value of Gabor wavelet
/// @return Complex number as std::pair
/// @param time Time[s]
std::pair<float, float> GaborFilter::ApproxValue(float time) {
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


/// @brief Filter time-seriesed values using gabor filter
/// @param time_list Time at for each values
/// @param value_list Values
/// @param time_offset Time offset[s] > 0.0, center of Gabor wavelet = -time_offset
float GaborFilter::Filter(const std::list<float> time_list,
                          const std::list<float> value_list,
                          float time_offset) {
  std::list<float>::const_iterator time_iter = time_list.begin();
  std::list<float>::const_iterator value_iter = value_list.begin();

  float result = 0.0;
  float res_re = 0.0;
  float res_im = 0.0;
  
  while (time_iter != time_list.end() && value_iter != value_list.end()) {
    float time = *time_iter - time_offset;
    float value = *value_iter;

    std::pair<float, float> gabor_value = ApproxValue(time);
    float re = gabor_value.first * value;
    float im = gabor_value.second * value;

    res_re += re;
    res_im += im;
    
    time_iter++;
    value_iter++;
  }

  result = sqrt(freq_) * sqrt (res_re * res_re + res_im * res_im);
  return result;
}

/// @brief Show params of GaborFilter
void GaborFilter::Status() {
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
