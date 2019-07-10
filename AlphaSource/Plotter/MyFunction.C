#include "TROOT.h"
#include "TGraph.h"

struct MyFunction {
    MyFunction(TGraph &g) : fGraph(&g) {}
    double operator() (double *x, double *) { return fGraph->Eval(x[0]); }
    TGraph *fGraph;
};

struct MyStruct {
    Float_t  fDelta;
    Float_t  fDDelta;
    Int_t    fMin;
};
