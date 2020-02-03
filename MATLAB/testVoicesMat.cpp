#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "matrix.h"
#include <iostream>

int main() {
    MATFile *pmat;
	mxArray *mx;
	mxArray *Set;
	char *fieldnames[] = {"startPulses", "durations", "noteNumbers"};
	mwSize n;
    
    const char *file = "tempSong.mat";
    pmat = matOpen(file, "w");

	n = 1;
	Set = mxCreateStructMatrix(1, n, 3, fieldnames);

	mx = mxCreateDoubleMatrix(1,10,mxREAL);
	mxSetField(Set, 0, "startPulses", mx);
	//
	mx = mxCreateDoubleMatrix(1,10,mxREAL);
	mxSetField(Set, 0, "durations", mx);
	//
	mx = mxCreateDoubleMatrix(1,10,mxREAL);
	mxSetField(Set, 0, "noteNumbers", mx);
    
    matPutVariable(pmat, "theVoices", Set);
}