#include "nonlinearapproximation.h"

NonLinearApproximation::NonLinearApproximation()
{
    interpolationSteps=50;
}


int NonLinearApproximation::solve(const QVector<QPointF> &point,int method, QString &log, QVector<double> &results) {

    //set inteproplation steps x5 of original point number:
    setInterpolationSteps(point.size()*5);

    int p; //number of parameters
    gsl_vector* xvector;
    gsl_matrix *covar;
    //multifit function
    gsl_multifit_function_fdf f;
    size_t iter=0;
    int n = point.size();
    QString formula;

    switch (method) {
    case 0:
        formula="Y(x) = a*x+b";
        p=2;
        xvector=gsl_vector_alloc(p);
        //setting initial values for fitting a, b and c parameters
        gsl_vector_set(xvector,0,0); //a
        gsl_vector_set(xvector,1,1); //b

        //initialize f, df and fdf for multifit function f
        f.f=&NonLinearApproximation::lineb_f;
        f.df=&NonLinearApproximation::lineb_df;
        f.fdf=&NonLinearApproximation::lineb_fdf;

        break;
    case 1: //exp fitting
        formula="Y(x) = (b-a) exp (-x / c) +a";
        p = 3;
        xvector=gsl_vector_alloc(p);
        //setting initial values for fitting a, b and c parameters
        gsl_vector_set(xvector,0,0); //a
        gsl_vector_set(xvector,1,1); //b
        gsl_vector_set(xvector,2,1); //c

        //initialize f, df and fdf for multifit function f
        f.f=&NonLinearApproximation::expb_f;
        f.df=&NonLinearApproximation::expb_df;
        f.fdf=&NonLinearApproximation::expb_fdf;

        break;
    case 2: //expline
        formula="Y(x) = (b-a) exp (-x / c) +a +d*x";
        p=4;
        xvector=gsl_vector_alloc(p);
        //setting initial values for fitting a, b and c parameters
        gsl_vector_set(xvector,0,0); //a
        gsl_vector_set(xvector,1,1); //b
        gsl_vector_set(xvector,2,1); //c
        gsl_vector_set(xvector,3,0); //d

        //initialize f, df and fdf for multifit function f
        f.f=&NonLinearApproximation::explineb_f;
        f.df=&NonLinearApproximation::explineb_df;
        f.fdf=&NonLinearApproximation::explineb_fdf;
        break;

    case 4:
        formula="Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x";
        p=6;
        xvector=gsl_vector_alloc(p);
        //setting initial values for fitting a, b and c parameters
        gsl_vector_set(xvector,0,1); //a
        gsl_vector_set(xvector,1,1); //b
        gsl_vector_set(xvector,2,1); //c
        gsl_vector_set(xvector,3,1); //d
        gsl_vector_set(xvector,4,0); //e
        gsl_vector_set(xvector,5,0); //f

        //initialize f, df and fdf for multifit function f
        f.f=&NonLinearApproximation::expexplineb_f;
        f.df=&NonLinearApproximation::expexplineb_df;
        f.fdf=&NonLinearApproximation::expexplineb_fdf;
        break;

    }

    //f.n - the number of functions, i.e. the number of components of the vector f. - from gsl doc
    //f.n - is the points count. - from gsl example
    f.n=point.size();
    //f.p - number of independent variables for appriximation function (a, b, c) - 3 independent coefficients
    f.p=p;
    //init covar matrix (i don't know what it is)
    covar = gsl_matrix_alloc (p, p);
    f.params=(void *) &point;

    if (point.size()<p) {
        qWarning()<<"Not enough selected points to use this approximation method";
        log="Status: insufficient data points, n < p\nSelect more points or use other approximation method";
        gsl_vector_free(xvector);
        gsl_matrix_free(covar);
        return GSL_EINVAL;
    }

//init solver
    T=gsl_multifit_fdfsolver_lmsder;
    s=gsl_multifit_fdfsolver_alloc(T,f.n,f.p);
    gsl_multifit_fdfsolver_set(s,&f,xvector);
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
    #define FIT(i) QString::number(gsl_vector_get(s->x,i))
    #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))
    double chi=gsl_blas_dnrm2(s->f);
    double dof=n-p;
    double c=GSL_MAX_DBL(1, chi/sqrt(dof));

    qDebug()<<"chisq/dof"<<pow(chi,2.0)/dof;
    log="Status: ";
    log+=gsl_strerror(status);
    log+="\nIterations made:";
    log+=QString::number(iter)+"\n";
    log+="chisq/dof "+QString::number(pow(chi,2.0)/dof)+"\n\n";
    log+="Solution of "+formula+"\n";
    for (int i=0;i<p;++i) {
        QChar letter(97+i); //97 is ascii code for letter 'a'
        log+=letter;
        log+=" = "+FIT(i)+"+-"+QString::number(c*ERR(i))+"\n";
        qDebug()<<letter<<" ="<<FIT(i)<<"+-"<<c*ERR(i);
    }

    double xStart=point.first().x();
    double xEnd=point.last().x();

    //report calculated coefficients for later calculation of dT and dF:
    results.clear();

    for (int i=0;i<p;++p) {
        results.append(gsl_vector_get(s->x,p));
    }
//    switch (method) {
//    case 0:
//        //formula="Y(x) = a*x+b";
//        results.append(gsl_vector_get(s->x,1));
//        break;
//    case 1:
//        //formula="Y(x) = (b-a) exp (-x / c) +a";
//        results.append(gsl_vector_get(s->x,1));
//        results.append(gsl_vector_get(s->x,2));
//        break;
//    case 2:
//        //formula="Y(x) = (b-a) exp (-x / c) +a +d*x";
//        results.append(gsl_vector_get(s->x,1));
//        results.append(gsl_vector_get(s->x,2));
//        break;
//    case 4:
//        //formula="Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x";
//        results.append(gsl_vector_get(s->x,1));
//        results.append(gsl_vector_get(s->x,2));
//        break;
//    }

    //generate approximation curve points

    for (int i=0;i<interpolationSteps;++i) {
        double x=xStart+(xEnd-xStart)*i/interpolationSteps;
        double a,b,c,d,e,f,y;

        switch (method) {
        case 0:
            //formula="Y(x) = a*x+b";
            a=gsl_vector_get(s->x,0);
            b=gsl_vector_get(s->x,1);
            y=a*x+b;
            break;
        case 1:
            //formula="Y(x) = (b-a) exp (-x / c) +a";
            a=gsl_vector_get(s->x,0);
            b=gsl_vector_get(s->x,1);
            c=gsl_vector_get(s->x,2);
            y=(b-a)*exp(-x/c)+a;
            break;
        case 2:
            //formula="Y(x) = (b-a) exp (-x / c) +a +d*x";
            a=gsl_vector_get(s->x,0);
            b=gsl_vector_get(s->x,1);
            c=gsl_vector_get(s->x,2);
            d=gsl_vector_get(s->x,3);
            y=(b-a)*exp(-x/c)+a+d*x;
            break;
        case 4:
            //formula="Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x";
            a=gsl_vector_get(s->x,0);
            b=gsl_vector_get(s->x,1);
            c=gsl_vector_get(s->x,2);
            d=gsl_vector_get(s->x,3);
            e=gsl_vector_get(s->x,4);
            f=gsl_vector_get(s->x,5);
            y=a*(1-exp(-x/c))+b*(exp(-x/d)-1)+e+f*x;
            break;
        }
        QPointF interpolationPoint(x,y);
        interpolation.append(interpolationPoint);
    }


    gsl_vector_free(xvector);
    gsl_multifit_fdfsolver_free(s);
    gsl_matrix_free(covar);

    return status;
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


void NonLinearApproximation::print_state(size_t iter, gsl_multifit_fdfsolver *s) {
    /*QString text="iter: %1\tx=%2\t%3\t%4\t|f(x)|=%5";
        qDebug()<<text.arg(QString::number(iter),
                        QString::number(gsl_vector_get(s->x,0)),
                        QString::number(gsl_vector_get(s->x,1)),
                        QString::number(gsl_vector_get(s->x,2)),
                        QString::number(gsl_blas_dnrm2(s->f)));
        */
      /*  QString text="iter: %1\tx=%2\t%3\t%4\t";
            qDebug()<<text.arg(QString::number(iter),
                            QString::number(gsl_vector_get(s->x,0)),
                            QString::number(gsl_vector_get(s->x,1)),
                            QString::number(gsl_vector_get(s->x,2)));*/
}

int NonLinearApproximation::explineb_f(const gsl_vector *approximationCoefficients,void * vectorPtr, gsl_vector *f) {
    //Y(x) = (b-a) exp (-x/c) + a + d*x

    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    //qDebug()<<"Initializing approximation function coefficients";
    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);
    double d=gsl_vector_get(approximationCoefficients,3);


    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        double y=point->at(i).y();
        double Yi= (b-a) * exp (-x/c) + a + d*x;
        gsl_vector_set(f,i,Yi-y);
    }

    return GSL_SUCCESS;
}


