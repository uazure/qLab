#include "nonlinearapproximation.h"

NonLinearApproximation::NonLinearApproximation()
{

}


int NonLinearApproximation::expb_f(const gsl_vector *approximationCoefficients,void * vectorPtr, gsl_vector *f) {
    //Y(x) = (b-a) exp (-x / c) +a

    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    //qDebug()<<"Initializing approximation function coefficients";
    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);


    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        double y=point->at(i).y();
        double Yi= (b-a) * exp (-x/c) + a;
        gsl_vector_set(f,i,Yi-y);
    }

    return GSL_SUCCESS;
}

int NonLinearApproximation::expb_df(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_matrix *J) {
    //Y(x) = (b-a) exp (-x / c) +a
    //converting void * to QVector<QPointF>
    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);

    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        double e = exp ( -x / c );
        //each cell in Jacobian matric J should be filled with derivative of the function for a, b or c as variable for j=0..2 respectively
        //check http://www.numberempire.com/derivatives.php for online derivative calculator. It has been used to calculate these functions:

        double dYda=e * (exp(x/c)-1);
        double dYdb=e;
        double dYdc=(b-a)*x*e/(c*c);
        //fill Jacobian matrix with calculated values
        gsl_matrix_set(J,i,0,dYda);
        gsl_matrix_set(J,i,1,dYdb);
        gsl_matrix_set(J,i,2,dYdc);
    }
    return GSL_SUCCESS;
}

int NonLinearApproximation::expb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f, gsl_matrix *J) {
    expb_f(approximationCoefficients,vectorPtr,f);
    expb_df(approximationCoefficients,vectorPtr,J);
    return GSL_SUCCESS;
}

int NonLinearApproximation::solve(const QVector<QPointF> &point) {

    size_t iter=0;
    int p = 3;
    int n = point.size();
    //x_init - initial values for a, b, c parameters for solver;
    double x_init[3] = {0,1,1};
    gsl_vector_view x=gsl_vector_view_array(x_init,3);
    gsl_matrix *covar = gsl_matrix_alloc (3, 3);

    //multifit function f and it's initialization
    gsl_multifit_function_fdf f;

    //initialize f, df and fdf for multifit function f
    f.f=&NonLinearApproximation::expb_f;
    f.df=&NonLinearApproximation::expb_df;
    f.fdf=&NonLinearApproximation::expb_fdf;
    //f.n - the number of functions, i.e. the number of components of the vector f. - from gsl doc
    //f.n - is the points count. - from gsl example
    f.n=point.size();
    //f.p - number of independent variables for appriximation function (a, b, c) - 3 independent coefficients
    f.p=3;
    f.params=(void *) &point;



    T=gsl_multifit_fdfsolver_lmsder;
    s=gsl_multifit_fdfsolver_alloc(T,f.n,f.p);
    gsl_multifit_fdfsolver_set(s,&f,&x.vector);
    print_state(iter,s);
    do {
        ++iter;
        status=gsl_multifit_fdfsolver_iterate(s);
        qDebug()<<"Status:"<<gsl_strerror(status);
        print_state(iter,s);
        if (status) break;
        status=gsl_multifit_test_delta(s->dx,s->x,1e-4,1e-4);
    }
    while (status==GSL_CONTINUE && iter<500);
    gsl_multifit_covar(s->J,0.0,covar);
#define FIT(i) gsl_vector_get(s->x,i)
#define ERR(i) sqrt(gsl_matrix_get(covar,i,i))
    double chi=gsl_blas_dnrm2(s->f);
    double dof=n-p;
    double c=GSL_MAX_DBL(1, chi/sqrt(dof));

    qDebug()<<"chisq/dof"<<pow(chi,2.0)/dof;

    qDebug()<<"a ="<<FIT(0)<<"+-"<<c*ERR(0);
    qDebug()<<"b ="<<FIT(1)<<"+-"<<c*ERR(1);
    qDebug()<<"c ="<<FIT(2)<<"+-"<<c*ERR(2);


    gsl_multifit_fdfsolver_free(s);
    gsl_matrix_free(covar);

    return 0;
}

void NonLinearApproximation::print_state(size_t iter, gsl_multifit_fdfsolver *s) {
    QString text="iter: %1\tx=%2\t%3\t%4\t|f(x)|=%5";
        qDebug()<<text.arg(QString::number(iter),
                        QString::number(gsl_vector_get(s->x,0)),
                        QString::number(gsl_vector_get(s->x,1)),
                        QString::number(gsl_vector_get(s->x,2)),
                        QString::number(gsl_blas_dnrm2(s->f)));
}


