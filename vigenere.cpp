/*
 * @Author: goodpeanuts goodpeanuts@foxmail.com
 * @Date: 2023-11-07 06:23:24
 * @LastEditors: goodpeanuts goodpeanuts@foxmail.com
 * @LastEditTime: 2023-11-07 06:52:00
 * @FilePath: /rcore-ali/cryto/vignir.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


# include <iostream>
using namespace std;

string encryptVigenere(string plainText, string key) {
    string cipherText = "";
    for (int i = 0; i < plainText.length(); i++) {
        char plainChar = toupper(plainText[i]);
        char keyChar = toupper(key[i % key.length()]);
        if (plainChar >= 'A' && plainChar <= 'Z') {
            char cipherChar = ((plainChar - 'A') + (keyChar - 'A')) % 26 + 'A';
            cipherText += cipherChar;
        } else {
            cipherText += plainChar;
        }
    }
    return cipherText;
}

string decryptVigenere(string cipherText, string key) {
    string plainText = "";
    for (int i = 0; i < cipherText.length(); i++) {
        char cipherChar = toupper(cipherText[i]);
        char keyChar = toupper(key[i % key.length()]);
        if (cipherChar >= 'A' && cipherChar <= 'Z') {
            char plainChar = ((cipherChar - 'A') - (keyChar - 'A') + 26) % 26 + 'A';
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
    cout << "1. 加密" << endl;
    cout << "2. 解密" << endl;
    cout << "3. 退出" << endl;
    cout << "请输入你的选择: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        cout << "请输入明文: ";
        cin >> plainText;
        cout << "请输入密钥: ";
        cin >> key;
        cipherText = encryptVigenere(plainText, key);
        cout << "密文为: " << cipherText << endl;
        break;
    case 2:
        cout << "请输入密文: ";
        cin >> cipherText;
        cout << "请输入密钥: ";
        cin >> key;
        decipherText = decryptVigenere(cipherText, key);
        cout << "明文为: " << decipherText << endl;
        break;
    case 3:
        exit(0);
    default:
        cout << "输入错误!" << endl;
        exit(0);
    }
}

