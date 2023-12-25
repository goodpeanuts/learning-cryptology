/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-12 08:16:20
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-25 23:09:15
 * @FilePath: /learning-cryptology/aes/aes.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include "aes.h"

const char *welcom_msg = "=========== AES ===========\n"
                         "1. 测试字符串\n"
                         "2. 测试文件\n"
                         "3. 退出\n"
                         "==========================\n"
                         "请选择:";

void readFileToVector(const std::string &filename, std::vector<unsigned char> &in)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    // 读取文件到 in 中
    in = std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void writeVectorToFile(const std::string &filename, const std::vector<unsigned char> &out)
{
    std::string outFilename = filename + "bak";
    std::ofstream file(outFilename, std::ios::binary);
    if (!file)
    {
        std::cerr << "无法打开文件: " << outFilename << std::endl;
        return;
    }

    // 写入 out 到文件中
    file.write(reinterpret_cast<const char *>(out.data()), out.size());
    std::cout << "写入文件: " << outFilename << std::endl;
}

void testString()
{
    std::string encrypt_text{};
    std::string plain_text{};
    std::string key{};

    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};
    std::vector<unsigned char> k{};

    std::cout << "=========== 字符串加密 ===========" << std::endl;
    std::cout << "输入明文: ";
    std::getline(std::cin, plain_text);
    std::cout << "输入密钥: ";
    std::cin >> key;

    in.assign(plain_text.begin(), plain_text.end());
    k.assign(key.begin(), key.end());
    out.assign(plain_text.begin(), plain_text.end());

    // 加密前
    std::cout << "加密前: " << std::endl;
    for (auto i : in)
    {
        std::cout << i;
    }
    std::cout << std::endl;

    AES aes(AESMode::AES_128);
    out = aes.encrypt_CFB(in, k, iv);
    std::cout << "加密结果: " << std::endl;
    for (auto i : out)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;

    in = out;
    out = aes.decrypt_CFB(in, k, iv);
    std::cout << "解密结果: " << std::endl;
    for (auto i : out)
    {
        std::cout << i;
    }
    std::cout << std::endl;
}

void testFile()
{
    std::string filename{};
    std::string encrypt_text{};
    std::string plain_text{};
    std::string key{};

    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};
    std::vector<unsigned char> k{};

    std::cout << "=========== 文件加密 ===========" << std::endl;
    std::cout << "输入文件名: ";
    std::cin >> filename;
    std::cout << "输入密钥: ";
    std::cin >> key;

    readFileToVector(filename, in);
    k.assign(key.begin(), key.end());
    out.assign(in.begin(), in.end());

    // 加密前
    std::cout << "加密前: " << std::endl;
    for (auto i : in)
    {
        std::cout << i;
    }
    std::cout << std::endl;

    AES aes(AESMode::AES_128);
    out = aes.encrypt_CFB(in, k, iv);
    std::cout << "加密结果: " << std::endl;
    for (auto i : out)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;

    in = out;
    out = aes.decrypt_CFB(in, k, iv);
    std::cout << "解密结果: " << std::endl;
    for (auto i : out)
    {
        std::cout << i;
    }
    std::cout << std::endl;
    writeVectorToFile("", out);
}
// 1234567890abcdef
int main()
{
    int op{};
    while (true)
    {
        std::cout << welcom_msg << std::endl;
        std::cin >> op;
        switch (op)
        {
        case 1:
            testString();
            break;
        case 2:
            testFile();
            break;
        case 3:
            return 0;
        default:
            std::cout << "输入错误!" << std::endl;
            std::cout << "请重新选择: ";
            std::cin >> op;
            break;
        }
    }
}