#include <iostream>
#include <time.h>
using namespace std;
#define MAX 100
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
/*    ~Matriz()
    {
        for (int i=0;i<MAX;i++)
            delete[] mat[i];
        delete[] mat;
    }*/
};


bool Blocked_version_with_six_nested_loops(Matriz A , Matriz B, Matriz & C)
{
    int bloque=MAX/10;
    for(int i=0;i<MAX;i+=bloque)
        for(int j=0;j<MAX;j+=bloque)
            for(int k=0;k<MAX;k+=bloque)
                    for(int x=i;x<i+bloque;x++)
                        for(int y=j;y<j+bloque;y++)
                            for(int z=k;z<k+bloque;z++)
                                C.mat[x][y]+=A.mat[x][z]*B.mat[z][y];
    return 1;
}
int main()
{
    Matriz A, B, C;
    Blocked_version_with_six_nested_loops(A,B,C);
    cout << "Blocked_version_with_six_nested_loops" << endl;
    return 0;
}
