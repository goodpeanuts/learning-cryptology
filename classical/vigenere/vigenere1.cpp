/*
 * @Author: goodpeanuts goodpeanuts@foxmail.com
 * @Date: 2023-11-07 06:23:24
 * @LastEditors: goodpeanuts goodpeanuts@foxmail.com
 * @LastEditTime: 2023-11-07 07:50:28
 * @FilePath: /cryto/vigenere.cpp
 * @Description: 维吉尼亚密码的加密与解密
 */
# include <iostream>
using namespace std;

string encryptVigenere(string plainText, string key) {
    string cipherText = "";
    for (int i = 0; i < plainText.length(); i++) {
        //char plainChar = toupper(plainText[i]);
        char plainChar = plainText[i];
        char keyChar = toupper(key[i % key.length()]);
        if (plainChar >= 'A' && plainChar <= 'Z') {
            char cipherChar = ((plainChar - 'A') + (keyChar - 'A')) % 26 + 'A';
            cipherText += cipherChar;
        } else if (plainChar >= 'a' && plainChar <= 'z') {
            char cipherChar = ((plainChar - 'a') + (keyChar - 'A')) % 26 + 'a';
            cipherText += cipherChar;
        } else {
            cipherText += plainChar; // 不是字母的话直接加上
        }
    }
    return cipherText;
}

string decryptVigenere(string cipherText, string key) {
    string plainText = "";
    for (int i = 0; i < cipherText.length(); i++) {
        //char cipherChar = toupper(cipherText[i]);
        char cipherChar = cipherText[i];
        char keyChar = toupper(key[i % key.length()]);
        if (cipherChar >= 'A' && cipherChar <= 'Z') {
            char plainChar = ((cipherChar - 'A') - (keyChar - 'A') + 26) % 26 + 'A';
            plainText += plainChar;
        } else if (cipherChar >= 'a' && cipherChar <= 'z') {
            char plainChar = ((cipherChar - 'a') - (keyChar - 'A') + 26) % 26 + 'a';
            plainText += plainChar;
        } else {
            plainText += cipherChar;
        }
    }
    return plainText;
}


int main()
{
    string plainText;
    string key;
    string cipherText;
    string decipherText;
    int choice;

    while (1){
        cout << "\033[0m1. 加密" << endl;
        cout << "2. 解密" << endl;
        cout << "3. 退出" << endl;
        cout << "请输入你的选择: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "\033[32m请输入明文:  ";
            cin >> plainText;
            cout << "\033[36m请输入密钥:  ";
            cin >> key;
            cipherText = encryptVigenere(plainText, key);
            cout << "\033[35m密文结果为:  " << cipherText << endl;
            break;
        case 2:
            cout << "\033[35m请输入密文:  ";
            cin >> cipherText;
            cout << "\033[36m请输入密钥:  ";
            cin >> key;
            decipherText = decryptVigenere(cipherText, key);
            cout << "\033[32m明文结果为:  " << decipherText << endl;
            break;
        case 3:
            exit(0);
        default:
            cout << "输入错误!" << endl;
            exit(0);
        }
    }
    
}