int NonLinearApproximation::explineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_matrix *J) {
    //Y(x) = (b-a) exp (-x/c) + a + d*x
    //converting void * to QVector<QPointF>
    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);
    //double d=gsl_vector_get(approximationCoefficients,3); - not used in jacobian matrix calculation because it's constant for all derivatives

    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        double e = exp ( -x / c );
        //each cell in Jacobian matric J should be filled with derivative of the function for a, b or c as variable for j=0..2 respectively
        //check http://www.numberempire.com/derivatives.php for online derivative calculator. It has been used to calculate these functions:

        double dYda=e * (exp(x/c)-1);
        double dYdb=e;
        double dYdc=(b-a)*x*e/(c*c);
        double dYdd=x;
        //fill Jacobian matrix with calculated values
        gsl_matrix_set(J,i,0,dYda);
        gsl_matrix_set(J,i,1,dYdb);
        gsl_matrix_set(J,i,2,dYdc);
        gsl_matrix_set(J,i,3,dYdd);
    }
    return GSL_SUCCESS;
}

int NonLinearApproximation::explineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f, gsl_matrix *J) {
    explineb_f(approximationCoefficients,vectorPtr,f);
    explineb_df(approximationCoefficients,vectorPtr,J);
    return GSL_SUCCESS;
}

