/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 07:45:34
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-20 07:59:18
 * @FilePath: /learning-cryptology/rsa/check.cpp
 * @Description: 用于检验
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <iostream>

int main() {
    CryptoPP::AutoSeededRandomPool rng;

    bool isPrime;
    long long count {};

    while (!isPrime)
    {
        // 生成一个2048位的随机数
        CryptoPP::Integer p(rng, 2048);

        // 使用Miller-Rabin算法判断是否为素数
        isPrime = CryptoPP::IsPrime(p);

        if (isPrime) {
            std::cout << "p is prime" << std::endl;
            std::cout << p << std::endl;
            std::cout << "count: " << count << std::endl;
        } else {
            // std::cout << "p is not prime" << std::endl;
            count++;
        }
    }

    return 0;
}