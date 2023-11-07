'''
Author: goodpeanuts goodpeanuts@foxmail.com
Date: 2023-11-07 07:56:54
LastEditors: goodpeanuts goodpeanuts@foxmail.com
LastEditTime: 2023-11-07 14:33:08
FilePath: /cryto/vigenere.py
Description: 维吉尼亚密码的加密与解密
'''
def encrypt_vigenere(plain_text, key):
    cipher_text = ""
    for i in range(len(plain_text)):
        plain_char = plain_text[i]
        key_char = key[i % len(key)].upper()
        if 'A' <= plain_char <= 'Z':
            cipher_char = chr(((ord(plain_char) - ord('A')) + ord(key_char) - ord('A')) % 26 + ord('A'))
            cipher_text += cipher_char
        elif 'a' <= plain_char <= 'z':
            cipher_char = chr(((ord(plain_char) - ord('a')) + ord(key_char) - ord('A')) % 26 + ord('a'))
            cipher_text += cipher_char
        else: 
            cipher_text += plain_char
    return cipher_text
    

def decrypt_vigenere(cipher_text, key):
    plain_text = ""
    for i in range(len(cipher_text)):
        cipher_char = cipher_text[i]
        key_char = key[i % len(key)].upper()
        if 'A' <= cipher_char <= 'Z':
            plain_char = chr((ord(cipher_char) - ord('A') - (ord(key_char) + ord('A'))) % 26 + ord('A'))
            plain_text += plain_char
        elif 'a' <= cipher_char <= 'z':
            plain_char = chr((ord(cipher_char) - ord('a') - ord(key_char) + ord('A')) % 26 + ord('a'))
            plain_text += plain_char
        else:
            plain_text += cipher_char
    return plain_text

def main():
    while True:
        print("1. 加密")
        print("2. 解密")
        print("3. 退出")
        choice = input("请输入你的选择")
        if choice == "1":
            cipher_text = input("请输入密文")
            key = input("请输入密钥")
            print("密文结果为：%s" % encrypt_vigenere(cipher_text, key))
        elif choice == "2":
            plain_text = input("请输入密文： ")
            key = input("请输入密钥： ")
            print("明文结果为： %s" % decrypt_vigenere(plain_text, key))
        elif choice == "3":
            break
        else:
            print("无效操作")

if __name__ == "__main__":
    main()
                
        