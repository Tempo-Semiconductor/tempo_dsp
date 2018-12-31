
![Tempo Logo](https://github.com/Tempo-Semiconductor/tempo_res/blob/master/tempo.png)

# Tempo DSP

This is a small library that can be used in conjunction with a Tempo DSP.

Much of the source from this library is derived from the *Cookbook formulae for
audio EQ biquad filter coefficients* by Robert Bristow-Johnson.

Currently only biquad filters are supported. Other effects will be added soon.

## Build and Install

### Prerequisites

`$ sudo apt-get install libpython3-dev python3-distutils`

### Build and Install C++ Library:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ sudo make install
    $ sudo ldconfig

### Build and Install Python Module (Requires C++ Lib):

    $ cd python_src/module_tempo_dsp
    $ python3 setup.py build
    $ sudo python3 setup.py install
