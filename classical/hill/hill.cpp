/*
 * @Author: goodpeanuts goodpeanuts@foxmail.com
 * @Date: 2023-11-07 07:51:46
 * @LastEditors: goodpeanuts goodpeanuts@foxmail.com
 * @LastEditTime: 2023-11-07 07:54:05
 * @FilePath: /cryto/hill.cpp
 * @Description: 
 */
# include <iostream>
# include <vector>
using namespace std;

vector<vector<int>> inverseMatrix(vector<vector<int>> matrix) {
    int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    int invDet = 1 / det;
    swap(matrix[0][0], matrix[1][1]);
    matrix[0][1] *= -1;
    matrix[1][0] *= -1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            matrix[i][j] *= invDet;
        }
    }
    return matrix;
}

vector<int> matrixVectorMultiply(vector<vector<int>> matrix, vector<int> vec) {
    vector<int> result(2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
        result[i] %= 26;
    }
    return result;
}

string encryptHill(string plainText, vector<vector<int>> key) {
    string cipherText = "";
    for (int i = 0; i < plainText.length(); i += 2) {
        vector<int> vec = {plainText[i] - 'A', plainText[i + 1] - 'A'};
        vector<int> res = matrixVectorMultiply(key, vec);
        cipherText += 'A' + res[0];
        cipherText += 'A' + res[1];
    }
    return cipherText;
}

string decryptHill(string cipherText, vector<vector<int>> key) {
    string plainText = "";
    vector<vector<int>> invKey = inverseMatrix(key);
    for (int i = 0; i < cipherText.length(); i += 2) {
        vector<int> vec = {cipherText[i] - 'A', cipherText[i + 1] - 'A'};
        vector<int> res = matrixVectorMultiply(invKey, vec);
        plainText += 'A' + res[0];
        plainText += 'A' + res[1];
    }
    return plainText;
}

int main(){
    string plainText;
    string cipherText;
    string decipherText;
    vector<vector<int>> key = {{3, 3}, {2, 5}};
    int choice;

    while (1){
        cout << "\033[0m1. 加密" << endl;
        cout << "2. 解密" << endl;
        cout << "3. 退出" << endl;
        cout << "请输入你的选择: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "请输入明文: ";
                cin >> plainText;
                cipherText = encryptHill(plainText, key);
                cout << "密文为: " << cipherText << endl;
                break;
            case 2:
                cout << "请输入密文: ";
                cin >> cipherText;
                decipherText = decryptHill(cipherText, key);
                cout << "明文为: " << decipherText << endl;
                break;
            case 3:
                return 0;
            default:
                cout << "输入错误, 请重新输入" << endl;
                break;
        }
    }
    return 0;
}