#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <iostream>
void callFevalgcd() {

    // Pass vector containing MATLAB data array scalar
    using namespace matlab::engine;

    // Start MATLAB engine synchronously
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

    // Create MATLAB data array factory
    matlab::data::ArrayFactory factory;

    // Pass vector containing 2 scalar args in vector    
    std::vector<matlab::data::Array> args({
        factory.createScalar<int16_t>(30),
        factory.createScalar<int16_t>(56) });

    // Call MATLAB function and return result
    matlab::data::TypedArray<int16_t> result = matlabPtr->feval(u"add(a,b)", args);
    int16_t v = result[0];
    std::cout << "Result: " << v << std::endl;
}