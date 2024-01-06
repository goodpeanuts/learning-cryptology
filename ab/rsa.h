/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-25 15:51:45
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 13:21:22
 * @FilePath: /learning-cryptology/ab/rsa.h
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <cassert>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>

class RSA
{
private:
    CryptoPP::Integer e, d, n;
    static const size_t pubKeyLen = 256; // 公钥长度

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
    CryptoPP::Integer generateBigPrime(int bits)
    {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::Integer rand_prime;
        bool isPrime = false;
        while (!isPrime)
        {
            rand_prime = CryptoPP::Integer(rng, bits);

            if (rand_prime.BitCount() != bits)
                continue;

            isPrime = millerRabinTest(rand_prime, 10);
        }
        assert(CryptoPP::IsPrime(rand_prime));
        assert(rand_prime.BitCount() == bits);
        return rand_prime;
    }

    /**
     * @brief Get the public key object
     *
     * @param n
     * @param bitSize
     * @return CryptoPP::Integer
     */
    CryptoPP::Integer get_public_key(const CryptoPP::Integer &n)
    {
        CryptoPP::AutoSeededRandomPool rng;
        while (true)
        {
            CryptoPP::Integer e(rng, pubKeyLen);
            if (CryptoPP::GCD(e, n) == 1)
            {
                return e;
            }
        }
    }

    /**
     * @brief
     *
     * @param key_size
     */
    void generate_keys(size_t key_size)
    {
        CryptoPP::Integer p = generateBigPrime(key_size);
        CryptoPP::Integer q = generateBigPrime(key_size);
        CryptoPP::Integer fn = (p - 1) * (q - 1);
        this->n = p * q;
        this->e = get_public_key(fn);
        this->d = modInverse(e, fn);
        assert(p != q);
        assert(CryptoPP::IsPrime(p));
        assert(CryptoPP::IsPrime(q));
        assert(gcd(e, fn) == 1);
        assert((e * d) % fn == 1);
    }

public:
    RSA(int key_size)
    {
        generate_keys(key_size);
        // std::cout << "n: " << n << std::endl;
        // std::cout << "e: " << e << std::endl;
        // std::cout << "d: " << d << std::endl;

        // 将大数转换为字节串
        // CryptoPP::byte eBytes[e.MinEncodedSize()];
        // e.Encode(eBytes, sizeof(eBytes));
        // CryptoPP::byte dBytes[d.MinEncodedSize()];
        // d.Encode(dBytes, sizeof(dBytes));

        // 将字节串编码为Base64格式
        // std::string eBase64, dBase64;
        // CryptoPP::StringSource(eBytes, sizeof(eBytes), true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(eBase64)));
        // CryptoPP::StringSource(dBytes, sizeof(dBytes), true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(dBase64)));

        // 打印Base64格式的大数
        // std::cout << "e: " << eBase64 << std::endl;
        // std::cout << "d: " << dBase64 << std::endl;
    }
    CryptoPP::Integer get_public_key()
    {
        return this->e;
    }
    CryptoPP::Integer get_private_key()
    {
        return this->d;
    }
    CryptoPP::Integer get_n()
    {
        return this->n;
    }

    void set_public_key(const CryptoPP::Integer &e)
    {
        this->e = e;
    }
    void set_private_key(const CryptoPP::Integer &d)
    {
        this->d = d;
    }
    void set_n(const CryptoPP::Integer &n)
    {
        this->n = n;
    }

    static CryptoPP::Integer encode_string(const std::string &str)
    {
        CryptoPP::Integer num;
        num.Decode((const CryptoPP::byte *)str.data(), str.size());
        return num;
    }
    static std::string decode_string(const CryptoPP::Integer &num)
    {
        // 将CryptoPP::Integer对象转换为字节串
        size_t size = num.MinEncodedSize();
        CryptoPP::byte *buffer = new CryptoPP::byte[size];
        num.Encode(buffer, size);

        // 将字节串转换为std::string
        std::string str((const char *)buffer, size);
        return str;
    }
};