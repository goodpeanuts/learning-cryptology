/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 07:21:22
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-20 09:17:12
 * @FilePath: /learning-cryptology/rsa/rsa.cpp
 * @Description: 使用Crypto++实现RSA算法
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/rsa.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/files.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cassert>

/**
 * @brief 求最大公约数
 * 
 * @param a 
 * @param b 
 * @return CryptoPP::Integer 
 */
CryptoPP::Integer gcd(const CryptoPP::Integer &a, const CryptoPP::Integer &b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

/**
 * @brief 扩展欧几里得算法
 * 
 * @param a 
 * @param b 
 * @param x 
 * @param y 
 * @return CryptoPP::Integer 
 */
CryptoPP::Integer extendedGCD(CryptoPP::Integer a, CryptoPP::Integer b, CryptoPP::Integer &x, CryptoPP::Integer &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    CryptoPP::Integer x1, y1;
    CryptoPP::Integer gcd = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

/**
 * @brief 求模逆的函数
 * 
 * @param a 
 * @param m 
 * @return CryptoPP::Integer 
 */
CryptoPP::Integer modInverse(CryptoPP::Integer a, CryptoPP::Integer m) {
    CryptoPP::Integer x, y;
    CryptoPP::Integer g = extendedGCD(a, m, x, y);
    if (g != 1) {
        std::cout << "Inverse doesn't exist";
        return -1;
    } else {
        CryptoPP::Integer res = (x % m + m) % m;
        return res;
    }
}

/**
 * @brief 素性检验
 * 
 * @param n 被检验的数
 * @param iterations 迭代次数
 * @return true 
 * @return false 
 */
bool millerRabinTest(const CryptoPP::Integer &n, int iterations)
{
    if (n <= 1 || (n > 2 && n % 2 == 0))
        return false; // 小于等于1或者偶数（除了2）不是素数
    CryptoPP::Integer d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    CryptoPP::AutoSeededRandomPool rng;
    for (int i = 0; i < iterations; i++)
    {
        CryptoPP::Integer a = CryptoPP::Integer(rng, 2, n - 2);
        CryptoPP::Integer x = a_exp_b_mod_c(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        while (d != n - 1)
        {
            x = (x * x) % n;
            d *= 2;
            if (x == 1)
                return false;
            if (x == n - 1)
                break;
        }
        if (x != n - 1)
            return false;
    }
    return true;
}

/**
 * @brief 生成大素数
 * 
 * @param bits 指定位数
 * @return CryptoPP::Integer 
 */
CryptoPP::Integer generatePrime(int bits)
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::Integer rand_prime;
    bool isPrime = false;
    while (!isPrime)
    {
        // 生成一个2048位的随机数
        rand_prime = CryptoPP::Integer(rng, bits);

        isPrime = millerRabinTest(rand_prime, 10);
        // isPrime = CryptoPP::IsPrime(rand_prime);
    }
    return rand_prime;
}

void generateBigPrime()
{
    CryptoPP::Integer e(65537);
    CryptoPP::Integer p = generatePrime(2048);
    CryptoPP::Integer q = generatePrime(2048);
    CryptoPP::Integer n = (p - 1) * (q - 1);
    CryptoPP::Integer d = modInverse(e, n);

    // 使用CryptoPP::IsPrime算法判断是否为素数
    if (!CryptoPP::IsPrime(p))
    {
        std::cout << "p: " << p <<  std::endl;
    }
    if (!CryptoPP::IsPrime(q))
    {
        std::cout << "q: " << q <<  std::endl;
    }
    std::cout << "n: " << n << std::endl;

    std::cout << "gcd: " <<gcd(e, n) << std::endl;
    std::cout << "d: " << d << std::endl;
    if (!CryptoPP::IsPrime(d))
    {
        std::cout << "d is not prime!!" <<  std::endl;
    }
}

int main()
{
    // 生成密钥
    generateBigPrime();
}