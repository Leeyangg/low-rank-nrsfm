#include "mex.h"
#include "low-rank-nrsfm.hpp"

void checkDimensions(const mxArray* projections,
                     const mxArray* structure,
                     const mxArray* cameras,
                     const mxArray* bases,
                     const mxArray* coeffs) {
  bool valid = true;
  mwSize num_dims;
  const mwSize* dims;

  int F;
  int P;
  int K;

  num_dims = mxGetNumberOfDimensions(projections);
  if (num_dims != 3) {
    valid = false;
  } else {
    dims = mxGetDimensions(projections);
    if (dims[0] != 2) {
      valid = false;
    } else {
      P = dims[1];
      F = dims[2];
    }
  }
  if (!valid) {
    mexErrMsgTxt("projections should be 2 x P x F");
  }

  num_dims = mxGetNumberOfDimensions(structure);
  if (num_dims != 3) {
    valid = false;
  } else {
    dims = mxGetDimensions(structure);
    if (dims[0] != 3) {
      valid = false;
    } else if (dims[1] != P) {
      valid = false;
    } else if (dims[2] != F) {
      valid = false;
    }
  }
  if (!valid) {
    mexErrMsgTxt("structure should be 3 x P x F");
  }

  num_dims = mxGetNumberOfDimensions(cameras);
  if (num_dims != 2) {
    valid = false;
  } else {
    dims = mxGetDimensions(cameras);
    if (dims[0] != 4) {
      valid = false;
    } else if (dims[1] != F) {
      valid = false;
    }
  }
  if (!valid) {
    mexErrMsgTxt("cameras should be 4 x F");
  }

  num_dims = mxGetNumberOfDimensions(bases);
  if (num_dims != 3) {
    valid = false;
  } else {
    dims = mxGetDimensions(bases);
    if (dims[0] != 3) {
      valid = false;
    } else if (dims[2] != P) {
      valid = false;
    } else {
      K = dims[1];
    }
  }
  if (!valid) {
    mexErrMsgTxt("bases should be 3 x K x P");
  }

  num_dims = mxGetNumberOfDimensions(coeffs);
  if (num_dims != 2) {
    valid = false;
  } else {
    dims = mxGetDimensions(coeffs);
    if (dims[0] != K) {
      valid = false;
    } else if (dims[1] != F) {
      valid = false;
    }
  }
  if (!valid) {
    mexErrMsgTxt("coeffs should be K x F");
  }
}

void mexFunction(int nlhs, mxArray** plhs, int nrhs, const mxArray** prhs) {
  const int NARGIN = 8;
  const int NARGOUT = 3;

  if (nrhs < NARGIN) {
    mexErrMsgTxt("Not enough input arguments");
  }
  if (nrhs > NARGIN) {
    mexErrMsgTxt("Too many input arguments");
  }
  if (nlhs < NARGOUT) {
    mexErrMsgTxt("Not enough output arguments");
  }
  if (nlhs > NARGOUT) {
    mexErrMsgTxt("Too many output arguments");
  }

  const mxArray* projections = prhs[0];
  const mxArray* structure_init = prhs[1];
  const mxArray* cameras = prhs[2];
  const mxArray* bases_init = prhs[3];
  const mxArray* coeffs_init = prhs[4];
  double lambda = mxGetScalar(prhs[5]);
  int max_iter = mxGetScalar(prhs[6]);
  double tol = mxGetScalar(prhs[7]);

  checkDimensions(projections, structure_init, cameras, bases_init,
      coeffs_init);

  int F = mxGetDimensions(projections)[2];
  int P = mxGetDimensions(projections)[1];
  int K = mxGetDimensions(bases_init)[1];

  mxArray* structure = plhs[0] = mxDuplicateArray(structure_init);
  mxArray* bases = plhs[1] = mxDuplicateArray(bases_init);
  mxArray* coeffs = plhs[2] = mxDuplicateArray(coeffs_init);

  const double* W = mxGetPr(projections);
  const double* Q = mxGetPr(cameras);
  double* S = mxGetPr(structure);
  double* B = mxGetPr(bases);
  double* C = mxGetPr(coeffs);

  findApproxLowRankStructure(W, Q, S, B, C, lambda, F, P, K, max_iter, tol,
      true, false);
}
