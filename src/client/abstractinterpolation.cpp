#include "plot.h"
#include "abstractinterpolation.h"


AbstractInterpolation::AbstractInterpolation()
{
    T0=-1;
    //plot=plotObject;
}

void AbstractInterpolation::interpolate(const QVector<QPointF> &data) {
    if (!issetT0()) {
        qWarning()<<"T0 not set";
        return;
    }


}

void AbstractInterpolation::setT0(double Tzero) {
    T0=Tzero;
    qDebug()<<"T0 set to"<<T0;
    emit T0Selected();
    //disconnect this slot from all signals.
    disconnect(this,SLOT(setT0(double)));
}





void AbstractInterpolation::Line_Reg(int valcount, int *val, double *XD, double *YD, double *k, double *b0, double *xs, double *ys, double *xmin, double *xmax) {
    /** valcount - кол-во
      valcount - кол-во выделенных пользователем точек.
      *val - массив, содержащий индексы всех выделенных пользователем точек
      *XD - массив для чтения всех X-данных в кривой (не только выделенных)
      *XY - массив для чтения всех Y-данных в кривой (не только выделенных)
      *k - указатель на коэффициент линейной зависимости (a=kx+b0)
      *b0 - указатель на свободный член линейной зависимости (a=kx+b0)
      *xs - указатель на среднее значение иксов
      *ys - указатель на среднее значение игреков
      *xmin - указатель на минимальное значение икс
      *ymax - указатель на максимальное значение икс
       */

    //нахождение коэфициентов линейной регрессии
    double a,b,c,d;
    int nt,i;
    a=b=c=d=0;nt=0;
    (*ys)=0;
    (*xmin)=(*xmax)=XD[val[0]];
    for (i=0;i<valcount;i++)
    {
        a+=XD[val[i]];b+=YD[val[i]];c+=XD[val[i]]*XD[val[i]];
        d+=XD[val[i]]*YD[val[i]];
        if(XD[val[i]]>(*xmax)) (*xmax)=XD[val[i]];
        if(XD[val[i]]<(*xmin)) (*xmin)=XD[val[i]];
        nt++;
    }
    (*xs)=a/nt;
    (*ys)=b/nt;
    (*k)=(a*b-nt*d)/(a*a-nt*c);
    (*b0)=(b-(*k)*a)/nt;
}

void AbstractInterpolation::lineApproximation(const QVector<QPointF> &data, double &k, double &b0, double &avgX, double &avgY, double &minX, double &maxX) {

    if (data.size()<2) {
        //if there's no sufficient data then just exit
        return;
    }

    double a=0,b=0,c=0,d=0;
    int nt=data.size()-1;

    minX=data.at(0).x();
    maxX=minX;

    for (int i=0;i<data.size();++i) {
        a+=data.at(i).x();
        b+=data.at(i).y();
        c+=a*a;
        d+=a*b;
    }

    avgX=a/nt;
    avgY=b/nt;
    k=(a*b-nt*d)/(a*a-nt*c);
    b0=(b-k*a)/nt;
}


/** Calculates parameters for Gram matrix.
  T - array of doubles which stores coefficients of Fi functions in Gram matrix
      T is used to retun calculated values.
  Poly - type of polynomial to use. Refer to enum Polynomial
  x - argument value
  c - coefficient value
  */
void AbstractInterpolation::calculateBasisForGramMatrix(QVector<double> &T, Polynomial Poly, double x, double c) {
    if (T.size()==0) {
        qWarning()<<"vector T has zero size";
        return;
    }
    int M = T.size()-1;
    switch (Poly) {
    case polynomExpLine:
        for (int k=0;k<=M;++k) {
            switch (k)
            {
            case 0:T[0] =exp(-c*x);
                break;
            case 1:T[1] =1-exp(-c*x);
                break;
            case 2:T[2]=x;
                break;
            default :{
                qWarning()<<"Requested k larger than T.size()";
                return;
            }

        }

        }
        break;

    case polynomExpExp:
        if (T.size()<2) {
            qWarning()<<"Insufficient data in T";
            return;
        }
        T[0]=1;
        T[1]=x;
        for(int k=2;k<=M;++k) {
            T[k]=T[k-1]*x;
        }
        break;

    case polynomExpFIXME:
        if (T.size()<3) {
            qWarning()<<"Insufficient data in T";
            return;
        }

        T[0]=1;
        T[1]=x;
        for (int k=2;k<=M-1;++k) {
            T[k]=T[k-1]*x;
        }
        T[M]=exp(-c*x);
        break;
    }
}


