/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-26 23:38:47
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-27 14:23:21
 * @FilePath: /learning-cryptology/rsa/gen_rsa_key.cpp
 * @Description: rsa加解密demo以及生成密钥对
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include "rsa.h"
void test_string()
{
    RSA rsa(2048);
    std::string input = "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world ";

    CryptoPP::Integer pub_key = rsa.get_public_key();
    CryptoPP::Integer pri_key = rsa.get_private_key();
    CryptoPP::Integer n = rsa.get_n();
    size_t bits = n.BitCount();

    std::cout << "------- 明文 -------" << std::endl;
    std::cout << "plain_text: " << input << std::endl;

    std::cout << "------- 加密 -------" << std::endl;
    CryptoPP::Integer cipher_text = a_exp_b_mod_c(rsa.encode_string(input), pub_key, n);
    std::cout << "cipher_text: " << cipher_text << std::endl;

    std::cout << "------- 解密 -------" << std::endl;
    std::string plain_text = rsa.decode_string(a_exp_b_mod_c(cipher_text, pri_key, n));
    std::cout << "plain_text: " << plain_text << std::endl;

    assert(input == plain_text);

    std::cout << "------- 公钥 -------" << std::endl;
    std::cout << pub_key << std::endl;
    std::cout << "------- 私钥 -------" << std::endl;
    std::cout << pri_key << std::endl;
    std::cout << "------- n -------" << std::endl;
    std::cout << n << std::endl;
}

int main()
{
    test_string();
}