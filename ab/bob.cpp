/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-26 23:13:47
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-27 12:16:46
 * @FilePath: /learning-cryptology/ab/bob.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved. 
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close function
#include <iostream>
#include <fstream>
#include <string.h> // for memset
#include "rsa.h"
#define PORT 55035
#define BUFFER_SIZE 4096
using namespace std;

const string alice_e_str = "89199853505208837434219915418407101778770843977489290671627353926135276140267";
const string alice_n_str = "756382892270458101418679125554245392008112371258983767909894414938106891308552600445122086461436414661864518099813807010555191723086196148659597820002453699888135516080477536907210139653952341230739243743617458018766673156363745907262045305318186498066192981329480594982315593885173639283767491493645768978457412960541129345929786561026495454263424716275627867149204441919047608946921152551523689774931821930018369250899478677826625900078871874081052948095998627441514046855843610885230573093126952817745051924307008447809817539449335992189397180799276996065334839850826010684981721158507376722845860165618164386541933617045200207565793549143943130331560257402475589086011374955992241285627596112036658011885152575168383352551407165953437539626387139533790070827342500058468317009974355292052384470156040217323061636314674794313952720737256437957202177104211670251359670958082800111954934860430892426756189242618983897238979990500509062809733962095829228363434075810518750614959379592734761189637538851726126492151749544524754210273518012255113409644359547764076813800693695289037242458728678326912146324158330254070704828112373528583978397903276894151038308123811847463542546937888879951025084643883827949913103226173976935611494261";
const CryptoPP::Integer alice_e(alice_e_str.c_str());
const CryptoPP::Integer alice_n(alice_n_str.c_str());

//发给客户端的信息
char msg[] = "TCP连接成功，我是Bob";  

int main() {
    //建立流式套接字,分别为服务端和新连接
    int sock_server, newsock;    
    struct sockaddr_in addr;         
    struct sockaddr_in client_addr;  
    char buffer[BUFFER_SIZE];
    
    //设置服务端套接字,使用IPv4地址族,TCP协议
    //接受所有网络地址的连接申请
    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    int addr_len = sizeof(struct sockaddr_in);
    memset((void*)&addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");   
    
    //监听套接字绑定地址
    bind(sock_server, (struct sockaddr*)&addr, sizeof(addr));
    
    //监听客户的连接请求
    listen(sock_server, 0);
    //接收连接请求并收发数据
    //程序会一直循环执行,持续监听客户端连接和进行数据交互，
    //直到手动停止程序
    while (true) {
        cout << "【Bob上线】\n";

        // 接收连接请求
        newsock = accept(sock_server, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);  
        cout << "【成功建立连接】\n";
        
        // 与客户端建立连接
        int ret1_hello = send(newsock, msg, sizeof(msg), 0);
        if (ret1_hello == -1) {
            perror("【连接失败】\n");
            close(newsock);
            continue;
        }
        
        // 接收Alice的公钥
        int size1 = recv(newsock, buffer, BUFFER_SIZE, 0);
        if (size1 == 0) {
            cout << "## 与客户端断开连接 ## \n";
            close(newsock);
            continue;
        }
        else {
            buffer[size1] = '\0';
            string alice_d_str = buffer;
            cout << "【Alice的长度】 " << alice_d_str.length() << endl;
            CryptoPP::Integer alice_d(alice_d_str.c_str());
            cout << "【Alice的私钥】 " << alice_d << endl;
        }
        
        //发送完成,结束本次TCP连接
        close(newsock);
    }
    
    //关闭套接字
    close(sock_server);
    return 0;
}