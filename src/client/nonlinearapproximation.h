#ifndef NONLINEARAPPROXIMATION_H
#define NONLINEARAPPROXIMATION_H

#include "gsl/gsl_multifit_nlin.h"

class NonLinearApproximation
{
public:
    NonLinearApproximation();

private:
    //type definition of solver
    const gsl_multifit_fdfsolver_type *T;
    //solver
    gsl_multifit_fdfsolver *s;
    int status;
    //n - number of points;
    const size_t n = N;
    //p - number of parameters of interpolation function. For exp interpolation p=3
    const size_t p = 3;
    //covar matrix. I don't know what it is :( But it's size is pxp
    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    //y and sigma - array of doubles. size is num of experimental points
    double y[N], sigma[N];
    struct data d = { n, y, sigma};
    gsl_multifit_function_fdf f;
    //starting parameters for fitting
    double x_init[3] = { 1.0, 0.0, 0.0 };
    gsl_vector_view x = gsl_vector_view_array (x_init, p);

//function mapping for f (  gsl_multifit_function_fdf )
    f.f = &expb_f;
    f.df = &expb_df;
    f.fdf = &expb_fdf;
    f.n = n;
    f.p = p;
    f.params = &d;
};

#endif // NONLINEARAPPROXIMATION_H
