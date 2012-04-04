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

    enum approximationMethod {
        methodLine,
        methodExp,
        methodExpLine,
        methodExpExpLine,
        methodExpExp,
        methodTailAvg
    };

    NonLinearApproximation();

    /** result - reference of vector<double>. It will be filled with
      0 - a
      1 - b
      2 - c
      .....
      */

    int solve(const QVector<QPointF> &point,int method,QString &log, QVector<double> &result);

    //exponental base function
    //Y(x) = (b-a) exp (-x / c) +a
    static int expb_f(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f);
    static int expb_df(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_matrix * J);
    static int expb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_vector *f, gsl_matrix * J);


    //exponental + line base function
    //Y(x) = (b-a) exp (-x/c) + a + d*x
    static int explineb_f(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f);
    static int explineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_matrix * J);
    static int explineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_vector *f, gsl_matrix * J);


    //exponental + exponental + line base function
    //Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + f + g*x
    static int expexplineb_f(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f);
    static int expexplineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_matrix * J);
    static int expexplineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_vector *f, gsl_matrix * J);


    //line base function
    //Y(x) = a*x+b
    static int lineb_f(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f);
    static int lineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_matrix * J);
    static int lineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr,gsl_vector *f, gsl_matrix * J);



    QVector<QPointF> getApproximationPoints() const {return interpolation;}
    void setInterpolationSteps(int steps) {interpolationSteps=steps;}
    int getInterpolationSteps() const {return interpolationSteps;}

    static double linearApproximation(const QVector<QPointF> & point);


private:

    //type definition of solver
    const gsl_multifit_fdfsolver_type *T;
    //solver
    gsl_multifit_fdfsolver *s;
    int status;
    void print_state (size_t iter,gsl_multifit_fdfsolver * s);
    int interpolationSteps;
    QVector<QPointF> interpolation;



};

#endif // NONLINEARAPPROXIMATION_H
