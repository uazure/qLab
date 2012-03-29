#ifndef NONLINEARAPPROXIMATION_H
#define NONLINEARAPPROXIMATION_H

#include <QVector>
#include <QPointF>
#include <QDebug>
#include "gsl/gsl_multifit_nlin.h"
#include "gsl/gsl_blas.h"

class NonLinearApproximation
{
public:
    NonLinearApproximation();
    //exponental base function
    //Y(x) = (b-a) exp (-x / c) +a
    static int expb_f(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f);

    static int expb_df(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_matrix * J);

    static int expb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_vector *f, gsl_matrix * J);

    int solve(const QVector<QPointF> &point);





private:

    //type definition of solver
    const gsl_multifit_fdfsolver_type *T;
    //solver
    gsl_multifit_fdfsolver *s;
    int status;
    void print_state (size_t iter,gsl_multifit_fdfsolver * s);

    /*
    //n - number of points;
    const size_t n = N;
    //p - number of parameters of interpolation function. For exp interpolation p=3
    const size_t p = 3;
    //covar matrix. I don't know what it is :( But it's size is pxp

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
    */
};

#endif // NONLINEARAPPROXIMATION_H
