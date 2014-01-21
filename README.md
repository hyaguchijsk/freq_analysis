# freq_analysis

Author
======

* Hiroaki Yaguchi, JSK, The University of Tokyo

Abstract
========
Libraries for frequency analysis

Components
==========

WaveletConverter
----------------
Converting time-seriesed values to frequency space using Gabor filters
with various frequency


GaborFilter
-----------
Gabor filter with single frequency


Build
=====

```
cmake .
make
```

Example
=======

example/sample_walking.cpp
--------------------------

Analyze walking using magnitude of acceralation.

```
./bin/sample_walking ./example/right_leg.dat
```

This sample will write 3 output files;

- input_walking.dat : input accs magnitude
- result_walking.dat : results for each freqency
- plot_walking.dat : spectrogram data for gnuplot pm3d plot

Then you can visualize these data files using gnuplot.

```
plot "input_walking.dat" with line
plot "result_walking.dat" using 1:2 with line
splot "plot_walking.dat" with pm3d
```