/** базисные функции,
N - размер входных массивов
M - номер функции (c нуля)
X1 - значение аргумента
X - массив аргументов
T - массив значений (выход)
*/
void AbstractInterpolation::Bas(int N,int M,int Poly,double X1,double c,double *X,double *T)
{

    int k;
    double Z;
    //Z = 2*(X1-X[0])/(X[N]-X[0])-1.0;
    Z=X1;
    switch (Poly)
    {
    case 0:
        for(k=0;k<=M;k++)
        {
            switch (k)
            {
            case 0:T[0] =exp(-c*Z);
                break;
            case 1:T[1] =1-exp(-c*Z);
                break;
            case 2:T[2]=Z;
                break;
            default :{return;}
        }//switch (k)
        }//for k
        break;
case 1:
        {
            T[0]=1;T[1]=Z;
            for(k=2;k<=M;k++) T[k]=T[k-1]*Z;
        }
        break;
case 2:
        {
            T[0]=1;T[1]=Z;
            for(k=2;k<=M-1;k++) T[k]=T[k-1]*Z;
            T[M] =exp(-c*Z);
            break;
        }//case 2:
    }//switch (Poly)
}


QVector<QVector<long double> > AbstractInterpolation::calculateGramMatrix(
        const QVector<QPointF> &data,
        int M,
        Polynomial Poly,
        double c) {

    int i,j,k;
    long double q,r,s;
    int N = data.size()-1;

    if (M<0) {
        qWarning()<<"M for calculateGramMatrix is"<<M<<"Consider quit";
        QVector<QVector<long double> > A(0);
        return A;
    }
        QVector<QVector<long double> > A(M);

        for (int i=0;i<A.size();++i) {
            A[i].resize(M+1);
        }


    QVector<double> T(M);

    //long double *P=new long double[M*N];
    long double **P=new long double* [M];
    for (int i=0;i<M;++i) {
        P[i]=new long double[N];
        for (int j=0;j<N;++j) {
            P[i][j]=0;
        }
    }

    //long double P[M_T][N_T];//матрица плана

    for(i=0;i<=N;i++)
    {
        calculateBasisForGramMatrix(T,Poly,data.at(i).x(),c);
        //Bas(N,M,Poly,X[i],c,X,T);
        for(j=0;j<M;j++) {
            P[j][i]= T[j];
        }
    }

    for(k=0;k<M;k++)
    {
        for(j=0;j<M;j++)
        {
            s=0.0;r=0.0;
            for(i=0;i<=N;i++)
            {
                q=P[k][i];
                s+=q*P[j][i];
                if(j==M)
                    r+=q*data.at(i).y();
            }//for i
            A[k][j]=s;A[j][k]=s;
        }//for j
        A[k][M]=r;
    }//for k
    return A;
}

/** заполнение матрицы Грамма
N - размер входных массивов
M - количество функций (с нуля)
X - массив абсцисс входных точек
F - массив ординат входных точек
A - результирующая матрица
*/
void AbstractInterpolation::Gram(int N,int M,int Poly,double c,double *X,double *F,long double A[M_T][M_T])
{

    int i,j,k;
    long double q,r,s;
    double T[N_T];
    long double P[M_T][N_T];//матрица плана
    for(i=0;i<=N;i++)
    {
        Bas(N,M,Poly,X[i],c,X,T);
        for(j=0;j<=M;j++) P[j][i]= T[j];
    }
    for(k=0;k<=M;k++)
    {
        for(j=0;j<=M;j++)
        {
            s=0.0;r=0.0;
            for(i=0;i<=N;i++)
            {
                q=P[k][i];
                s+=q*P[j][i];
                if(j==M)
                    r+=q*F[i];
            }//for i
            A[k][j]=s;A[j][k]=s;
        }//for j
        A[k][M+1]=r;
    }//for k
}


void AbstractInterpolation::calculateGauss(QVector<double> &coef, QVector<QVector<long double> > &A) {
    int i,j,k,N1,K1;
    long double R,S;
    int N=coef.size()-1;
    N1 = N+1;
    for(k=0;k<=N;k++)
    {
        K1=k+1;S = A[k][k];
        for(j=K1;j<=N1;j++) A[k][j]=A[k][j]/S;
        for(i=K1;i<=N;i++)
        {
            R = A[i][k];
            for(j=K1;j<=N1;j++) A[i][j]-=A[k][j]*R;
        }//for i
    }//for k
    for(i=N;i>=0;i--)
    {
        S=A[i][N1];
        for(j=i+1;j<=N;j++) S-=A[i][j]*coef[j];
        coef[i]=S;
    }//for i
}

