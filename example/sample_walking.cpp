/// @file sample_walking.cpp
/// @brief WaveletConverter sample with imu data of walking
/// @author Hiroaki Yaguchi
/// @author Copyright (c) 2014 Hiroaki Yaguchi, JSK, The University of Tokyo

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <string>

#include <math.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "freq_analysis/wavelet_converter.hpp"

using freq_analysis::WaveletConverter;
using freq_analysis::WaveletConverterPtr;

class SampleDataReader {
 public:
  SampleDataReader() {
    time_offset_ = 8.0;
    conv_ = WaveletConverterPtr(
        new WaveletConverter(0.25, sqrt(2.0), 10, 1024, time_offset_, 1.0));
  }

  bool ReadData(std::string& filename) {
    std::ifstream datafile(filename.c_str());
    if (!datafile) {
      std::cerr << "cannot open file: " << filename << std::endl;
      return false;
    }
    std::string str;
    while (getline(datafile, str)) {
      std::vector<std::string> split_str;
      boost::split(split_str, str, boost::is_any_of(" "));
      // data line is asuumed to be (time acc.x y z gyro.x y z)
      if (split_str.size() != 7) {
        continue;
      }
      // when line starts at # then it is a comment line
      if (split_str[0].at(0) == '#') {
        continue;
      }
      // reading
      time_list_.push_back(boost::lexical_cast<float>(split_str[0]));

      std::vector<float> acc;
      for (size_t i = 1; i < 4; i++) {
        acc.push_back(boost::lexical_cast<float>(split_str[i]));
      }
      acc_list_.push_back(acc);

      std::vector<float> gyro;
      for (size_t i = 4; i < 7; i++) {
        gyro.push_back(boost::lexical_cast<float>(split_str[i]));
      }
      gyro_list_.push_back(gyro);
    }
    return true;
  }

  void Proc(float start = 0.0, float end = -1.0) {
    std::cout << "Proc start" << std::endl;
    float indicate_time_step = 10.0;
    float next_time = indicate_time_step;
    if (end < 0.0) {
      end = time_list_[time_list_.size()-1];
    }

    float start_proc = start + time_offset_ * 2.0;
    for (size_t i = 0; i < time_list_.size(); i++) {
      float time = time_list_[i];
      if (time > next_time) {
        std::cout << "Proc at " << next_time << "[s]" << std::endl;
        next_time += indicate_time_step;
      }
      if (time < start) {
        continue;
      }

      float acc_x = acc_list_[i][0];
      float acc_y = acc_list_[i][1];
      float acc_z = acc_list_[i][2];
      float abs_acc =
          sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);
      conv_->AddValue(time, abs_acc);

      input_time_list_.push_back(time);
      input_list_.push_back(abs_acc);

      if (time > start_proc) {
        std::vector<float> result;
        conv_->Convert(result);
        result_time_list_.push_back(time - time_offset_);
        result_list_.push_back(result);
      }

      if (time > end) {
        break;
      }
    }
    std::cout << "Proc finish" << std::endl;
  }

  void WriteResult(std::string& filename) {
    std::ofstream resultfile(filename.c_str());
    std::vector<float> freqs;
    conv_->Frequencies(freqs);
    resultfile << "# time";
    for (size_t i = 0; i < freqs.size(); i++) {
      resultfile << " " << freqs[i];
    }
    resultfile << std::endl;

    for (size_t i = 0; i < result_time_list_.size(); i++) {
      resultfile << result_time_list_[i];
      for (size_t j = 0; j < result_list_[i].size(); j++) {
        resultfile << " " << result_list_[i][j];
      }
      resultfile << std::endl;
    }
  }

  void WriteInput(std::string& filename) {
    std::ofstream resultfile(filename.c_str());
    for (size_t i = 0; i < input_list_.size(); i++) {
      resultfile << input_time_list_[i] << " "
                 << input_list_[i] << std::endl;
    }
  }

  void WriteGnuplot3D(std::string& filename) {
    std::ofstream resultfile(filename.c_str());
    std::vector<float> freqs;
    conv_->Frequencies(freqs);

    for (size_t i = 0; i < freqs.size(); i++) {
      float freq = freqs[i];
      for (size_t j = 0; j < result_time_list_.size(); j++) {
        resultfile << result_time_list_[j] << " "
                   << freq << " "
                   << result_list_[j][i] << std::endl;
      }
      resultfile << std::endl;
    }
  }


 private:
  WaveletConverterPtr conv_;
  std::vector<float> time_list_;
  std::vector<std::vector<float> > acc_list_;
  std::vector<std::vector<float> > gyro_list_;
  float time_offset_;

  std::vector<float> input_time_list_;
  std::vector<float> input_list_;

  std::vector<float> result_time_list_;
  std::vector<std::vector<float> > result_list_;
};

int main(int argc, char** argv) {
  std::string filename;
  float start = 0.0;
  float end = -1.0;
  if (argc < 2) {
    std::cerr
        << "usage: sample_walking <data file> [start time] [end time]"
        << std::endl;
    return 1;
  } else if (argc >= 4) {
    try {
      start = boost::lexical_cast<float>(std::string(argv[2]));
      end = boost::lexical_cast<float>(std::string(argv[3]));
    } catch (boost::bad_lexical_cast& ex) {
      std::cerr << "invalid start or end time." << std::endl;
      return 1;
    }
  }
  filename = std::string(argv[1]);

  SampleDataReader sample;
  if (!sample.ReadData(filename)) {
    std::cerr << "invalid or missing data file: " << filename << std::endl;
    return 1;
  }
  sample.Proc(start, end);

  std::string resultfile("result_walking.dat");
  sample.WriteResult(resultfile);

  std::string targetfile("input_walking.dat");
  sample.WriteInput(targetfile);

  std::string plotfile("plot_walking.dat");
  sample.WriteGnuplot3D(plotfile);

  return 0;
}
