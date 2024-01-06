/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-11 15:25:41
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 13:02:58
 * @FilePath: /learning-cryptology/sequence_cipher/lsrf.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

int feedback(const std::string &reg, const std::vector<int> &fb)
{
    int res = reg[fb[0] - 1] - '0';
    for (size_t i = 1; i < fb.size(); ++i)
    {
        res ^= reg[fb[i] - 1] - '0';
    }
    return res;
}

std::pair<std::vector<char>, std::vector<std::string>> lfsr(const std::vector<int> &p)
{
    int reg_len = *std::max_element(p.begin(), p.end());
    std::string shift_reg(reg_len, '0');
    shift_reg[0] = '1';
    std::vector<std::string> regs{shift_reg};
    std::vector<char> output_;
    for (int i = 0; i < (1 << reg_len) - 1; ++i)
    {
        output_.push_back(shift_reg.back());
        char input_ = '0' + feedback(shift_reg, p);
        shift_reg = input_ + shift_reg.substr(0, reg_len - 1);
        regs.push_back(shift_reg);
    }
    return {output_, regs};
}

int main()
{
    // 16 5 3 2
    std::cout << "请输入本原多项式\n";
    std::string num_str;
    std::getline(std::cin, num_str);
    std::vector<int> num;
    size_t pos = 0;
    while ((pos = num_str.find(' ')) != std::string::npos)
    {
        num.push_back(std::stoi(num_str.substr(0, pos)));
        num_str.erase(0, pos + 1);
    }
    num.push_back(std::stoi(num_str));

    auto [outputs, regs] = lfsr(num);

    for (size_t i = 0; i < regs.size(); ++i)
    {
        std::cout << "第" << i << "次: " << regs[i] << "\n";
    }
    std::cout << "序列周期为: " << regs.size() - 1 << "\n";

    std::ofstream f("regs.txt");
    for (const auto &i : regs)
    {
        f << i << "\n";
    }
    return 0;
}