//решение СЛАУ методом Гаусса
void AbstractInterpolation::Gauss(int N,double *X,long double A[M_T][M_T])
{

    int i,j,k,N1,K1;
    long double R,S;
    N1 = N+1;
    for(k=0;k<=N;k++)
    {
        K1=k+1;S = A[k][k];
        for(j=K1;j<=N1;j++) A[k][j]=A[k][j]/S;
        for(i=K1;i<=N;i++)
        {
            R = A[i][k];
            for(j=K1;j<=N1;j++) A[i][j]-=A[k][j]*R;
        }//for i
    }//for k
    for(i=N;i>=0;i--)
    {
        S=A[i][N1];
        for(j=i+1;j<=N;j++) S-=A[i][j]*X[j];
        X[i]=S;
    }//for i
}



void AbstractInterpolation::calculateMNK(Polynomial Poly,
                                         double X0,
                                         double c_k,
                                         QVector<QPointF> &data,
                                         QVector<double> &coef,
                                         bool *error)
{
    //long double A[M_T][M_T];//матрица Грамма

    int N=data.size()-1; //last index of data
    int i;
    //double X[N_T],Y[N_T];
    *error=true;
    QVector<QPointF> normalizedData=data;

    //normalize time.
    for (i=0;i<=N;i++)
    {
        //set x=x-x0 for every QPointF
        normalizedData[i].setX(data.at(i).x()-X0);
    }

    QVector<QVector<long double> > A=calculateGramMatrix(normalizedData,coef.size(),Poly,c_k);
    //Gram(N,M,Poly,c_k,X,Y,A);
    calculateGauss(coef,A);
    //Gauss(M,C,A);
    *error=false;
}




/** вызов расчета аппроксимации функции методом
 наименьших квадратов с
произвольным базисом
 c_k - коэфициент экспоненты
 X,Y - массивы входных точек
C - выходной массив коэфициентов
*/
/** N - points count
    M - parameter that is passed to Gram()
    c_k - parameter passed to Gram()
    A - Gram matrix
    Poly - type of approximation function (index)
  */
void AbstractInterpolation::CalcMNK(int N,int M,int Poly,int *val,double X0,double c_k,double *XData,double *YData,double *C,bool *error)
{
    long double A[M_T][M_T];//матрица Грамма

    int i;
    double X[N_T],Y[N_T];
    *error=true;
    for (i=0;i<=N;i++)
    {
        X[i]=XData[val[i]]-X0;
        Y[i]=YData[val[i]];
    }
    Gram(N,M,Poly,c_k,X,Y,A);
    //Gauss returns double C value (and possible values for A)
    Gauss(M,C,A);
    *error=false;
}


double AbstractInterpolation::calculateRMS(Polynomial Poly, double X0, double c_k, const QVector<QPointF> &data, const QVector<double> &coef, bool *error) {
    int i,j;

    double S,S_k;
    int N=data.size()-1;
    S_k=0;
    QVector<double> T(coef.size());

    QVector<QPointF> normalizedData=data;
    for (i=0;i<=N;i++)
    {
        //set x=x-x0 for every QPointF
        normalizedData[i].setX(data.at(i).x()-X0);
    }

    for (j=0;j<=N;j++)
    {
        S=0;
        calculateBasisForGramMatrix(T,Poly,normalizedData.at(j).x(),c_k);
        //Bas(N,M,Poly,X[j],c_k,X,T);
        //FIXME: what is C and T???
        for (i=0;i<coef.size();++i) S+=coef.at(i)*T.at(i);
        S_k+=(S-data.at(j).y())*(S-data.at(j).y());
    }
    return(S_k);
}


/** возвращает значение суммы среднеквадратичных отклонений
аппроксимирующей функции в точках XData относительно
значений YData */

//C - array of double, C[i] where i = 0 .. M
double AbstractInterpolation::Skvo(int N,int M,int Poly,int *val,double X0,double c_k,double *C,double *XData,double *YData)
{
    int i,j;
    double T[M_T];
    double S,S_k;
    double X[N_T];
    S_k=0;
    for (i=0;i<=N;i++)
        X[i]=XData[val[i]]-X0;
    for (j=0;j<=N;j++)
    {
        S=0;
        Bas(N,M,Poly,X[j],c_k,X,T);
        for (i=0;i<=M;i++) S+=C[i]*T[i];
        S_k+=((S-YData[val[j]])*(S-YData[val[j]]));
    }
    return(S_k);
}

