/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-11 03:19:32
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-11 05:07:31
 * @FilePath: /learning-cryptology/classical/vigenere/vigenereCracker.cpp
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

// 计算密文的重合指数
double index_of_coincidence(const std::vector<char> &ciphertext) {
    std::vector<int> freq(26);
    int n = 0;

    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            ++freq[std::tolower(c) - 'a'];
            ++n;
        }
    }

    double sum = 0;
    for (int f : freq) {
        sum += f * (f - 1);
    }

    return sum / (n * (n - 1));
}

// 计算密文的平均重合指数
double avg_ic(const std::string &origin, size_t period) {
    std::vector<std::vector<char>> vec {}; 
    vec.resize(period); 

    // ensure that origin.length() will not make var i overflow
    assert(origin.length() < std::numeric_limits<size_t>::max()); 

    for (size_t i = 0; i < origin.length(); ++i) {
        vec.at(i % period).push_back(origin.at(i)); 
    }

    double avg_ic {0.0}; 
    for (size_t i = 0; i < period; ++i) {
        avg_ic += index_of_coincidence(vec.at(i)); 
    }

    return avg_ic / period; 
}

// 获取不同周期的密文的重合指数
void get_ic(size_t period_limit, std::string origin) {
    std::cout << std::endl << "period" << "\tindex of coincidence" << std::endl; 
    for (size_t i = 1; i <= period_limit; ++i) {
        std::cout << i << "\t" << avg_ic(origin, i) << std::endl; 
    }
}

// 计算各凯撒分组中不同偏移量下的拟重合指数
double chi_square(const std::vector<char> &vec, size_t key) {
    std::vector<int> freq(26);
    for (char c : vec) {
        if (isalpha(c)) {
            c = tolower(c);
            c = (c - 'a' + (26 - key)) % 26 + 'a';
            freq[c - 'a']++;
        }
    }
    std::cout << "\t"; 

    // The expected frequency of each letter in English text is taken from Wikipedia.
    // https://en.wikipedia.org/wiki/Letter_frequency
    const std::vector<double> expected_freq = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702,
                                               0.02228, 0.02015, 0.06094, 0.06966, 0.00153,
                                               0.00772, 0.04025, 0.02406, 0.06749, 0.07507,
                                               0.01929, 0.00095, 0.05987, 0.06327, 0.09056,
                                               0.02758, 0.00978, 0.02360, 0.00150, 0.01974,
                                               0.00074};
    // ensure that summation of expected_freq is 1.0
    double sum {0.0}; 
    for (const auto & each : expected_freq) sum += each; 
    assert(std::abs(1.0 - sum) < 1e-4); 

    int n = vec.size();
    double chi_square = 0;
    for (int i = 0; i < freq.size(); i++) {
        double observed_freq = static_cast<double>(freq[i]) / n;
        chi_square += pow(observed_freq - expected_freq[i], 2) / expected_freq[i];
    }

    return chi_square;
}

// 获取可能的密钥
std::string get_possible_key(std::string origin, size_t period) {
    // ensure that origin.length() will not make var i overflow
    assert(origin.length() < std::numeric_limits<size_t>::max()); 

    std::string possible_key {""}; 

    std::cout << std::endl; 

    // do period time of caeser decipher
    for (size_t i = 0; i < period; ++i) {
        std::cout << "=======================================================" << std::endl; 
        std::cout << "In the Caeser cipher crack number " << i + 1 << "#: " << std::endl << std::endl; 
        std::cout << "Number" << "\t\tLetter" << "\t\tChi-Square" << std::endl; 
        
        // generate work vector for each caeser decipher
        std::vector<char> vec {}; 
        for (size_t j = i; j < origin.length(); j += period) {
            vec.push_back(origin.at(j)); 
        }

        // exhaust all possible key which need only 26 tries
        char pos {'a'}; 
        std::vector<std::pair<char, double>> possibility_array {};
        for (size_t j = 0; j < 26; ++j) {
            double curr_chi_square = chi_square(vec, j); 
            possibility_array.push_back(std::make_pair(pos, curr_chi_square)); 
            std::cout << j << "\t" << pos++ << "\t"; 
            std::cout << "\t" << curr_chi_square << std::endl; 
        }

        // sort from most possible to less possible
        // namely ascending sort of chi-square
        std::sort(possibility_array.begin(), possibility_array.end(), 
        [](const std::pair<char, double> &lhs, const std::pair<char, double> &rhs) {
            return lhs.second < rhs.second; 
        }); 

        std::cout << std::endl; 
        std::cout << "The " << i + 1 << "# digit of key (descending order by possibility): " << std::endl; 
        for (const auto & ch : possibility_array) std::cout << ch.first << " "; 
        possible_key += possibility_array.at(0).first; 

        std::cout << std::endl << "=======================================================" << std::endl; 
        std::cout << std::endl << std::endl; // begin next round
    }

    std::cout << std::endl << "Possible key: " << possible_key << std::endl; 
    return possible_key;
}

// 解密
std::string vigenere_decrypt(std::string ciphertext, std::string key) {
    std::transform(ciphertext.begin(), ciphertext.end(), 
                   ciphertext.begin(), [](unsigned char c){ return std::tolower(c); });
    std::transform(key.begin(), key.end(), 
                   key.begin(), [](unsigned char c){ return std::tolower(c); });
    std::string plaintext {};
    for (int i = 0; i < ciphertext.size(); ++i) {
        plaintext += (ciphertext[i] - key[i % key.size()] + 26) % 26 + 'A';
    }
    return plaintext;
}


int main() {
    std::string decrypt_key {};
    std::string plaintext {};

    // std::string ciphertext {};
    // std::cout << "请输入密文： "
    // std::cin  >> origin;

    // TODO: test
    std::string ciphertext {"CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBWRVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAKLXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELXVRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHRZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJTAMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBIPEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHPWQAIIWXNRMGWOIIFKEE"};

    size_t period_limit {};
    std::cout << "输入周期限制: ";
    std::cin >> period_limit;

    get_ic(period_limit, ciphertext);

    // TODO: 通过ic返回的最佳周期判断周期
    decrypt_key = get_possible_key(ciphertext, period_limit);


    plaintext = vigenere_decrypt(ciphertext, decrypt_key);
    std::cout << "明文为: " << plaintext << std::endl;

}