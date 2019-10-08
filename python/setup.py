# !/usr/bin/env python

from setuptools import setup, Extension

setup(name="crystal",
      ext_modules=[
          Extension("crystal", ["pycrystal.cpp"], libraries=["boost_python"],
                    extra_compile_args=['-std=c++11'],

                    include_dirs=[],
                    library_dirs=[])

      ])
