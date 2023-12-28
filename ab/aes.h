/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-12 08:38:23
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-28 22:44:50
 * @FilePath: /learning-cryptology/ab/aes.h
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <random>
#include "aes_tables.h"

enum class AESMode
{
    AES_128,
    AES_192,
    AES_256
};

class AES
{
public:
    AES(AESMode mode)
    {
        switch (mode)
        {
        case AESMode::AES_128:
            Nk = 4;  // 密钥长度
            Nr = 10; // 轮数
            break;
        case AESMode::AES_192:
            Nk = 6;
            Nr = 12;
            break;
        case AESMode::AES_256:
            Nk = 8;
            Nr = 14;
            break;
        default:
            break;
        }
    }

    static constexpr unsigned int Nb = 4;                                         // 分组长度
    static constexpr unsigned int blockByteslen = 4 * Nb * sizeof(unsigned char); // 每组位数128

    unsigned int Nk;
    unsigned int Nr;

    /**
     * @brief   字节变换
     *
     * @param state    状态矩阵
     */
    void sub_bytes(unsigned char state[4][Nb])
    {
        unsigned int i, j;
        unsigned char t;
        for (i = 0; i < 4; ++i)
        {
            for (j = 0; j < Nb; ++j)
            {
                t = state[i][j];
                state[i][j] = sbox[t / 16][t % 16];
            }
        }
    }

    /**
     * @brief 对 i 行进行左移 n 位
     *
     * @param state 状态矩阵
     * @param i     行号
     * @param n     位移位数
     */
    void shift_row(unsigned char state[4][Nb], unsigned int i, unsigned int n)
    {
        unsigned char tmp[Nb];
        for (unsigned int j = 0; j < Nb; j++)
        {
            tmp[j] = state[i][(j + n) % Nb];
        }
        memcpy(state[i], tmp, Nb * sizeof(unsigned char));
    }

    /**
     * @brief 行位移
     *
     * @param state 状态矩阵
     */
    void shift_rows(unsigned char state[4][Nb])
    {
        shift_row(state, 1, 1);
        shift_row(state, 2, 2);
        shift_row(state, 3, 3);
    }

    /**
     * @brief   xtime运算
     *
     * @param b
     * @return unsigned char
     */
    unsigned char xtime(unsigned char b)
    {
        return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
    }

    /**
     * @brief 列混合
     *
     * @param state 状态矩阵
     */
    void mix_columns(unsigned char state[4][Nb])
    {
        unsigned char temp_state[4][Nb];

        // 将中间态状态矩阵清零
        for (size_t i = 0; i < 4; ++i)
            memset(temp_state[i], 0, 4);

        for (size_t i = 0; i < 4; i++)
            for (size_t k = 0; k < 4; k++)
                for (size_t j = 0; j < 4; j++)
                {
                    if (CMDS[i][k] == 1)
                        temp_state[i][j] ^= state[k][j];
                    else
                        temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
                }

        for (size_t i = 0; i < 4; ++i)
        {
            memcpy(state[i], temp_state[i], 4);
        }
    }

    /**
     * @brief 轮密钥加
     *
     * @param state 需要被加密的分组
     * @param key   密钥
     */
    void add_round_key(unsigned char state[4][Nb], unsigned char *key)
    {
        unsigned int i, j;
        for (i = 0; i < 4; i++)
            for (j = 0; j < Nb; j++)
                state[i][j] = state[i][j] ^ key[i + 4 * j];
    }

    /**
     * @brief T函数字循环
     *
     * @param w
     */
    void rot_word(unsigned char *w)
    {
        unsigned char tmp;
        unsigned int i;
        tmp = w[0];
        for (i = 0; i < 3; i++)
            w[i] = w[i + 1];
        w[3] = tmp;
    }

    /**
     * @brief T函数字节代换
     *
     * @param w 字节
     */
    void sub_word(unsigned char *w)
    {
        unsigned int i;
        for (i = 0; i < 4; i++)
            w[i] = sbox[w[i] / 16][w[i] % 16];
    }

    /**
     * @brief 轮常量生成
     *
     * @param w
     * @param n
     */
    void rcon(unsigned char *w, unsigned char n)
    {
        unsigned int i;
        unsigned char c = 1;
        for (i = 0; i < n - 1; i++)
            c = xtime(c);
        w[0] = c;
        w[1] = w[2] = w[3] = 0;
    }

    /**
     * @brief 字异或
     *
     * @param w1
     * @param w2
     * @param w3
     */
    void xor_words(unsigned char *w1, unsigned char *w2, unsigned char *w3)
    {
        unsigned int i;
        for (i = 0; i < 4; i++)
            w3[i] = w1[i] ^ w2[i];
    }