double AbstractInterpolation::calculateOptimizedMNK(QVector<QPointF> &data,
                                                    QVector<double> &coef,
                                                    Polynomial Poly,
                                                    double X0,
                                                    double c_k_start,
                                                    double c_k_end,
                                                    int stepCount,
                                                    bool *error) {
    double h,c_k,S,S_min,c_ret;
    int i,i_ret;
    *error=true;
    //h - step size for parameter variation
    //col_h - number of steps
    h = (c_k_end-c_k_start)/stepCount;
    S_min=0;
    for (i=0;i<stepCount;i++)
    {
    c_k=c_k_start+h*i;
    //calculate RMS value for each c_k value
    //C - is *double where coefficients is written. C[i] where i = 0 .. M
    calculateMNK(Poly,X0,c_k,data,coef,error);
    //CalcMNK(N,M,Poly,val,X0,c_k,XData,YData,C,error);
    //S - is RMS deviation of aprpoximation function with current parameters set (c_k, calculated C)
    S = calculateRMS(Poly,X0,c_k,data,coef,error);
    //S = Skvo(N,M,Poly,val,X0,c_k,C,XData,YData);
    if(!i) {S_min=S;i_ret=i;c_ret=c_k;}
    if(S<S_min)
            {
            S_min=S;c_ret=c_k;i_ret=i;
            }
    if(S>S_min*5) break;
    }
    if (!i_ret) {
        //*error_TH=1;
        *error=false;
        return(c_ret);
    }

    if (i_ret>=stepCount-1) {
        //*error_TH=2;
        *error=false;
        return(c_ret);
    }

    c_k_start = (c_k_start+h*(i_ret-2));
    c_k_end = (c_k_start+h*(i_ret+2));
    h = (c_k_end-c_k_start)/stepCount;
    for (i=0;i<stepCount;i++)
    {
    c_k=c_k_start+h*i;
    calculateMNK(Poly,X0,c_k,data,coef,error);
    //CalcMNK(N,M,Poly,val,X0,c_k,XData,YData,C,error);
    S = calculateRMS(Poly,X0,c_k,data,coef,error);
    //S = Skvo(N,M,Poly,val,X0,c_k,C,XData,YData);
    if(!i) {S_min=S;i_ret=i;c_ret=c_k;}
    if(S<S_min)
            {
            S_min=S;c_ret=c_k;i_ret=i;
            }
    }
    *error=false;
    return(c_ret);
}



/** ВОЗВРАЩАЕТ оптимизированное значение
коэфициента экспоненты для
расчета аппроксимации функции методом
 наименьших квадратов с
произвольным базисом
 c_k_Start - стартовое значение коэфициента экспоненты
 c_k_End - конечное значение коэфициента экспоненты
 XData,YData - массивы входных точек
C - выходной массив коэфициентов */
double AbstractInterpolation::CalcMNK_opt(int N,int M,int Poly,int *val,double X0,double c_k_Start,double c_k_End,double *XData,double *YData,double *C,int *error_TH,bool *error,int col_h) {
    double h,c_k,S,S_min,c_ret;
    int i,i_ret;
    *error=true;
    //h - step size for parameter variation
    //col_h - number of steps
    h = (c_k_End-c_k_Start)/col_h;
    S_min=0;*error_TH=0;
    for (i=0;i<col_h;i++)
    {
    c_k=c_k_Start+h*i;
    //calculate RMS value for each c_k value
    //C - is *double where coefficients is written. C[i] where i = 0 .. M
    CalcMNK(N,M,Poly,val,X0,c_k,XData,YData,C,error);
    //S - is RMS deviation of aprpoximation function with current parameters set (c_k, calculated C)
    S = Skvo(N,M,Poly,val,X0,c_k,C,XData,YData);
    if(!i) {S_min=S;i_ret=i;c_ret=c_k;}
    if(S<S_min)
            {
            S_min=S;c_ret=c_k;i_ret=i;
            }
    if(S>S_min*5) break;
    }
    if (!i_ret) {*error_TH=1;*error=false;
    return(c_ret);}
    if (i_ret>=col_h-1) {*error_TH=2;*error=false;
    return(c_ret);}
    c_k_Start = (c_k_Start+h*(i_ret-2));
    c_k_End = (c_k_Start+h*(i_ret+2));
    h = (c_k_End-c_k_Start)/col_h;
    for (i=0;i<col_h;i++)
    {
    c_k=c_k_Start+h*i;
    CalcMNK(N,M,Poly,val,X0,c_k,XData,YData,C,error);
    S = Skvo(N,M,Poly,val,X0,c_k,C,XData,YData);
    if(!i) {S_min=S;i_ret=i;c_ret=c_k;}
    if(S<S_min)
            {
            S_min=S;c_ret=c_k;i_ret=i;
            }
    }
    *error=false;
    return(c_ret);
}


