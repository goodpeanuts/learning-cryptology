/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-11 03:19:32
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 15:54:32
 * @FilePath: /learning-cryptology/classical/vigenere/vigenere.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>
#include <limits>
#include <cmath>
#include <iomanip>

// 计算密文的重合指数
double index_of_coincidence(const std::vector<char> &ciphertext)
{
    std::vector<int> freq(26);
    int n = 0;

    for (char c : ciphertext)
    {
        if (std::isalpha(c))
        {
            ++freq[std::tolower(c) - 'a'];
            ++n;
        }
    }

    double sum = 0;
    for (int f : freq)
    {
        sum += f * (f - 1);
    }

    return sum / (n * (n - 1));
}

// 计算密文的平均重合指数
double avg_ic(const std::string &origin, size_t period)
{
    std::vector<std::vector<char>> vec{};
    vec.resize(period);

    // 确保密文长度不超过size_t的最大值导致 i 溢出
    assert(origin.length() < std::numeric_limits<size_t>::max());

    for (size_t i = 0; i < origin.length(); ++i)
    {
        vec.at(i % period).push_back(origin.at(i));
    }

    double avg_ic{0.0};
    for (size_t i = 0; i < period; ++i)
    {
        avg_ic += index_of_coincidence(vec.at(i));
    }

    return avg_ic / period;
}

// 获取不同周期的密文的重合指数
size_t get_ic(size_t period_limit, std::string origin)
{
    std::vector<std::pair<double, size_t>> ic{};

    for (size_t i = 1; i <= period_limit; ++i)
    {
        double curr_ic = avg_ic(origin, i);
        ic.push_back(std::make_pair(curr_ic, i));
    }
    // 重合指数排序
    std::sort(ic.begin(), ic.end(), [](const std::pair<double, size_t> &lhs, const std::pair<double, size_t> &rhs)
              { return lhs.first > rhs.first; });

    std::cout << std::endl
              << "周期"
              << "\t重合指数" << std::endl;
    for (const auto &each : ic)
        std::cout << each.second << "\t" << each.first << std::endl;

    return ic.at(0).second;
}

// 计算各凯撒分组中不同偏移量下的拟重合指数
double chi_square(const std::vector<char> &vec, size_t key)
{
    std::vector<int> freq(26);
    for (char c : vec)
    {
        if (isalpha(c))
        {
            c = tolower(c);
            c = (c - 'a' + (26 - key)) % 26 + 'a';
            freq[c - 'a']++;
        }
    }
    std::cout << "\t";

    // 英文文本中每个字母的预期频率
    const std::vector<double> expected_freq = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702,
                                               0.02228, 0.02015, 0.06094, 0.06966, 0.00153,
                                               0.00772, 0.04025, 0.02406, 0.06749, 0.07507,
                                               0.01929, 0.00095, 0.05987, 0.06327, 0.09056,
                                               0.02758, 0.00978, 0.02360, 0.00150, 0.01974,
                                               0.00074};
    // 确保所有期望的总和为 1.0
    double sum{0.0};
    for (const auto &each : expected_freq)
        sum += each;
    assert(std::abs(1.0 - sum) < 1e-4);

    int n = vec.size();
    double chi_square = 0;
    for (int i = 0; i < freq.size(); i++)
    {
        double observed_freq = static_cast<double>(freq[i]) / n;
        chi_square += pow(observed_freq - expected_freq[i], 2) / expected_freq[i];
    }

    return chi_square;
}

