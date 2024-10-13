#include "common/Spline.h"
#include "linalg.h"
#include <array>

Spline SplineFactory::makeSpline(std::array<double, 4> coefficients, linalg::aliases::double3 start, linalg::aliases::double3 end) {
    return {coefficients, start, end};
}

Spline SplineFactory::makeSpline(linalg::aliases::double3 start, linalg::aliases::double3 end) {
    std::array<double, 4> coefficients;

    linalg::aliases::double4x4 A {
        {pow(start.x, 3), pow(start.x, 2), start.x, 1},
        {pow(end.x, 3), pow(end.x, 2), end.x, 1},
        {3 * pow(start.x, 2), 2 * start.x, 1, 0},
        {3 * pow(end.x, 2), 2 * end.x, 1, 0}
    };

    linalg::aliases::double4 b{start.y, end.y, tan(start.z), tan(end.z)};

    double det = linalg::determinant(A);
    for (int i = 0; i < coefficients.size(); i++) {
        linalg::aliases::double4x4 Ax = linalg::transpose(A);
        Ax[i] = b;
        Ax = linalg::transpose(Ax);
        coefficients[i] = linalg::determinant(Ax)/det;
    }
    return makeSpline(coefficients, start, end);
}

Spline SplineFactory::makeSpline(const Spline& start, linalg::aliases::double3 end){ 
    return makeSpline(start.end, end);
}