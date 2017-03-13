//=========================================================
//
//A C++ program to implement the mRMR selection using mutual information
// written by Hanchuan Peng.
//
//Disclaimer: The author of program is Hanchuan Peng
//      at <penghanchuan@yahoo.com>.
//
//The CopyRight is reserved by the author.#include <math.h>

#include <omp.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "pbetai.cpp"
#include "sort2.cpp"

/* use S__LINE__ instead of __LINE__ */
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

using namespace std;

double calMutualInfo (std::vector< std::vector<int> > data, unsigned long v1,
  unsigned long v2);
double compute_mutualinfo (double *pab, long pabhei, long pabwid);
template < class T > void copyvecdata (T * srcdata, long len, int *desdata,
  int &nstate);
template < class T > double *compute_jointprob (T * img1, T * img2, long len,
  long maxstatenum, int &nstate1, int &nstate2);

enum Method { MID, MIQ };

void printPaperInfo()
{
  printf ("\n\n *** This program and the respective minimum Redundancy Maximum Relevance (mRMR) \n");
  printf( "     algorithm were developed by Hanchuan Peng <hanchuan.peng@gmail.com>for\n");
  printf( "     the paper \n");
  printf( "     \"Feature selection based on mutual information: criteria of \n");
  printf( "      max-dependency, max-relevance, and min-redundancy,\"\n");
  printf( "      Hanchuan Peng, Fuhui Long, and Chris Ding, \n");
  printf( "      IEEE Transactions on Pattern Analysis and Machine Intelligence,\n");
  printf( "      Vol. 27, No. 8, pp.1226-1238, 2005.\n\n");
  return;
}

template < class T > void copyvecdata (T * srcdata, long len, int *desdata,
    int &nstate) {
  if (!srcdata || !desdata)
    throw std::runtime_error("Line " S__LINE__
      ": NULL points in copyvecdata()!");
  //copy data
  int minn, maxx;
  if (srcdata[0] > 0) { maxx = minn = int (srcdata[0] + 0.5); }
  else { maxx = minn = int (srcdata[0] - 0.5); }
  int tmp;
  double tmp1;
  for (long i = 0; i < len; i++) {
    tmp1 = double (srcdata[i]);
    //round to integers
    tmp = (tmp1 > 0) ? (int) (tmp1 + 0.5) : (int) (tmp1 - 0.5);
    minn = (minn < tmp) ? minn : tmp;
    maxx = (maxx > tmp) ? maxx : tmp;
    desdata[i] = tmp;
  }
  //make the vector data begin from 0 (i.e. 1st state)
  for (long i = 0; i < len; i++) { desdata[i] -= minn; }
  //return the #state
  nstate = (maxx - minn + 1);
  return;
}

template < class T > double * compute_jointprob (T * img1, T * img2, long len,
    long maxstatenum, int &nstate1, int &nstate2) {
  //get and check size information
  long i, j;
  if (!img1 || !img2 || len < 0)
    throw std::runtime_error("Line " S__LINE__
      " At least one of the input vectors is invalid.");
  int b_returnprob = 1;
  //copy data into new INT type array (hence quantization) and then reange them begin from 0 (i.e. state1)
  int *vec1 = new int[len];
  int *vec2 = new int[len];
  if (!vec1 || !vec2)
      throw std::runtime_error("Line " S__LINE__
        " At least one of the input vectors is invalid.");
  int nrealstate1 = 0, nrealstate2 = 0;
  copyvecdata (img1, len, vec1, nrealstate1);
  copyvecdata (img2, len, vec2, nrealstate2);
  //update the #state when necessary
  nstate1 = (nstate1 < nrealstate1) ? nrealstate1 : nstate1;
  nstate2 = (nstate2 < nrealstate2) ? nrealstate2 : nstate2;
  //generate the joint-distribution table
  double *hab = new double[nstate1 * nstate2];
  double **hab2d = new double *[nstate2];
  if (!hab || !hab2d)
    throw std::runtime_error("Line " S__LINE__ " Fail to allocate memory.");
  for (j = 0; j < nstate2; j++) hab2d[j] = hab + (long) j *nstate1;
  for (i = 0; i < nstate1; i++)
    for (j = 0; j < nstate2; j++) { hab2d[j][i] = 0; }
  for (i = 0; i < len; i++) { hab2d[vec2[i]][vec1[i]] += 1; }
  //return the probabilities, otherwise return count numbers
  if (b_returnprob) {
    for (i = 0; i < nstate1; i++)
      for (j = 0; j < nstate2; j++) { hab2d[j][i] /= len; }
  }
  //finish
  if (hab2d) { delete[]hab2d; hab2d = 0; }
  if (vec1) { delete[]vec1; vec1 = 0; }
  if (vec2) { delete[]vec2; vec2 = 0; }
  return hab;
}

