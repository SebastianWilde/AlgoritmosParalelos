#include <iostream>
#include <time.h>
using namespace std;
int main()
{
    cout << "Hello world!" << endl;
    double** A = new double*[100000];
    for (int i=0;i<10000;i++)
        A[i]=new double [10000];
    double X[100000] = {1};
    double Y[100000] = {0};
    clock_t t_ini1,t_ini2, t_fin1,t_fin2;
    double secs1,secs2;
    for (int k=0;k<100;k++)
    {
        X[k]=k;
        for(int l=0;l<10000;l++)
            A[k][l]=l;
    }
    t_ini1 = clock();
    for (int i=0;i<10000;i++)
        for(int j=0;j<10000;j++)
            Y[i]+=A[i][j]*X[j];
    t_fin1 = clock();
    t_ini2 = clock();
    for (int j=0;j<10000;j++)
        for(int i=0;i<10000;i++)
            Y[i]+=A[i][j]*X[j];
    t_fin2 = clock();
    secs1 = (double)(t_fin1 - t_ini1) / CLOCKS_PER_SEC;
    secs2 = (double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC;
    cout<<"Primero: "<< secs1 * 10000.0<<".16g milisegundos\n";
    cout<<"Segundo: "<< secs2 * 10000.0<<".16g milisegundos\n";
    cout << "Hello world!" << endl;
    return 0;
}