// 获取可能的密钥
std::string get_possible_key(std::string origin, size_t period)
{

    // 确保密文长度不超过size_t的最大值导致 i 溢出
    assert(origin.length() < std::numeric_limits<size_t>::max());

    std::string possible_key{""}; //
    std::vector<std::vector<double>> chi_tables{};
    std::vector<std::vector<char>> possible_keys{};
    chi_tables.resize(period);
    possible_keys.resize(period);

    std::cout << std::endl;

    // 对于每个字串进行凯撒破解
    for (size_t i = 0; i < period; ++i)
    {

        std::vector<char> vec{};
        for (size_t j = i; j < origin.length(); j += period)
        {
            vec.push_back(origin.at(j));
        }

        // 对26种偏移下的拟重合指数
        char pos{'a'};
        std::vector<std::pair<char, double>> possibility_array{};

        // 计算每个分组的拟重合指数
        for (size_t j = 0; j < 26; ++j)
        {
            double curr_chi_square = chi_square(vec, j);
            chi_tables.at(i).push_back(curr_chi_square);
            possibility_array.push_back(std::make_pair(pos++, curr_chi_square));
        }

        // 对每个字串可能的密钥降序排序
        std::sort(possibility_array.begin(), possibility_array.end(),
                  [](const std::pair<char, double> &lhs, const std::pair<char, double> &rhs)
                  {
                      return lhs.second < rhs.second;
                  });

        for (const auto &ch : possibility_array)
            possible_keys.at(i).push_back(ch.first);

        possible_key += possibility_array.at(0).first;
    }

    // 输出拟重合指数表格

    std::cout << "==================";
    for (int i = 0; i < 15 * period; i++)
        std::cout << "=";
    std::cout << std::endl;
    std::cout << "选定重合指数最大的周期计算各分组不同偏移下的重合指数 " << std::endl
              << std::endl;
    std::cout << std::left << std::setw(12) << "序号" << std::setw(10) << "字母";
    for (size_t i = 0; i < period; ++i)
        std::cout << std::setw(19) << "    第" + std::to_string(i + 1) + "组";
    std::cout << std::endl;

    char letter{'a'};
    for (int i = 0; i < 26; ++i)
    {
        std::cout << std::setw(12) << i << std::setw(10) << letter++ << "";
        for (size_t j = 0; j < period; ++j)
        {
            std::cout << std::setw(17) << chi_tables.at(j).at(i);
        }
        std::cout << std::endl;
    }
    std::cout << "==================";
    for (int i = 0; i < 15 * period; i++)
        std::cout << "=";
    std::cout << std::endl
              << std::endl;

    std::cout << "密钥字确定(可能性降序排序): " << std::endl;
    std::cout << "=======================================================" << std::endl;

    // 输出各分组可能密钥排序
    for (size_t i = 0; i < period; ++i)
    {
        std::cout << i + 1 << "#  "
                  << "";
        for (const auto &ch : possible_keys.at(i))
            std::cout << ch << " ";
        std::cout << std::endl
                  << "=======================================================" << std::endl;
    }

    std::cout << std::endl
              << "key: " << possible_key << std::endl;
    return possible_key;
}

// 解密
std::string vigenere_decrypt(std::string ciphertext, std::string key)
{
    std::transform(ciphertext.begin(), ciphertext.end(),
                   ciphertext.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(key.begin(), key.end(),
                   key.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::string plaintext{};
    for (int i = 0; i < ciphertext.size(); ++i)
    {
        plaintext += (ciphertext[i] - key[i % key.size()] + 26) % 26 + 'A';
    }
    return plaintext;
}

// 加密
std::string vigenere_encrypt(std::string plaintext, std::string key)
{
    std::transform(plaintext.begin(), plaintext.end(),
                   plaintext.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(key.begin(), key.end(),
                   key.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::string ciphertext{};
    for (int i = 0; i < plaintext.size(); ++i)
    {
        ciphertext += (plaintext[i] + key[i % key.size()] - 2 * 'a') % 26 + 'A';
    }
    return ciphertext;
}

void encrypt()
{
    std::string plaintext{};
    std::string key{};
    std::string ciphertext{};

    std::cout << "请输入明文: ";
    std::cin >> plaintext;
    std::cout << "请输入密钥: ";
    std::cin >> key;

    ciphertext = vigenere_encrypt(plaintext, key);
    std::cout << "密文为: " << ciphertext << std::endl;
}

void decrypt()
{
    std::string ciphertext{};
    std::string key{};
    std::string plaintext{};

    std::cout << "请输入密文: ";
    std::cin >> ciphertext;
    std::cout << "请输入密钥: ";
    std::cin >> key;

    plaintext = vigenere_decrypt(ciphertext, key);
    std::cout << "明文为: " << plaintext << std::endl;
}

void cracker()
{

    // std::string ciphertext {};
    std::string decrypt_key{};
    std::string plaintext{};
    size_t period_limit{};
    size_t period{};

    // TODO: test
    std::string ciphertext{"CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBWRVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAKLXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELXVRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHRZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJTAMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBIPEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHPWQAIIWXNRMGWOIIFKEE"};

    // std::cout << "请输入密文: ";
    // std::cin >> ciphertext;

    std::cout << "输入周期限制: ";
    std::cin >> period_limit;

    period = get_ic(period_limit, ciphertext);

    decrypt_key = get_possible_key(ciphertext, period);

    plaintext = vigenere_decrypt(ciphertext, decrypt_key);
    std::cout << "明文为: " << plaintext << std::endl;
}

int main()
{
    int mode{};

    std::cout << "1 加密 " << std::endl;
    std::cout << "2 解密 " << std::endl;
    std::cout << "3 破解 " << std::endl;
    std::cout << "请选择模式: ";
    std::cin >> mode;
    while (1)
    {
        switch (mode)
        {
        case 0:
            return 0;
        case 1:
            encrypt();
            break;
        case 2:
            decrypt();
            break;
        case 3:
            cracker();
            break;
        default:
            std::cout << "输入错误, 请重新输入" << std::endl;
            break;
        }

        std::cout << "1 加密 " << std::endl;
        std::cout << "2 解密 " << std::endl;
        std::cout << "3 破解 " << std::endl;
        std::cout << std::endl
                  << "请选择模式: ";
        std::cin >> mode;
    }
}