#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
test_pymrmr
----------------------------------

Tests for `pymrmr` module.
"""


import io
import pandas as pd
import requests as rq
import sys
import unittest

from pymrmr import mRMR


class TestPymrmr(unittest.TestCase):

  def setUp(self):

    data_url = 'http://home.penglab.com/proj/mRMR/test_colon_s3.csv'
    data = rq.get(data_url).content
    self.data = pd.read_csv(io.StringIO(data.decode('utf-8')))

    self.feats = {
      10: set(['v765', 'v1123', 'v1772', 'v286', 'v467', 'v377', 'v513',
        'v1325', 'v1972', 'v1412']),
      20: set(['v765', 'v1123', 'v1772', 'v286', 'v467', 'v377', 'v513',
        'v1325', 'v1972', 'v1412', 'v1381', 'v897', 'v1671', 'v1582', 'v1423',
        'v317', 'v249', 'v1473', 'v1346', 'v125'])}

  def tearDown(self):
    pass

  def test_mRMR(self):

    for n in self.feats.keys():
      feats = mRMR(self.data, 'MIQ', n)
      assert self.feats[n] == set(feats)
