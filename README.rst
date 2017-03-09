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

* Free software: MIT license


Installation
--------

.. code-block:: none

   pip install numpy Cython
   pip install git+https://github.com/fbrundu/pymrmr.git
   
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
   
   In [3]: df = pd.read_csv(pd.read_csv('test_colon_s3.csv')
   
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