double compute_mutualinfo (double *pab, long pabhei, long pabwid) {
  //check if parameters are correct
  if (!pab)
    throw std::runtime_error("Line " S__LINE__
      " Got illegal parameter in compute_mutualinfo().");
  long i, j;
  double **pab2d = new double *[pabwid];
  for (j = 0; j < pabwid; j++) pab2d[j] = pab + (long) j *pabhei;
  double *p1 = 0, *p2 = 0;
  long p1len = 0, p2len = 0;
  int b_findmarginalprob = 1;
  //generate marginal probability arrays
  if (b_findmarginalprob != 0) {
    p1len = pabhei;
    p2len = pabwid;
    p1 = new double[p1len];
    p2 = new double[p2len];
    for (i = 0; i < p1len; i++) p1[i] = 0;
    for (j = 0; j < p2len; j++) p2[j] = 0;
    for (i = 0; i < p1len; i++)
      for (j = 0; j < p2len; j++) {
        p1[i] += pab2d[j][i]; p2[j] += pab2d[j][i];
      }
  }
  //calculate the mutual information
  double muInf = 0;
  muInf = 0.0;
  for (j = 0; j < pabwid; j++) {
    for (i = 0; i < pabhei; i++) {
      if (pab2d[j][i] != 0 && p1[i] != 0 && p2[j] != 0) {
        muInf += pab2d[j][i] * log (pab2d[j][i] / p1[i] / p2[j]);
      }
    }
  }
  muInf /= log (2);
  //free memory
  if (pab2d) { delete[]pab2d; }
  if (b_findmarginalprob != 0) {
      if (p1) { delete[]p1; }
      if (p2) { delete[]p2; }
  }
  return muInf;
}

double calMutualInfo (std::vector< std::vector<int> > data, unsigned long v1,
    unsigned long v2) {
  double mi = -1;		//initialized as an illegal value
  if (v1 >= data[0].size() || v2 >= data[0].size())
    throw std::runtime_error("Line " S__LINE__
      "Input variable indexes are invalid (out of range)");
  //copy data
  int *v1data = new int[data.size()];
  int *v2data = new int[data.size()];
  if (!v1data || !v2data)
    throw std::runtime_error("Line " S__LINE__ "Fail to allocate memory");
  #pragma omp parallel num_threads(4)
  #pragma omp parallel for
  for (unsigned long i = 0; i < data.size(); i++) {
      v1data[i] = data[i][v1];
      v2data[i] = data[i][v2];
  }
  //compute mutual info
  //always true for DataTable, which was discretized as three states
  long nstate = 3;
  int nstate1 = 0, nstate2 = 0;
  double *pab = compute_jointprob (v1data, v2data, data.size(), nstate,
    nstate1, nstate2);
  mi = compute_mutualinfo (pab, nstate1, nstate2);
  //free memory and return
  if (v1data) { delete[]v1data; v1data = 0; }
  if (v2data) { delete[]v2data; v2data = 0; }
  if (pab) { delete[]pab; pab = 0; }
  return mi;
}

