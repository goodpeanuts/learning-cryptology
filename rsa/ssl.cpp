/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-20 06:47:21
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-20 07:15:33
 * @FilePath: /learning-cryptology/rsa/ssl.cpp
 * @Description: C++语言简单实现RSA算法
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <iostream>
#include <cstring>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

int main() {
    int ret = 0;
    RSA *r = RSA_new();
    BIGNUM *bne = BN_new();
    unsigned long e = RSA_F4;

    ret = BN_set_word(bne,e);
    if(ret != 1){
        // handle error
    }

    ret = RSA_generate_key_ex(r, 2048, bne, NULL);
    if(ret != 1){
        // handle error
    }

    // 明文
    const char *plaintext = "Hello, RSA!";
    // 分配内存来保存密文和解密后的文本
    unsigned char *ciphertext = (unsigned char *)malloc(RSA_size(r));
    unsigned char *decrypted_text = (unsigned char *)malloc(RSA_size(r));

    // 加密
    int ciphertext_len = RSA_public_encrypt(strlen(plaintext), (const unsigned char *)plaintext, ciphertext, r, RSA_PKCS1_PADDING);    
    if(ciphertext_len == -1){
        // handle error
    }

    // 打印结果
    std::cout << "Encrypted Text: " << ciphertext << std::endl;

    // 解密
    int decrypted_text_len = RSA_private_decrypt(ciphertext_len, ciphertext, decrypted_text, r, RSA_PKCS1_PADDING);
    if(decrypted_text_len == -1){
        // handle error
    }

    // 打印结果
    std::cout << "Decrypted Text: " << decrypted_text << std::endl;

    // 打印公钥
    PEM_write_RSAPublicKey(stdout, r);

    // 打印私钥
    PEM_write_RSAPrivateKey(stdout, r, NULL, NULL, 0, NULL, NULL);

    // 释放内存
    free(ciphertext);
    free(decrypted_text);

    // 释放RSA结构
    RSA_free(r);
    BN_free(bne);

    return 0;
}