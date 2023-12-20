/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-12 08:16:20
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-13 16:07:00
 * @FilePath: /learning-cryptology/aes/aes.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <iostream>
#include "aes.h"

int main()
{
    std::string encrypt_text{};
    std::string plain_text{};
    std::string key {};

    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in {};
    std::vector<unsigned char> out {};
    std::vector<unsigned char> k {};

    std::cout << "=========== AES ===========" << std::endl;
    std::cout << "输入明文: ";
    std::getline(std::cin, plain_text);
    std::cout << "输入密钥: ";
    std::cin >> key;

    in.assign(plain_text.begin(), plain_text.end());
    k.assign(key.begin(), key.end());
    out.assign(plain_text.begin(), plain_text.end());

    // 加密前
    std::cout << "加密前: " << std::endl;
    for (auto i : in) {
        std::cout << i;
    }
    std::cout << std::endl;

    AES aes(AESMode::AES_128);
    out = aes.encrypt_CFB(in, k, iv);
    std::cout << "加密结果: " << std::endl;
    for (auto i : out) {
        std::cout << std::hex << static_cast<int>(i) ;
    }
    std::cout << std::endl;

    in = out;
    out = aes.decrypt_CFB(in, k, iv);
    std::cout << "解密结果: " << std::endl;
    for (auto i : out) {
        std::cout << i;
    }
    std::cout << std::endl;
}