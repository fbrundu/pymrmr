===============================
PymRMR
===============================


.. image:: https://img.shields.io/pypi/v/pymrmr.svg
        :target: https://pypi.python.org/pypi/pymrmr

.. image:: https://img.shields.io/travis/fbrundu/pymrmr.svg
        :target: https://travis-ci.org/fbrundu/pymrmr

.. image:: https://readthedocs.org/projects/pymrmr/badge/?version=latest
        :target: https://pymrmr.readthedocs.io/en/latest/?badge=latest
        :alt: Documentation Status

.. image:: https://pyup.io/repos/github/fbrundu/pymrmr/shield.svg
     :target: https://pyup.io/repos/github/fbrundu/pymrmr/
     :alt: Updates


Python3 binding to mRMR Feature Selection algorithm [1]

Original author: Hanchuan Peng (http://home.penglab.com/proj/mRMR/)

[1]: Hanchuan Peng, Fuhui Long, and Chris Ding, "Feature selection based on mutual information: criteria of max-dependency, max-relevance, and min-redundancy," IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 27, No. 8, pp.1226-1238, 2005.

The CPP code is subject to the original license (retrieved from http://home.penglab.com/proj/mRMR/FAQ_mrmr.htm):

The mRMR software packages can be downloaded and used, subject to the following conditions: Software and source code Copyright (C) 2000-2007 Written by Hanchuan Peng. These software packages are copyright under the following conditions: Permission to use, copy, and modify the software and their documentation is hereby granted to all academic and not-for-profit institutions without fee, provided that the above copyright notice and this permission notice appear in all copies of the software and related documentation and our publications (TPAMI05, JBCB05, CSB03, etc.) are appropriately cited. Permission to distribute the software or modified or extended versions thereof on a not-for-profit basis is explicitly granted, under the above conditions. However, the right to use this software by companies or other for profit organizations, or in conjunction with for profit activities, and the right to distribute the software or modified or extended versions thereof for profit are NOT granted except by prior arrangement and written consent of the copyright holders. For these purposes, downloads of the source code constitute "use" and downloads of this source code by for profit organizations and/or distribution to for profit institutions in explicitly prohibited without the prior consent of the copyright holders. Use of this source code constitutes an agreement not to criticize, in any way, the code-writing style of the author, including any statements regarding the extent of documentation and comments present. The software is provided "AS-IS" and without warranty of any kind, expressed, implied or otherwise, including without limitation, any warranty of merchantability or fitness for a particular purpose. In no event shall the authors be liable for any special, incidental, indirect or consequential damages of any kind, or any damages whatsoever resulting from loss of use, data or profits, whether or not advised of the possibility of damage, and on any theory of liability, arising out of or in connection with the use or performance of these software packages.

The Python wrapper is subject to MIT license.


Installation
--------

.. code-block:: none

   pip install numpy Cython
   pip install pymrmr

Usage
--------

pymRMR provides the single entry point method :code:`pymrmr.mRMR()`.

Data should be provided already discretised, as defined in the original paper [1]. This version of the algorithm does NOT provide discretisation, differently from the original C code.

This method requires 3 input parameters:

* First parameter is a pandas DataFrame (http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.html) containing the input dataset, discretised as defined in the original paper (for ref. see http://home.penglab.com/proj/mRMR/). The rows of the dataset are the different samples. The first column is the classification (target) variable for each sample. The remaining columns are the different variables (features) which may be selected by the algorithm. (see "Sample Data Sets" at http://home.penglab.com/proj/mRMR/ to download sample dataset to test this algorithm);
* Second parameter is a string which defines the internal Feature Selection method to use (defined in the original paper): possible values are "MIQ" or "MID";
* Third parameter is an integer which defines the number of features that should be selected by the algorithm.

The return value is a list containing the names of the selected features.



The following is an example of execution:

.. code-block:: python

   In [1]: import pandas as pd

   In [2]: import pymrmr

   In [3]: df = pd.read_csv('test_colon_s3.csv')

   In [4]: pymrmr.mRMR(df, 'MIQ', 10)


    *** This program and the respective minimum Redundancy Maximum Relevance (mRMR)
        algorithm were developed by Hanchuan Peng <hanchuan.peng@gmail.com>for
        the paper
        "Feature selection based on mutual information: criteria of
         max-dependency, max-relevance, and min-redundancy,"
         Hanchuan Peng, Fuhui Long, and Chris Ding,
         IEEE Transactions on Pattern Analysis and Machine Intelligence,
         Vol. 27, No. 8, pp.1226-1238, 2005.


    *** MaxRel features ***
    Order    Fea     Name    Score
    1        765     v765    0.375
    2        1423    v1423   0.337
    3        513     v513    0.321
    4        249     v249    0.309
    5        267     v267    0.304
    6        245     v245    0.304
    7        1582    v1582   0.280
    8        897     v897    0.269
    9        1771    v1771   0.269
    10       1772    v1772   0.269

    *** mRMR features ***
    Order    Fea     Name    Score
    1        765     v765    0.375
    2        1123    v1123   24.913
    3        1772    v1772   3.984
    4        286     v286    2.280
    5        467     v467    1.979
    6        377     v377    1.768
    7        513     v513    1.803
    8        1325    v1325   1.634
    9        1972    v1972   1.741
    10       1412    v1412   1.689
    Out[4]:
    ['v765',
     'v1123',
     'v1772',
     'v286',
     'v467',
     'v377',
     'v513',
     'v1325',
     'v1972',
     'v1412']


Credits
---------

This package was created with Cookiecutter_ and the `audreyr/cookiecutter-pypackage`_ project template.

.. _Cookiecutter: https://github.com/audreyr/cookiecutter
.. _`audreyr/cookiecutter-pypackage`: https://github.com/audreyr/cookiecutter-pypackage
