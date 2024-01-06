#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

/******   hill破解运算     ******/
// 计算最大公约数
int gcd(int m, int n)
{
    if (n > m)
        swap(m, n);

    do
    {
        int temp = m % n;
        m = n;
        n = temp;
    } while (n != 0);

    return m;
}

// 计算逆元
int findInvers(int m, int n)
{
    int t0 = 0, t1 = 1, invers, q, r, b = m;
    while (r != 1)
    {
        q = m / n;
        r = m % n;
        invers = t0 - q * t1;
        if (invers < 0)
        {
            invers = b - (abs(invers) % b);
        }
        else
        {
            invers %= b;
        }
        t0 = t1;
        t1 = invers;
        m = n;
        n = r;
    }
    return invers;
}


/******   hill解密运算     ******/
// 计算mod逆元
int modInverse(int a, int m)
{ 
    a = a % m;
    for (int x = -m; x < m; x++)
        if ((a * x) % m == 1)
            return x;
}

// 计算余子式
void getCofactor(vector<vector<int>> &a, vector<vector<int>> &temp, int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = a[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// 计算行列式的值
int determinant(vector<vector<int>> &a, int n, int N)
{ 
    int D = 0;
    if (n == 1)
        return a[0][0];
    vector<vector<int>> temp(N, vector<int>(N));
    int sign = 1;
    for (int f = 0; f < n; f++)
    {
        getCofactor(a, temp, 0, f, n);
        D += sign * a[0][f] * determinant(temp, n - 1, N);
        sign = -sign;
    }
    return D;
}

// 计算伴随矩阵
void adjoint(vector<vector<int>> &a, vector<vector<int>> &adj, int N)
{ 
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }
    int sign = 1;
    vector<vector<int>> temp(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            getCofactor(a, temp, i, j, N);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign) * (determinant(temp, N - 1, N));
        }
    }
}

// 计算逆矩阵
bool inverse(vector<vector<int>> &a, vector<vector<int>> &inv, int N)
{
    int det = determinant(a, N, N);
    if (det == 0)
    { // 行列式的值为0，无法计算逆矩阵
        cout << "逆矩阵无法计算";
        return false;
    }
    int invDet = modInverse(det, 26);
    cout << det % 26 << ' ' << invDet << '\n';
    vector<vector<int>> adj(N, vector<int>(N));
    adjoint(a, adj, N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            inv[i][j] = (adj[i][j] * invDet) % 26;
    return true;
}