std::vector<unsigned long> _select (std::vector< std::vector<int> > _data,
    std::vector<string> _names, unsigned long _nfeats, Method _method) {

  std::vector<unsigned long> _feats_ixs(_nfeats);

  unsigned long poolUseFeaLen = 5000;
  // there is a target variable (the first one), that is why must remove one
  if (poolUseFeaLen > _data[0].size() - 1) poolUseFeaLen = _data[0].size() - 1;
  if (_nfeats > poolUseFeaLen) _nfeats = poolUseFeaLen;
  //determine the pool
  float *mival = new float[_data[0].size()];
  float *poolInd = new float[_data[0].size()];
  char *poolIndMask = new char[_data[0].size()];
  if (!mival || !poolInd || !poolIndMask)
    throw std::runtime_error("Line " S__LINE__ "Fail to allocate memory");
  //the mival[0] is the entropy of target classification variable
  for (unsigned long i = 0; i < _data[0].size(); i++) {
      mival[i] = -calMutualInfo (_data, 0, i);//set as negative for sorting purpose
      poolInd[i] = i;
      poolIndMask[i] = 1;//initialized to be everything in pool would be considered
  }
  float *NR_mival = mival;	//vector_phc(1,_data->_nvars-1);
  float *NR_poolInd = poolInd;	//vector_phc(1,_data->_nvars-1);
  sort2 (_data[0].size() - 1, NR_mival, NR_poolInd);	// note that poolIndMask is not needed to be sorted, as everything in it is 1 up to this point
  mival[0] = -mival[0];
  printf ("\n*** MaxRel features ***\n");
  printf ("Order \t Fea \t Name \t Score\n");
  for (unsigned long i = 1; i < _data[0].size() - 1; i++) {
    mival[i] = -mival[i];
    if (i <= _nfeats)
      printf ("%ld \t %d \t %s \t %5.3f\n", i, int (poolInd[i]),
        _names[int (poolInd[i])].c_str(), mival[i]);
  }
  //mRMR selection
  unsigned long poolFeaIndMin = 1;
  unsigned long poolFeaIndMax = poolFeaIndMin + poolUseFeaLen - 1;
  _feats_ixs[0] = long (poolInd[1]);
  poolIndMask[_feats_ixs[0]] = 0;	//after selection, no longer consider this feature
  poolIndMask[0] = 0;		// of course the first one, which corresponds to the classification variable, should not be considered. Just set the mask as 0 for safety.
  printf ("\n*** mRMR features *** \n");
  printf ("Order \t Fea \t Name \t Score\n");
  printf ("%d \t %ld \t %s \t %5.3f\n", 1, _feats_ixs[0],
    _names[_feats_ixs[0]].c_str(), mival[1]);
  //the first one, _feats_ixs[0] has been determined already
  for (unsigned long k = 1; k < _nfeats; k++) {
    double relevanceVal, redundancyVal, tmpscore, selectscore;
    long selectind;
    int b_firstSelected = 0;
    for (unsigned long i = poolFeaIndMin; i <= poolFeaIndMax; i++) {
      if (poolIndMask[long (poolInd[i])] == 0) continue;		//skip this feature as it was selected already
      relevanceVal = calMutualInfo (_data, 0, long (poolInd[i]));	//actually no necessary to re-compute it, this value can be retrieved from mival vector
      redundancyVal = 0;
      for (unsigned long j = 0; j < k; j++)
        redundancyVal += calMutualInfo (_data, _feats_ixs[j], long (poolInd[i]));
      redundancyVal /= k;
      switch (_method) {
        case MID:
          tmpscore = relevanceVal - redundancyVal;
          break;
        case MIQ:
          tmpscore = relevanceVal / (redundancyVal + 0.0001);
          break;
        default:
          fprintf (stderr, "Undefined selection method=%d. Use MID instead.\n",
            _method);
          tmpscore = relevanceVal - redundancyVal;
      }
      if (b_firstSelected == 0) {
        selectscore = tmpscore;
        selectind = long (poolInd[i]);
        b_firstSelected = 1;
      } else {
        if (tmpscore > selectscore) {//update the best feature found and the score
          selectscore = tmpscore;
          selectind = long (poolInd[i]);
        }
      }
    }
    _feats_ixs[k] = selectind;
    poolIndMask[selectind] = 0;
    printf ("%ld \t %ld \t %s \t %5.3f\n", k + 1, _feats_ixs[k],
      _names[_feats_ixs[k]].c_str(), selectscore);
  }
  if (mival) { delete[]mival; mival = 0; }
  if (poolInd) { delete[]poolInd; poolInd = 0; }
  if (poolIndMask) { delete[]poolIndMask; poolIndMask = 0; }

  return _feats_ixs;
}

std::vector<unsigned long> _mRMR (std::vector< std::vector<int> > _data,
    std::vector<string> _names,
    int _method, unsigned long _nfeats) {

  if (_data.empty())
    throw std::runtime_error("Line " S__LINE__ ": _data matrix is empty");

  if (_names.empty())
    throw std::runtime_error("Line " S__LINE__ ": _names array is empty");

  printPaperInfo();

  return _select (_data, _names, _nfeats, Method (_method));
}
