# !/usr/bin/env python

from setuptools import setup, Extension

setup(name="crystal",
      ext_modules=[
          Extension("crystal", ["pycrystal.cpp"], libraries=["boost_python"],
                    include_dirs=[],
                    library_dirs=[])

      ])
