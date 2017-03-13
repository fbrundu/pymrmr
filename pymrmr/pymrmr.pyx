# distutils: language=c++

from libcpp.string cimport string
from libcpp.vector cimport vector
import numpy as np
cimport numpy as np
import pandas as pd

__version__ = '0.1.1'

cdef extern from 'mrmr.cpp':
  cdef vector[unsigned long] _mRMR(
    vector[vector[int]] _data,
    vector[string] _names,
    int _method,
    unsigned long _nfeats) except +

def mRMR(data, method, nfeats):
  cdef vector[vector[int]] _data = data.copy().values
  cdef vector[string] _names = [s.encode('utf-8') for s in data.columns]

  if method == 'MID': _method = 0
  elif method == 'MIQ': _method = 1
  else: raise ValueError('Only MID and MIQ method may be supplied')

  cdef vector[unsigned long] _feats = _mRMR(_data, _names, _method, nfeats)

  return list(data.columns[_feats])
