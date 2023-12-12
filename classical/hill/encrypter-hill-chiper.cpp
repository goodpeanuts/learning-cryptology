/*
Anggota Kelompok : 	140810180005 - Fauzan Akmal hariz
                    140810180013 - Alvin
                    140810180049 - Rizky Anugerah
kelas	: A
Program	: Hill Chiper- Encryptor
*/
#include <iostream> //library
#include <vector>
using namespace std;

int main()
{
    int x, y, i, j, k, n, choice;
    cout << "Hill 密码加密" << endl;
    cout << "============================" << endl;
    cout << "请输入矩阵维数 : ";
    cin >> n;
    cout << "请输入密钥矩阵\n"; // input element matriks kunci
    int a[n][n];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }
    cout << "输入明文 : "; // input plainteks
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
    cout << ans << '\n';
}
