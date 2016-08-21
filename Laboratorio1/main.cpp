#include <iostream>

using namespace std;
#define MAX 10000

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
};
Matriz Three_nested_loop(Matriz A , Matriz B)
{
    Matriz C;
    for (int i=0;i<MAX;i++)
        for(int j=0;j<MAX;j++)
            for(int k=0;k<MAX;k++)
                C.mat[i][j]+=A.mat[i][k]*B.mat[k][j];
    return C;
}
int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
