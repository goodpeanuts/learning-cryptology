/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 07:21:22
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-27 14:21:52
 * @FilePath: /learning-cryptology/rsa/test.cpp
 * @Description: 测试素性检验算法的速度
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <cassert>
#include <cstring>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/rsa.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/files.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <chrono>
#define debug(x) std::cerr << #x << ' ' << '=' << ' ' << x << std::endl

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
CryptoPP::Integer extendedGCD(CryptoPP::Integer a, CryptoPP::Integer b, CryptoPP::Integer &x, CryptoPP::Integer &y)
{
    if (b == 0)
    {
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
CryptoPP::Integer modInverse(CryptoPP::Integer a, CryptoPP::Integer m)
{
    CryptoPP::Integer x, y;
    CryptoPP::Integer g = extendedGCD(a, m, x, y);
    if (g != 1)
    {
        std::cout << "Inverse doesn't exist";
        return -1;
    }
    else
    {
        CryptoPP::Integer res = (x % m + m) % m;
        return res;
    }
}


// 素性检验
bool millerRabinTest(const CryptoPP::Integer &n, int iterations)
{
    if (n <= 1 || (n > 2 && n % 2 == 0))
    {
        return false; // 小于等于1或者偶数（除了2）不是素数
    }

    CryptoPP::Integer d = n - 1;
    while (d % 2 == 0)
    {
        d /= 2;
    }

    CryptoPP::AutoSeededRandomPool rng;
    for (int i = 0; i < iterations; i++)
    {
        CryptoPP::Integer a = CryptoPP::Integer(rng, 2, n - 2);
        CryptoPP::Integer x = a_exp_b_mod_c(a, d, n);

        if (x == 1 || x == n - 1)
        {
            continue;
        }

        while (d != n - 1)
        {
            x = (x * x) % n;
            d *= 2;

            if (x == 1)
            {
                return false;
            }
            if (x == n - 1)
            {
                break;
            }
        }

        if (x != n - 1)
        {
            return false;
        }
    }

    return true;
}

double generateSpeedTest(int bits)
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::Integer rand_prime;

    bool isPrime = false;
    long long count{};
    // 获取当前时间
    auto start = std::chrono::high_resolution_clock::now();
    while (!isPrime)
    {
        // 生成一个2048位的随机数
        rand_prime = CryptoPP::Integer(rng, bits);

        if (rand_prime.BitCount() != bits)
            continue;

        isPrime = millerRabinTest(rand_prime, 10);

        if (!isPrime)
            count++;
    }
    // 计算耗时
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    double cost = diff.count();
    std::cout << "Time taken: " << diff.count() << " ms\n";

    // 使用CryptoPP::IsPrime算法判断是否为素数
    assert(CryptoPP::IsPrime(rand_prime));
    assert(rand_prime.BitCount() == bits);
    return 0;
}

CryptoPP::Integer generateBigPrime(int bits)
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::Integer rand_prime;

    bool isPrime = false;
    long long count{};
    while (!isPrime)
    {
        // 生成一个2048位的随机数
        rand_prime = CryptoPP::Integer(rng, bits);

        if (rand_prime.BitCount() != bits)
            continue;

        isPrime = millerRabinTest(rand_prime, 10);

        if (isPrime)
        {
            std::cout << rand_prime << std::endl;
            std::cout << "循环次数: " << count << std::endl;
        }
        else
        {
            count++;
        }
    }
    // 输出 p 的二进制位数
    std::cout << "位数： " << rand_prime.BitCount() << std::endl;

    assert(rand_prime.BitCount() == bits);
    assert(CryptoPP::IsPrime(rand_prime));
    return rand_prime;
}

CryptoPP::Integer get_public_key(const CryptoPP::Integer& n, size_t bitSize)
{
    CryptoPP::AutoSeededRandomPool rng;
    while (true) {
        CryptoPP::Integer e(rng, bitSize);
        if (CryptoPP::GCD(e, n) == 1) {
            return e;
        }
    }
}

void generate_keys(size_t key_size)
{
    CryptoPP::Integer p = generateBigPrime(key_size);
    CryptoPP::Integer q = generateBigPrime(key_size);
    CryptoPP::Integer n = p * q;
    CryptoPP::Integer fn = (p - 1) * (q - 1);
    CryptoPP::Integer e = get_public_key(fn, 128);
    CryptoPP::Integer d = modInverse(e, fn);
    // std::cout << "p: " << p << std::endl;
    // std::cout << "q: " << q << std::endl;
    // std::cout << "n: " << n << std::endl;
    // std::cout << "e: " << e << std::endl;
    // std::cout << "d: " << d << std::endl;
    std::cout << "gcd: " << gcd(e, fn) << std::endl;
    std::cout << "d * e % n: " << d * e % fn << std::endl;
}


int main()
{
    // 速度测试
    // double cost{};
    // size_t count = 10;
    // while (count--)
    // {
    //     cost += generateSpeedTest(2048);
    // }
    // std::cout << "cost: " << cost / 10 << std::endl;

    // std::cout << "------------------" << std::endl;

    // 生成密钥轮数测试
    // generateBigPrime(2048);
    generate_keys(2048);
}