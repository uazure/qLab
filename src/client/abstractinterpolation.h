#ifndef ABSTRACTINTERPOLATION_H
#define ABSTRACTINTERPOLATION_H


//размер массивов точек данных
#define N_T 651
//количество базисных функций
#define M_T 10


#include <QVector>
#include <QPointF>
#include <QDebug>

//mathematical functions from C language (pow,exp...)
#include <cmath>

class AbstractInterpolation: public QObject
{
    Q_OBJECT

public:
    enum Polynomial {
        polynomExpLine,
        polynomExpExp,
        polynomExpFIXME
    };

    AbstractInterpolation();

    virtual void interpolate(const QVector<QPointF> & data);

public slots:
    void setT0(double Tzero);


    //FIXME: this function should be moved to appropriate subclass
    void Line_Reg(int valcount,int *val,double *XD,double *YD,double *k,double *b0,double *xs,double *ys,double *xmin,double *xmax);

    void lineApproximation (const QVector<QPointF> &data,
                            double &k,
                            double &b0,
                            double &avgX,
                            double &avgY,
                            double &minX,
                            double &maxX
                            );

protected:
    /** функция вычисляет значение S полинома степени М в точке Х1
    X1 - значение аргумента
    С - массив коэффициентов
    */
    static double Bas_Poly(int M, double X1,double *C);

    /** функция вычисляет значение производной S полинома степени М в точке Х1
    X1 - значение аргумента
    С - массив коэффициентов
    */
    static double Bas_Poly_h(int M,double X1,double *C);

    /**
    возвращает значение базисные функции,
    N - размер входных массивов
    M - номер функции (c нуля)
    X1 - значение аргумента
    X - массив аргументов
    T - массив значений (выход)
    */
    static double Bas_Exp(int M,double X0,double X1,double c,double *CH);

    /** базисные функции,
    N - размер входных массивов
    M - номер функции (c нуля)
    X1 - значение аргумента
    X - массив аргументов
    T - массив значений (выход)
    */
    static void Bas(int N,int M,int Poly,double X1,double c,double *X,double *T);

    /** Calculates parameters for Gram matrix.
      T - array of doubles which stores coefficients of Fi functions in Gram matrix
          T is used to retun calculated values.
      Poly - type of polynomial to use. Refer to enum Polynomial
      x - argument value
      c - coefficient value
      */
    static void calculateBasisForGramMatrix (QVector <double> &T,Polynomial Poly,const double x, const double c);

/** Calculates array of coefficients of Gram matrix.
    data - array of selected points
    M - number of approximation functions (2-4)
    Poly - type of polynomial
    x - X value (usually time)
    c - coefficient
*/
    static QVector<QVector<long double> > calculateGramMatrix(const QVector<QPointF> &data,int M,Polynomial Poly, double c);


    /** заполнение матрицы Грамма
    N - размер входных массивов
    M - количество функций (с нуля)
    X - массив абсцисс входных точек
    F - массив ординат входных точек
    A - результирующая матрица
    */
    static void Gram(int N,int M,int Poly,double c,double *X,double *F,long double A[M_T][M_T]);


    static void calculateGauss(QVector<double> &coef,QVector<QVector<long double> > &A);

    //решение СЛАУ методом Гаусса
    static void Gauss(int N,double *X,long double A[M_T][M_T]);

    static void calculateMNK(Polynomial Poly,double X0,double c_k,QVector<QPointF> &data,QVector<double> &coef,bool *error);


    /** вызов расчета аппроксимации функции методом
     наименьших квадратов с
    произвольным базисом
     c_k - коэфициент экспоненты
     X,Y - массивы входных точек
    C - выходной массив коэфициентов
    */
    static void CalcMNK(int N,int M,int Poly,int *val,double X0,double c_k,double *XData,double *YData,double *C,bool *error);


    static double calculateRMS(Polynomial Poly,double X0,double c_k,const QVector<QPointF> &data,const QVector<double> &coef, bool *error);

    /** возвращает значение суммы среднеквадратичных отклонений
    аппроксимирующей функции в точках XData относительно
    значений YData */
    static double Skvo(int N,int M,int Poly,int *val,double X0,double c_k,double *C,double *XData,double *YData);


    /** data - array of selected points for processing
        coef - array of coefficients, coef.size()==M-1
        Poly - type of interpolation. Selected by user.
        val - ???? WHAT IS THIS???
        X0 - starting X value (selected as T0 point by user)
        c_k_start - starting value for optimization (defined in settings)
        c_k_end - end value for optimization (defined in settings)
        stepCount - number of steps for variating c_k between c_k_start and c_k_end
        error - error indicator. Probably it's not required. It's better to throw some exceptions or emit signals
      */
    static double calculateOptimizedMNK(QVector<QPointF> &data,QVector<double> &coef,Polynomial Poly, double X0, double c_k_start, double c_k_end, int stepCount,bool *error);

    /** ВОЗВРАЩАЕТ оптимизированное значение
    коэфициента экспоненты для
    расчета аппроксимации функции методом
     наименьших квадратов с
    произвольным базисом
     c_k_Start - стартовое значение коэфициента экспоненты
     c_k_End - конечное значение коэфициента экспоненты
     XData,YData - массивы входных точек
    C - выходной массив коэфициентов */
    static double CalcMNK_opt(int N,int M,int Poly,int *val,double X0,double c_k_Start,double c_k_End,double *XData,double *YData,double *C,int *error_TH,bool *error,int col_h);



    /** аппроксимирующая функция
    возвращает значение производной функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_h(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData);

    /** аппроксимирующая функция
    возвращает значение производной функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_h_Exp(int M,double c_k,double X1,double *C);

    /** аппроксимирующая функция
    возвращает значение функции в точке X1 для полинома
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi_Exp(int M,double c_k,double X1,double *C);

    /** аппроксимирующая функция
    возвращает значение функции в точке X1
    коэфициенты базисных функций С[i] должны быть определены
    предварительным вызовом функции CalcMNK
    */
    static double Fi(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData);


private:
    double T0;


signals:
    void T0Selected(void);

};

#endif // ABSTRACTINTERPOLATION_H
