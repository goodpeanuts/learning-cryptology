/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 07:21:22
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-20 08:36:59
 * @FilePath: /learning-cryptology/rsa/sptest.cpp
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
#include <chrono>

// 素性检验
bool MillerRabinTest(const CryptoPP::Integer& n, int iterations) {
    if (n <= 1 || (n > 2 && n % 2 == 0)) {
        return false;  // 小于等于1或者偶数（除了2）不是素数
    }

    CryptoPP::Integer d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }

    CryptoPP::AutoSeededRandomPool rng;
    for (int i = 0; i < iterations; i++) {
        CryptoPP::Integer a = CryptoPP::Integer(rng, 2, n - 2);
        CryptoPP::Integer x = a_exp_b_mod_c(a, d, n);

        if (x == 1 || x == n - 1) {
            continue;
        }

        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) {
                return false;
            }
            if (x == n - 1) {
                break;
            }
        }

        if (x != n - 1) {
            return false;
        }
    }

    return true;
}


double generateKey()
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::Integer p;

    bool isPrime = false;
    long long count {};
    // 获取当前时间
    auto start = std::chrono::high_resolution_clock::now();
    while(!isPrime)
    {
        // 生成一个2048位的随机数
        p = CryptoPP::Integer(rng, 2048);

        isPrime = MillerRabinTest(p, 10);
        // isPrime = CryptoPP::IsPrime(p);
    
        if (isPrime) {
            // std::cout << p << std::endl;
            // std::cout << "count: " << count << std::endl;
        } else {
            count++;
        }
    }
    //计算耗时
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    double cost = diff.count();
    std::cout << "Time taken: " << diff.count() << " ms\n";

    // 使用CryptoPP::IsPrime算法判断是否为素数
    isPrime = CryptoPP::IsPrime(p);
    if (isPrime) {
        return cost / count;
    } else {
        std::cout << "p is not prime" << std::endl;
        return 0;
    }   
}

int main()
{
    double cost {};
    size_t count = 10;
    while (count --)
    {
        cost += generateKey();
    }
    std::cout << "cost: " << cost / 10 << std::endl; 
}