    /**
     * @brief       密钥扩展
     *
     * @param key   密钥
     * @param w     扩展后的密钥
     */
    void key_expansion(const unsigned char key[], unsigned char w[])
    {
        unsigned char temp[4];
        unsigned char rc[4];
        unsigned int i = 0;

        while (i < 4 * Nk)
        {
            w[i] = key[i];
            i++;
        }

        i = 4 * Nk;
        while (i < 4 * Nb * (Nr + 1))
        {
            temp[0] = w[i - 4 + 0];
            temp[1] = w[i - 4 + 1];
            temp[2] = w[i - 4 + 2];
            temp[3] = w[i - 4 + 3];

            if (i / 4 % Nk == 0)
            {
                rot_word(temp);
                sub_word(temp);
                rcon(rc, i / (Nk * 4));
                xor_words(temp, rc, temp);
            }
            else if (Nk > 6 && i / 4 % Nk == 4)
                sub_word(temp);

            w[i + 0] = w[i - 4 * Nk + 0] ^ temp[0];
            w[i + 1] = w[i - 4 * Nk + 1] ^ temp[1];
            w[i + 2] = w[i - 4 * Nk + 2] ^ temp[2];
            w[i + 3] = w[i - 4 * Nk + 3] ^ temp[3];
            i += 4;
        }
    }

    /**
     * @brief 逆字节变换
     *
     * @param state
     */
    void inv_sub_bytes(unsigned char state[4][Nb])
    {
        unsigned int i, j;
        unsigned char t;
        for (i = 0; i < 4; ++i)
            for (j = 0; j < Nb; ++j)
            {
                t = state[i][j];
                state[i][j] = inv_sbox[t / 16][t % 16];
            }
    }

    /**
     * @brief 逆行位移
     *
     * @param state
     */
    void inv_shift_rows(unsigned char state[4][Nb])
    {
        shift_row(state, 1, Nb - 1);
        shift_row(state, 2, Nb - 2);
        shift_row(state, 3, Nb - 3);
    }

    /**
     * @brief 逆列混合
     *
     * @param state
     */
    void inv_mix_columns(unsigned char state[4][Nb])
    {
        unsigned char temp_state[4][Nb];

        // 将中间态状态矩阵清零
        for (size_t i = 0; i < 4; ++i)
            memset(temp_state[i], 0, 4);

        for (size_t i = 0; i < 4; i++)
            for (size_t k = 0; k < 4; k++)
                for (size_t j = 0; j < 4; j++)
                {
                    if (CMDS[i][k] == 1)
                        temp_state[i][j] ^= state[k][j];
                    else
                        temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
                }

        for (size_t i = 0; i < 4; ++i)
            memcpy(state[i], temp_state[i], 4);
    }

    /**
     * @brief 块异或
     *
     * @param b1
     * @param b2
     * @param b3
     * @param len
     */
    void xor_blocks(const unsigned char *b1, const unsigned char *b2, unsigned char *b3, unsigned int len)
    {
        for (unsigned int i = 0; i < len; i++)
            b3[i] = b1[i] ^ b2[i];
    }

    // 输出数组内容为16进制
    void print_hex(unsigned char a[], unsigned int len)
    {
        for (unsigned int i = 0; i < len; i++)
            printf("%02x ", a[i]);
    }

    // 输出动态数组内容为16进制
    void print_hex(std::vector<unsigned char> a)
    {
        for (unsigned int i = 0; i < a.size(); i++)
            printf("%02x ", a[i]);
    }

    // 将字符数组转换为动态数组
    static std::vector<unsigned char> array_to_vector(unsigned char *a, unsigned int len)
    {
        std::vector<unsigned char> v(a, a + len * sizeof(unsigned char));
        return v;
    }

    // 将动态数组转换为字符数组
    static unsigned char *vector_to_array(std::vector<unsigned char> &v)
    {
        return v.data();
    }

    // 按照iso10126规则填充
    void padding_iso10126(std::vector<unsigned char> &v)
    {
        unsigned int len = v.size();
        unsigned int padlen = blockByteslen - len % blockByteslen;
        unsigned char padbyte = padlen;

        std::cout << "@" << std::endl;
        std::cout << "填充前: " << v.size() << std::endl;
        std::cout << "blockByteslen: " << blockByteslen << std::endl;
        std::cout << "len % blockByteslen: " << len % blockByteslen << std::endl;
        std::cout << "填充长度: " << padlen << std::endl;

        // 创建一个随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        for (unsigned int i = 0; i < padlen - 1; i++)
        {
            int r = dis(gen);
            // std::cout << "r: " << r << std::endl;
            v.push_back(r); // 使用生成器生成随机数
        }
        v.push_back(padbyte);
    }

