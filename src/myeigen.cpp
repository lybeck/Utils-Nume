#include "nr.h"
#include "matutl02.cpp"
#include "myeigen.h"

void myeigen(Mat_DP a, Vec_CPLX_DP& w) {
    NR::balanc(a);
    NR::elmhes(a);
    NR::hqr(a, w);
}

