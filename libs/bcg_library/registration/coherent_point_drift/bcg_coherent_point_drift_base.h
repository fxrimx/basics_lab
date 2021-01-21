//
// Created by alex on 07.12.20.
//

#ifndef BCG_GRAPHICS_BCG_COHERENT_POINT_DRIFT_BASE_H
#define BCG_GRAPHICS_BCG_COHERENT_POINT_DRIFT_BASE_H

#include "math/bcg_linalg.h"

namespace bcg{

struct coherent_point_drift_base{
    bool optimized = false;
    bool initialized = false;
    bcg_scalar_t sigma_squared, omega = 0.5, N_P;
    size_t M, N, D;
    MatrixS<-1, -1> PX;
    VectorS<-1> P1, PT1, denominator;

    virtual void init(const MatrixS<-1, -1> &Y, const MatrixS<-1, -1> &X);

    virtual void maximization_step(const MatrixS<-1, -1> &Y, const MatrixS<-1, -1> &X);

    virtual void optimized_expectation_step(const MatrixS<-1, -1> &Y, const MatrixS<-1, -1> &X,
                                            size_t parallel_grain_size);

    virtual void optimized_maximization_step(const MatrixS<-1, -1> &Y, const MatrixS<-1, -1> &X);

    virtual VectorS<-1> transformed(const MatrixS<-1, -1> &Y, long idx);

    virtual MatrixS<-1, -1> transformed(const MatrixS<-1, -1> &Y);

    void operator()(const MatrixS<-1, -1> &Y, const MatrixS<-1, -1> &X, size_t parallel_grain_size = 1024);
};

}
#endif //BCG_GRAPHICS_BCG_COHERENT_POINT_DRIFT_BASE_H