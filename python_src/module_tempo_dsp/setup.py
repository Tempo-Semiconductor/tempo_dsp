
from distutils.core import setup, Extension

import os

module = Extension('tempo_dsp',
                    include_dirs = ['/usr/local/include'],
                    libraries = ['tempo_dsp'],
                    library_dirs = ['/usr/local/lib'],
                    sources = ['tempo_dsp.cpp'])

setup (name = 'tempo_dsp',
        version = '0.1',
        description = 'Tempo DSP module',
        ext_modules = [module])
