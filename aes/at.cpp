/*
 * @Author: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @Date: 2023-12-12 16:16:29
 * @LastEditors: goodpeanuts 143506992+goodpeanuts@users.noreply.github.com
 * @LastEditTime: 2023-12-12 16:20:59
 * @FilePath: /learning-cryptology/aes/at.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <stdio.h>
#include <string.h>

#include <openssl/aes.h>

int main()
{
    // 初始化明文和密钥
    unsigned char plaintext[AES_BLOCK_SIZE] = "Hello, world!   ";
    unsigned char key[AES_BLOCK_SIZE] = "secret-key-secret";

    // 创建 AES 上下文
    AES_KEY aes_ctx;
    AES_set_encrypt_key(key, 128, &aes_ctx);

    // 加密明文
    unsigned char ciphertext[AES_BLOCK_SIZE];
    AES_encrypt(plaintext, ciphertext, &aes_ctx);

    // 打印密文
    for(int i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%02x", ciphertext[i]);
    printf("\n");

    // 创建解密上下文
    AES_set_decrypt_key(key, 128, &aes_ctx);

    // 解密密文
    unsigned char plaintext_decrypted[AES_BLOCK_SIZE];
    AES_decrypt(ciphertext, plaintext_decrypted, &aes_ctx);

    // 打印解密后的明文
    printf("plaintext_decrypted: %s\n", plaintext_decrypted);

    return 0;
}