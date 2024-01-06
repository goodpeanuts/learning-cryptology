/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-12 08:16:20
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 11:11:06
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

const size_t BUFFER_SIZE = 4 * 1024 * 1024; // 1kB

const char *welcom_msg = "=========== AES ===========\n"
                         "1. 测试字符串\n"
                         "2. 测试文件\n"
                         "3. 测试大文件\n"
                         "4. 退出\n"
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
    std::cout << "写入大小: " << out.size() << std::endl;
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
    // 避免受到回车的影响
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    out = aes.encrypt_CFB(in, k, iv);    //CFB
    // out = aes.EncryptECB(in, k);
    std::cout << "加密结果: " << std::endl;
    std::cout << "aa out: " << out.size() << std::endl;
    for (auto i : out)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;
    std::cout << "aa out: " << out.size() << std::endl;
    in = out;
    out = aes.decrypt_CFB(in, k, iv, true); //CFB
    // out = aes.DecryptECB(in, k, true);
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
    std::string key = "1234567890abcdef";

    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};
    std::vector<unsigned char> k{};

    std::cout << "=========== 文件加解密 ===========" << std::endl;
    std::cout << "输入文件名: ";
    std::cin >> filename;

    readFileToVector(filename, in);
    k.assign(key.begin(), key.end());
    out.assign(in.begin(), in.end());

    // 加密前
    // std::cout << "加密前: " << std::endl;
    // for (auto i : in)
    // {
    //     std::cout << i;
    // }
    // std::cout << std::endl;

    AES aes(AESMode::AES_128);
    out = aes.encrypt_CFB(in, k, iv);
    // std::cout << "加密结果: " << std::endl;
    // for (auto i : out)
    // {
    //     std::cout << std::hex << static_cast<int>(i);
    // }
    // std::cout << std::endl;

    in = out;
    out = aes.decrypt_CFB(in, k, iv, true);
    // std::cout << "解密结果: " << std::endl;
    // for (auto i : out)
    // {
    //     std::cout << i;
    // }
    std::cout << std::endl;
    writeVectorToFile("", out);
}

void buffer_en()
{
    std::string filename = "q.mkv";
    std::string output_filename = "mid";
    std::string key = "1234567890abcdef";
    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};
    std::vector<unsigned char> k{};

    std::cout << "=========== 大文件加密 ===========" << std::endl;

    k.assign(key.begin(), key.end());

    AES aes(AESMode::AES_128);

    std::ifstream input_file(filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    char *buffer = new char[BUFFER_SIZE];

    while (true)
    {
        input_file.read(buffer, BUFFER_SIZE);
        std::streamsize size = input_file.gcount();
        std::cout << "加密大小: " << size << std::endl;
        if (size == 0)
        {
            break;
        }
        in.assign(buffer, buffer + size);
        out = aes.encrypt_CFB(in, k, iv);
        output_file.write(reinterpret_cast<const char *>(out.data()), out.size());
    }

    delete[] buffer;
    input_file.close();
    output_file.close();
}

void buffer_de()
{
    std::string input_filename = "mid";
    std::string output_filename = "a.mkv";
    std::string key = "1234567890abcdef";
    std::vector<unsigned char> iv = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};
    std::vector<unsigned char> k{};

    std::cout << "=========== 大文件解密 ===========" << std::endl;

    k.assign(key.begin(), key.end());

    AES aes(AESMode::AES_128);

    std::ifstream input_file(input_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    char *buffer = new char[BUFFER_SIZE];

    while (true)
    {
        input_file.read(buffer, BUFFER_SIZE);
        std::streamsize size = input_file.gcount();
        std::cout << "解密大小: " << size << std::endl;
        if (size == 0)
        {
            break;
        }
        in.assign(buffer, buffer + size);
        if (size < BUFFER_SIZE)
        {
            out = aes.decrypt_CFB(in, k, iv, true);
        }
        else
        {
            out = aes.decrypt_CFB(in, k, iv, false);
        }
        output_file.write(reinterpret_cast<const char *>(out.data()), out.size());
    }

    delete[] buffer;
    input_file.close();
    output_file.close();
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
            buffer_en();
            buffer_de();
            break;
        case 4:
            return 0;
        default:
            std::cout << "输入错误!" << std::endl;
            std::cout << "请重新选择: ";
            std::cin >> op;
            break;
        }
    }
}