/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-26 23:13:47
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-27 00:42:09
 * @FilePath: /learning-cryptology/communication/alice.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <string.h> // for memset
#include <unistd.h> // for close function
#include <cryptopp/integer.h>
#define PORT 55035
#define BUFFER_SIZE 4096
using namespace std;

const string alice_d_str = "498453222215928988126169619006605384453272441202680604604576357471790002576263412848386385287928565117069481959424995104950955461213201927503048325278861346814486461243469995065910727701232527616953585068690615454007185673608891828233515849175709594636701977801913453897457128463450753027292662266297313784515357092147243116957962287689235138635048238597427055158686758288180417831351889604577787295922064621597343112701628444159940290880468418999258123277461877617813253533955905934523790699776181722963818285525373007208852878693250201561460729692297843301110365235196858971533880455991095604210879372338847748732573218970062928021562197310706118673225095091937998420330015168761493166726849750477929998130449132036984422352551012538938158948197271576609068121684054339209572547684222446878194447379973938750979119231629771396248769895877779678556275097388000349754104407571234555669605487501404361817615022171854701863540529395351951015831407004291819697413769974376968742701254475343994949538335262461324358546989229964324719199211783756044587926272286753942296280251518975161462448703911660193276559060050584286237888183984639400927841030034712435181944649254745534229692131171094798461218877138659764356483521846736600287101819";
const CryptoPP::Integer alice_d(alice_d_str.c_str());

int main()
{
    // 建立客户端流式套接字
    int sock_client;
    struct sockaddr_in server_addr;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    // 设置客户端套接字,使用IPv4地址族,TCP协议
    // 事先设定服务器地址 130.33.40.2
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    memset((void *)&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 填写Bob的IP地址

    // 发送连接请求
    if (connect(sock_client, (struct sockaddr *)&server_addr, addr_len) == -1)
    {
        perror("connect failed");
        close(sock_client);
        return -1;
    }

    // 接收连接成功Bob消息
    int ret1_hello = recv(sock_client, buffer, BUFFER_SIZE, 0);
    if (ret1_hello == 0 || ret1_hello == -1)
    {
        perror("【连接失败】\n");
        close(sock_client);
        return -1;
    }

    if (send(sock_client, alice_d_str.c_str(), alice_d_str.length(), 0) == -1)
    {
        cout << "发送失败\n";
        close(sock_client);
        return 0;
    }
    else
    {
        cout << alice_d_str.c_str() << endl;
        cout << alice_d_str.length() << endl;
    }

    // 关闭套接字
    close(sock_client);
    return 0;
}