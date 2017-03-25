#!/usr/bin/env python
# -*- coding: utf-8 -*-

from setuptools import setup, Extension
from Cython.Distutils import build_ext
import glob as gg
import numpy as np
import os
import sys


with open('README.rst') as readme_file:
  readme = readme_file.read()

with open('HISTORY.rst') as history_file:
  history = history_file.read()

requirements = [
  'numpy>=1.12.0',
]

test_requirements = [
  # TODO: put package test requirements here
]

if sys.platform == 'darwin':
  os.environ['CC'] = 'clang-omp'
  os.environ['CXX'] = 'clang-omp++'

setup(
  name='pymrmr',
  version='0.1.7',
  description="Python3 binding to mRMR Feature Selection algorithm",
  long_description=readme + '\n\n' + history,
  author="Francesco G. Brundu",
  author_email='francesco.brundu@gmail.com',
  url='https://github.com/fbrundu/pymrmr',
  packages=[
    'pymrmr',
  ],
  include_package_data=True,
  setup_requires=[
    'Cython>=0.25.2',
  ],
  install_requires=requirements,
  license="MIT license",
  zip_safe=False,
  keywords='pymrmr',
  classifiers=[
    'Development Status :: 2 - Pre-Alpha',
    'Intended Audience :: Developers',
    'License :: OSI Approved :: MIT License',
    'Natural Language :: English',
    'Programming Language :: Python :: 3.6',
  ],
  test_suite='tests',
  tests_require=test_requirements,
  cmdclass={'build_ext': build_ext},
  ext_modules=[
    Extension(
      'pymrmr',
      sources=[os.path.join('pymrmr', 'pymrmr.pyx')],
      language='c++',
      extra_compile_args=['-fopenmp', '-Ofast'],
      extra_link_args=['-fopenmp'])],
  include_dirs=[np.get_include()]
)
