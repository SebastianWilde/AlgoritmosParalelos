#include <iostream>
#include <time.h>
using namespace std;
#define MAX 1000
struct Matriz
{
    int **mat = new int*[MAX];
    Matriz()
    {
        for (int i=0;i<MAX;i++)
            mat[i]=new int [MAX];
        for (int k=0;k<MAX;k++)
            for(int l=0;l<MAX;l++)
                mat[k][l]=l+k;
    }
    bool operator==(Matriz A)
    {
        bool test=1;
        for (int k=0;k<MAX && test;k++)
            for(int l=0;l<MAX && test ;l++)
               test=mat[k][l]== A.mat[k][l];
        return test;
    }
};
bool Three_nested_loop(Matriz A , Matriz B, Matriz & C)
{
    for (int i=0;i<MAX;i++)
        for(int j=0;j<MAX;j++)
            for(int k=0;k<MAX;k++)
                C.mat[i][j]+=A.mat[i][k]*B.mat[k][j];
    return 1;
}

bool Blocked_version_with_six_nested_loops(Matriz A , Matriz B, Matriz & C)
{
    int bloque=MAX/10;
    for(int i=0;i<MAX;i+=bloque)
        for(int j=0;j<MAX;j+=bloque)
            for(int k=0;k<MAX;k+=bloque)
                    for(int i2=i;i2<i+bloque;i2++)
                        for(int j2=j;j2<j+bloque;j2++)
                            for(int k2=k;k2<k+bloque;k2++)
                                C.mat[i2][j2]+=A.mat[i2][k2]*B.mat[k2][j2];
    return 1;
}
int main()
{
    Matriz A, B, C,D;
    clock_t t_ini1,t_ini2, t_fin1,t_fin2;
    double secs1,secs2;
    t_ini1 = clock();
    cout<<Three_nested_loop(A,B,C);
    t_fin1 = clock();
    t_ini2 = clock();
    cout<<Blocked_version_with_six_nested_loops(A,B,D);
    t_fin2 = clock();
    secs1 = (double)(t_fin1 - t_ini1) / CLOCKS_PER_SEC;
    secs2 = (double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC;
    cout<<endl<<"Three_nested_loop: "<< secs1 * 10000.0<<".16g milisegundos\n"<<endl;
    cout<<"Blocked_version_with_six_nested_loops: "<< secs2 * 10000.0<<".16g milisegundos\n"<<endl;
    bool test=C==D;
    cout<<test;
    cout << "Hello world!" << endl;
    return 0;
}
