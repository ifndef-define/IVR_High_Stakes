#pragma once
#include "linalg.h"
#include <array>

struct Spline {
    std::array<double, 4> coefficients; 
    linalg::aliases::double3 start, end;
};

class SplineFactory {
private:
    static Spline makeSpline(std::array<double, 4> coefficients, linalg::aliases::double3 start, linalg::aliases::double3 end);
public:
    static Spline makeSpline(linalg::aliases::double3 start, linalg::aliases::double3 end);
    static Spline makeSpline(const Spline& start, linalg::aliases::double3 end);
};