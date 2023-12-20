/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 04:16:35
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-20 06:49:39
 * @FilePath: /learning-cryptology/rsa/gmp.cpp
 * @Description: 使用gmp实现RSA算法
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <iostream>
#include <gmpxx.h>
#include <cstdlib>

using namespace std;

mpz_class randbits(int bits) // base = 2
{
    gmp_randclass a(gmp_randinit_default);
    a.seed(rand());
    mpz_class l(bits);
    return a.get_z_bits(l);
}

inline mpz_class randprime(int bits)
{
    mpz_class a = randbits(bits);
    mpz_class ret;
    mpz_nextprime(ret.get_mpz_t(), a.get_mpz_t());
    return ret;
}

void setKey(mpz_class &n, mpz_class &e, mpz_class &d, const int nbits, int ebits = 16)
{
    if (nbits / 2 <= ebits)
    {
        ebits = nbits / 2;
    }
    mpz_class p = randprime(nbits / 2); //随机取p
    mpz_class q = randprime(nbits / 2); //随机取q
    n = q * p;                          //计算n=p*q
    mpz_class fn = (p - 1) * (q - 1);   //计算欧拉数
    mpz_class gcd;
    do
    {
        e = randprime(ebits);           //随机取e
        mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), fn.get_mpz_t());       //判断gcd(e,fn)=1是否成立
    } while (gcd != 1);
    //mpz_gcdext(g, s, t, a, b)： g = as + bt
    mpz_gcdext(p.get_mpz_t(), d.get_mpz_t(), q.get_mpz_t(), e.get_mpz_t(), fn.get_mpz_t()); //计算d=e^{-1} mod fn
}

inline mpz_class encrypt(const mpz_class &m, const mpz_class &e, const mpz_class &n)
{
    mpz_class ret;
    mpz_powm(ret.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t()); //ret=m^e mod n
    return ret;
}

inline mpz_class decrypt(const mpz_class &c, const mpz_class &d, const mpz_class &n)
{
    return encrypt(c, d, n);    //m=c^d mod n
}

int main()
{
    int nbits;
    cout << "输入大数比特数:";
    cin >> nbits;
    mpz_class n, e, d;
    setKey(n, e, d, nbits);         //密钥生成
    cout << "公钥:(e=" << e.get_str() << ", n=" << n.get_str() << ")" << endl;
    cout << "私钥:(d=" << d.get_str() << ", n=" << n.get_str() << ")" << endl;

    cout << "输入加密数据:";
    string s;
    cin >> s;

    mpz_class m(s);
    mpz_class c;
    c = encrypt(m, e, n);       //加密
    cout << "加密后:" << c.get_str() << endl;
    c = decrypt(c, d, n);       //解密
    cout << "解密后:" << c.get_str() << endl;
    if (c == m)
        cout << "加/解密成功!" << endl
             << endl;
    else
        cout << "加/解密失败!" << endl
             << endl;

    string q;
    cout << "是否继续(Y/N):";
    cin >> q;
    if (q == "y" || q == "Y")
        main();
    return 0;
}
