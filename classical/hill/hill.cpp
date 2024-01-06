/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-22 23:51:06
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 14:35:06
 * @FilePath: /learning-cryptology/classical/hill/hill.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <stdlib.h>
#include "hill.h"
using namespace std;

// 计算密钥
void findKey()
{
    string plainteks, cipherteks;
    int key[2][2], det, detInv, adj[2][2], plainteksInv[2][2], plainMatrix[2][2], cipMatrix[2][2], counter;
    int p, c;
    int transpose[2][2];

    cout << "输入明文 : ";
    cin.ignore();
    getline(cin, plainteks);

    // 将明文转置 plainMatrix
    counter = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            p = toupper(plainteks[counter]) - 65;
            plainMatrix[i][j] = p;
            counter++;
        }
    }

    cout << "输入密文 : ";
    getline(cin, cipherteks);

    // 将密文转置 cipMatrix
    counter = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            c = toupper(cipherteks[counter]) - 65;
            cipMatrix[i][j] = c;
            counter++;
        }
    }

    // 计算行列式的值
    det = (plainMatrix[0][0] * plainMatrix[1][1]) - (plainMatrix[0][1] * plainMatrix[1][0]);
    if (gcd(det, 26) == 1)
    {
        // 计算行列式的逆元
        detInv = findInvers(26, det);

        // 计算的伴随矩阵
        adj[0][0] = plainMatrix[1][1];
        adj[0][1] = (-1) * plainMatrix[0][1];
        adj[1][0] = (-1) * plainMatrix[1][0];
        adj[1][1] = plainMatrix[0][0];

        // 计算明文的逆矩阵
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                plainteksInv[i][j] = detInv * adj[i][j];
                if (plainteksInv[i][j] < 0)
                {
                    plainteksInv[i][j] = 26 - (abs(plainteksInv[i][j]) % 26);
                }
                else
                {
                    plainteksInv[i][j] = plainteksInv[i][j];
                    plainteksInv[i][j] = plainteksInv[i][j] % 26;
                }
            }
        }

        // 计算密钥
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                key[i][j] = 0;
                for (int k = 0; k < 2; k++)
                {
                    key[i][j] += (plainteksInv[i][k] * cipMatrix[k][j]);
                }
                key[i][j] %= 26;
            }
        }

        // 输出密钥
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                transpose[j][i] = key[i][j];
            }
        }

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                cout << (transpose[i][j]) << "\t";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "字母数量与矩阵不匹配" << endl;
    }
}


void hill_encrypt()
{
    int x, y, i, j, k, n, choice;
    cout << "Hill 密码加密" << endl;
    cout << "============================" << endl;
    cout << "请输入矩阵维数 : ";
    cin >> n;
    cout << "请输入密钥矩阵\n"; // 输入密钥矩阵
    int a[n][n];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }
    cout << "输入明文 : "; // 输入明文
    string s;
    cin >> s;
    int temp = (n - s.size() % n) % n;
    for (i = 0; i < temp; i++)
    {
        s += 'x';
    }
    k = 0;
    string ans = "";
    while (k < s.size())
    {
        for (i = 0; i < n; i++)
        {
            int sum = 0;
            int temp = k;
            for (j = 0; j < n; j++)
            {
                sum += (a[i][j] % 26 * (s[temp++] - 'a') % 26) % 26;
                sum = sum % 26;
            }
            ans += (sum + 'a');
        }
        k += n;
    }
    cout << "加密结果 ："<< ans << '\n';
}

void hill_decrypt()
{
    int x, y, i, j, k, n;

    cout << "Hill 密码解密" << endl;
    cout << "============================" << endl;
    cout << "请输入矩阵维数 : "; 
    cin >> n;
    cout << "请输入密钥矩阵\n"; 
    vector<vector<int>> a(n, vector<int>(n));
    vector<vector<int>> adj(n, vector<int>(n));
    vector<vector<int>> inv(n, vector<int>(n));
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }
    if (inverse(a, inv, n))
    {
        cout << "矩阵可逆\n";
    }
    cout << "输入密文 :\n";
    string s;
    cin >> s;
    k = 0;
    string ans;
    while (k < s.size())
    {
        for (i = 0; i < n; i++)
        {
            int sum = 0;
            int temp = k;
            for (j = 0; j < n; j++)
            {
                sum += ((inv[i][j] + 26) % 26 * (s[temp++] - 'a') % 26) % 26;
                sum = sum % 26;
            }
            ans += (sum + 'a');
        }
        k += n;
    }
    // ans+='\0';
    int f = ans.size() - 1;
    while (ans[f] == 'x')
    {
        f--;
    }
    for (i = 0; i <= f; i++)
    {
        cout << ans[i];
    }
    cout << '\n';
}

int main()
{
    bool menuActive = true;
    int choice;
    while (menuActive)
    {
        cout << "\nHill 密码" << endl;
        cout<<"=============================="<<endl;
        cout << "选项 : " << endl;
        cout << "1. 破解" << endl;
        cout << "2. 加密" << endl;
        cout << "3. 解密" << endl;
        cout << "4. 退出" << endl;
        cout << "选择 : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << endl;
            findKey();
            break;
        case 2:
            cout << endl;
            hill_encrypt();
            break;
        case 3:
            cout << endl;
            hill_decrypt();
            break;
        case 4:
            menuActive = false;
            break;
        default:
            cout << "\n无效选择" << endl;
            break;
        }
    }
}