/** функция вычисляет значение S полинома степени М в точке Х1
X1 - значение аргумента
С - массив коэффициентов
*/
double AbstractInterpolation::Bas_Poly(int M, double X1, double *C) {

    int k;
    double S;
    S=C[0];
    for(k=1;k<=M;k++) {
       S+=C[k]*pow(X1,k);
    }

    return S;
}

/** функция вычисляет значение производной S полинома степени М в точке Х1
X1 - значение аргумента
С - массив коэффициентов
*/
double AbstractInterpolation::Bas_Poly_h(int M,double X1,double *C)
{
    int k;
    double S;
    S=C[1];
    for(k=2;k<=M;k++)
        S+=C[k]*pow(X1,k-1)*k;
    return(S);
}

/**
возвращает значение базисные функции,
N - размер входных массивов
M - номер функции (c нуля)
X1 - значение аргумента
X - массив аргументов
T - массив значений (выход)
*/
double  AbstractInterpolation::Bas_Exp(int M,double X0,double X1,double c,double *CH)
{
    int k;
    double Z;
    X1-=X0;
    Z=0;
    for(k=0;k<=M;k++)
    {
        switch (k)
        {
        case 0:Z+=exp(-c*X1)*CH[k];
            break;
        case 1:Z+=(1-exp(-c*X1))*CH[k];
            break;
        case 2:Z+=X1*CH[k];
            break;
        default :{return(Z);}
    }
    }
    return(Z);
}


/** аппроксимирующая функция
возвращает значение производной функции в точке X1 для полинома
коэфициенты базисных функций С[i] должны быть определены
предварительным вызовом функции CalcMNK
*/
double AbstractInterpolation::Fi_h(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData)
{
    int i;
    double T[M_T];
    double S;
    double X[N_T];
    for (i=0;i<=N;i++)
        X[i]=XData[val[i]]-X0;
    S=0;
    Bas(N,M,Poly,X1-X0,c_k,X,T);
    for (i=1;i<=M;i++)
        S+=i*C[i]*T[i-1];
    return(S);
}


/** аппроксимирующая функция
возвращает значение производной функции в точке X1 для полинома
коэфициенты базисных функций С[i] должны быть определены
предварительным вызовом функции CalcMNK
*/
double AbstractInterpolation::Fi_h_Exp(int M,double c_k,double X1,double *C) {
    double S=0;
    int i;
    for(i=1;i<=M-1;i++)
    {
        if(i>1)
            S+=C[i]*i*pow(X1,i-1);
        else S+=C[i];
    }
    S+=C[M]*(-c_k)*exp(-c_k*X1);
    return(S);
}

/** аппроксимирующая функция
возвращает значение функции в точке X1 для полинома
коэфициенты базисных функций С[i] должны быть определены
предварительным вызовом функции CalcMNK
*/
double AbstractInterpolation::Fi_Exp(int M,double c_k,double X1,double *C)
{
    double S=0;
    int i;
    for(i=0;i<=M-1;i++)
    {
        if(i)
            S+=C[i]*pow(X1,i);
        else S+=C[i];
    }
    S+=C[M]*exp(-c_k*X1);
    return(S);
}


/** аппроксимирующая функция
возвращает значение функции в точке X1
коэфициенты базисных функций С[i] должны быть определены
предварительным вызовом функции CalcMNK
*/
double AbstractInterpolation::Fi(int N,int M,int Poly,int *val,double X0,double c_k,double X1,double *C,double *XData)
{
    int i;
    double T[M_T];
    double S;
    double X[N_T];
    for (i=0;i<=N;i++)
        X[i]=XData[val[i]]-X0;
    S=0;
    Bas(N,M,Poly,X1-X0,c_k,X,T);
    for (i=0;i<=M;i++) S+=C[i]*T[i];
    return(S);
}

void AbstractInterpolation::resetT0()
{
    T0=-1;
}

bool AbstractInterpolation::issetT0() const
{
    if (T0!=-1) return true;
    return false;
}