int NonLinearApproximation::expexplineb_f(const gsl_vector *approximationCoefficients,void * vectorPtr, gsl_vector *coefficients) {
    //Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x

    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    //qDebug()<<"Initializing approximation function coefficients";
    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);
    double d=gsl_vector_get(approximationCoefficients,3);
    double e=gsl_vector_get(approximationCoefficients,4);
    double f=gsl_vector_get(approximationCoefficients,5);

    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        double y=point->at(i).y();
        double Yi= a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x;
        gsl_vector_set(coefficients,i,Yi-y);
    }

    return GSL_SUCCESS;
}

int NonLinearApproximation::expexplineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_matrix *J) {
    //Y(x) = a*(1-exp(-x/c)) + b*(exp (-x/d)-1) + e + f*x
    //converting void * to QVector<QPointF>
    QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

    double a=gsl_vector_get(approximationCoefficients,0);
    double b=gsl_vector_get(approximationCoefficients,1);
    double c=gsl_vector_get(approximationCoefficients,2);
    double d=gsl_vector_get(approximationCoefficients,3); //not used in jacobian matrix calculation because it's constant for all derivatives
    //double e=gsl_vector_get(approximationCoefficients,4); //not used
    //double f=gsl_vector_get(approximationCoefficients,5); //not used

    for (int i=0;i<point->size();++i) {
        double x=point->at(i).x();
        //double ex = exp ( -x / c );
        //each cell in Jacobian matric J should be filled with derivative of the function for a, b or c as variable for j=0..2 respectively
        //check http://www.numberempire.com/derivatives.php for online derivative calculator. It has been used to calculate these functions:

        double dYda=exp (-x/c) * (exp(x/c)-1);
        double dYdb=-exp(-x/d) * (exp(x/d)-1);
        double dYdc=-a*x*exp(-x/c)/(c*c);
        double dYdd=b*x*exp(-x/d)/(d*d);
        double dYde=1;
        double dYdf=x;

        //fill Jacobian matrix with calculated values
        gsl_matrix_set(J,i,0,dYda);
        gsl_matrix_set(J,i,1,dYdb);
        gsl_matrix_set(J,i,2,dYdc);
        gsl_matrix_set(J,i,3,dYdd);
        gsl_matrix_set(J,i,4,dYde);
        gsl_matrix_set(J,i,5,dYdf);
    }
    return GSL_SUCCESS;
}

int NonLinearApproximation::expexplineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f, gsl_matrix *J) {
    expexplineb_f(approximationCoefficients,vectorPtr,f);
    expexplineb_df(approximationCoefficients,vectorPtr,J);
    return GSL_SUCCESS;
}



       int NonLinearApproximation::lineb_f(const gsl_vector *approximationCoefficients,void * vectorPtr, gsl_vector *coefficients) {
           //Y(x) = a*x+b

           QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

           //qDebug()<<"Initializing approximation function coefficients";
           double a=gsl_vector_get(approximationCoefficients,0);
           double b=gsl_vector_get(approximationCoefficients,1);

           for (int i=0;i<point->size();++i) {
               double x=point->at(i).x();
               double y=point->at(i).y();
               double Yi= a*x+b;
               gsl_vector_set(coefficients,i,Yi-y);
           }

           return GSL_SUCCESS;
       }


       int NonLinearApproximation::lineb_df(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_matrix *J) {
           //Y(x) = a*x+b
           //converting void * to QVector<QPointF>
           QVector<QPointF> * point=(QVector<QPointF> *) vectorPtr;

           double a=gsl_vector_get(approximationCoefficients,0);
           double b=gsl_vector_get(approximationCoefficients,1);

           for (int i=0;i<point->size();++i) {
               double x=point->at(i).x();
               //double ex = exp ( -x / c );
               //each cell in Jacobian matric J should be filled with derivative of the function for a, b or c as variable for j=0..2 respectively
               //check http://www.numberempire.com/derivatives.php for online derivative calculator. It has been used to calculate these functions:

               double dYda=x;
               double dYdb=1;

               //fill Jacobian matrix with calculated values
               gsl_matrix_set(J,i,0,dYda);
               gsl_matrix_set(J,i,1,dYdb);
           }
           return GSL_SUCCESS;
       }

       int NonLinearApproximation::lineb_fdf(const gsl_vector *approximationCoefficients, void * vectorPtr, gsl_vector *f, gsl_matrix *J) {
           lineb_f(approximationCoefficients,vectorPtr,f);
           lineb_df(approximationCoefficients,vectorPtr,J);
           return GSL_SUCCESS;
       }