    /**
     * @brief 分组加密函数
     *
     * @param in    明文输入
     * @param out   密文输出
     * @param roundKeys 轮密钥
     */
    void encrypt_block(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys)
    {
        unsigned char state[4][Nb];
        unsigned int i, j, round;

        for (i = 0; i < 4; i++)
            for (j = 0; j < Nb; j++)
                state[i][j] = in[i + 4 * j];

        add_round_key(state, roundKeys);

        for (round = 1; round <= Nr - 1; round++)
        {
            sub_bytes(state);
            shift_rows(state);
            mix_columns(state);
            add_round_key(state, roundKeys + round * 4 * Nb);
        }

        sub_bytes(state);
        shift_rows(state);
        add_round_key(state, roundKeys + Nr * 4 * Nb);

        for (i = 0; i < 4; i++)
            for (j = 0; j < Nb; j++)
                out[i + 4 * j] = state[i][j];
    }

    // 分组解密函数
    void decrypt_block(const unsigned char in[], unsigned char out[],
                       unsigned char *roundKeys)
    {
        unsigned char state[4][Nb];
        unsigned int i, j, round;

        for (i = 0; i < 4; i++)
            for (j = 0; j < Nb; j++)
                state[i][j] = in[i + 4 * j];

        add_round_key(state, roundKeys + Nr * 4 * Nb);

        for (round = Nr - 1; round >= 1; round--)
        {
            inv_shift_rows(state);
            inv_sub_bytes(state);
            add_round_key(state, roundKeys + round * 4 * Nb);
            inv_mix_columns(state);
        }

        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, roundKeys);

        for (i = 0; i < 4; i++)
            for (j = 0; j < Nb; j++)
                out[i + 4 * j] = state[i][j];
    }

    /**
     * @brief CFB加密
     * @param in    明文输入
     * @param inLen 明文长度
     * @param key   密钥
     * @param iv    初始向量
     * @return unsigned char*
     */
    unsigned char *encrypt_CFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv)
    {
        unsigned char *out = new unsigned char[inLen];
        unsigned char block[blockByteslen];
        unsigned char encryptedBlock[blockByteslen];
        unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
        key_expansion(key, roundKeys);
        memcpy(block, iv, blockByteslen);
        for (unsigned int i = 0; i < inLen; i += blockByteslen)
        {
            encrypt_block(block, encryptedBlock, roundKeys);
            xor_blocks(in + i, encryptedBlock, out + i, blockByteslen);
            memcpy(block, out + i, blockByteslen);
        }

        delete[] roundKeys;

        return out;
    }

    /**
     * @brief   CFB加密接口
     *
     * @param in    密文输入
     * @param key   密钥
     * @param iv    初始向量
     * @return std::vector<unsigned char>
     */
    std::vector<unsigned char> encrypt_CFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv)
    {
        if (in.size() % blockByteslen != 0)
        {
            std::cout << "输入长度不是16的倍数" << std::endl;
            padding_iso10126(in);
        }
        unsigned char *out = encrypt_CFB(vector_to_array(in), (unsigned int)in.size(),
                                         vector_to_array(key), vector_to_array(iv));
        std::vector<unsigned char> v = array_to_vector(out, in.size());
        delete[] out;
        return v;
    }

    /**
     * @brief CFB 解密
     *
     * @param in
     * @param inLen
     * @param key
     * @param iv
     * @return unsigned char
     */
    unsigned char *decrypt_CFB(const unsigned char in[], unsigned int inLen,
                               const unsigned char key[],
                               const unsigned char *iv,
                               unsigned int &outLen,
                               bool final)
    {
        unsigned char *out = new unsigned char[inLen];
        unsigned char block[blockByteslen];
        unsigned char encryptedBlock[blockByteslen];
        unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
        key_expansion(key, roundKeys);
        memcpy(block, iv, blockByteslen);
        for (unsigned int i = 0; i < inLen; i += blockByteslen)
        {
            encrypt_block(block, encryptedBlock, roundKeys);
            xor_blocks(in + i, encryptedBlock, out + i, blockByteslen);
            memcpy(block, in + i, blockByteslen);
        }
        if (final)
        {
            std::cout << "解密输入: " << inLen << std::endl;
            std::cout << "补位长度: " << (unsigned int)out[inLen - 1] << std::endl;
            outLen = inLen - (unsigned int)out[inLen - 1];
            std::cout << "输出长度: " << outLen << std::endl;
        }
        else
        {
            outLen = inLen;
        }
        return out;
    }

    /**
     * @brief CFB解密接口
     *
     * @param in
     * @param key
     * @param iv
     * @return std::vector<unsigned char>
     */
    std::vector<unsigned char> decrypt_CFB(std::vector<unsigned char> in,
                                           std::vector<unsigned char> key,
                                           std::vector<unsigned char> iv,
                                           bool final)
    {
        unsigned int outLen{};
        unsigned char *out = decrypt_CFB(vector_to_array(in), (unsigned int)in.size(),
                                         vector_to_array(key), vector_to_array(iv), outLen, final);
        std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
        delete[] out;
        v.resize(outLen);
        return v;
    }
};