cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "-g")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(gabor_wavelet SHARED src/gabor_wavelet.cpp)
add_library(wavelet_converter SHARED src/wavelet_converter.cpp)
target_link_libraries(wavelet_converter gabor_wavelet)


add_executable(test_gabor_filter test/test_gabor_filter.cpp include/gtest/gtest-all.cc include/gtest/gtest_main.cc)
target_link_libraries(test_gabor_filter gabor_wavelet pthread)
add_executable(test_wavelet_converter test/test_wavelet_converter.cpp include/gtest/gtest-all.cc include/gtest/gtest_main.cc)
target_link_libraries(test_wavelet_converter wavelet_converter pthread)

add_executable(sample_walking example/sample_walking.cpp)
target_link_libraries(sample_walking wavelet_converter)
