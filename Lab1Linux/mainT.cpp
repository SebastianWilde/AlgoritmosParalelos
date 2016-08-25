#include <iostream>
#include <time.h>
using namespace std;
#define MAX 5000
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
    ~Matriz()
    {
        for (int i=0;i<MAX;i++)
            delete[] mat[i];
        delete[] mat;
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

int main()
{
    Matriz A, B, C;
    Three_nested_loop(A,B,C);
    cout << "Three_nested_loop" << endl;
    return 0;
}
