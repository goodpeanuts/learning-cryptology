/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-26 23:13:47
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-29 13:26:54
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
#include "md5.h"
#include "aes.h"
#define PORT 55035
#define BUFFER_SIZE 4096
#define BUFFER_SIZE2 8388608
using namespace std;

struct CA
{
    std::string name;
    std::string id;
    std::string e;
    std::string n;
    std::string signature;
};

std::ostream &operator<<(std::ostream &os, const CA &ca)
{
    os << "[Name:] " << ca.name << "\n"
       << "[ID:] " << ca.id << "\n"
       << "[E:] " << ca.e << "\n"
       << "[N:] " << ca.n << "\n"
       << "[Signature:] " << ca.signature;
    return os;
}

// bob 的私钥
const std::string bob_d_str = "389825748650402112134998776902132410583756908795024087245043139972646260814652967766179757895076724324424081908933719324919154409923680787019794127142845663790100570927209749852804608397958623395452952169718062102001696545990613220524183760279403876536863781906843189651452294462993098322137869373346135791091448839388316574504890237834041581893945918588079631728648275457986697040852588184736836642504611241090768420802545035238600480905724655482622424334110555423605804864582830596116980404501410937562352717408126311530533439228808772759197002211467039493127308998434626959947017724681616020520060415920226628755122652882473546780487130614676753627020633593753968285702644394044824964462378203844871726930362345321603050978034850474500806416129949564886007770370924795918777565288858625442883356775478699639526885200140642915200644477545201132310828772384876447734353091324631919532742491325384329582870476467670261397155965843344730475544353019635243976248058209043526254838295719296369854280595991011646450445420259065830297809196883680118510975487656614251893087536033739386752509345357240916292252008364627090718855213294454960957328525211782965794756090611820700899401134982007188441220621409380137164937884794665026664870849";

const std::string bob_n_str = "456446233742196804212902752266936832852125394340798279785830696777179480012898061076033004654846661546245593708379992433074263257432636335739366726577985669049336812547790334948228576397426174978701974173957287621210016044094852679085416447649708027991410040232129273483468892847628220081565651823310497859002832507361485481989880199366386363504363147902551811484370121450778500501167927815594263611918068525564141710645769420199293850670889996303057366979873885242511692975203500434330746039278603161756814562276682343506640637258257443868009295125663629387955468252098947150839064916138940139984253473380017728811368220442955932459604311777195329660762485331104666004195402984068905436567813448775702782070146930987072297477472206067540983529781232571554416370513226436426433168797317163805162082651087959512465153650760490436129376316420216695146005204483052724844407563320859568784905134933660287567928142700713719851826229649532218750801223775446936103897839829361016565453088543897490597559573643098359678935057911435759855704510790434787499718161436442576809250279702081517348909828551456087751991781123899313625454587567988929658990956348935032934768191965775124634387122453446066264517044857966058886658227182869548828145081";

// ca 的公钥
const std::string ca_e_str = "48230234972912824395842558097639002513568074476425538103268507341286887459231";

const std::string ca_n_str = "619602684616718133248166089597571487511277402914611620850125903424668688104687670617378588775835487311063258626548554081827812655925357694818063993928526158345239445898782939091939534158590104237602533025009702708312473083110552254979005350786410296624529923063440890284652345112272600681355857616730143146815018336562190721398854580071921151308489054038089831835855652447214604080474324413176928326486156913321758465185674780798384482836956528975868405845522525584348905831801107722404011544272835142084995828581965045114648892493671003801328310152648415556547166218496006151999501918778575259950301418580325847216333782171847192858517438607670864898944012265229831003226596417986859654041555237089012297250957831824506518898039849667441035198614256630307837962864485936438192773058171655684737474296652245829932551480504038519905494148852150387567449487862996399079029669341374317456363373157341294071261687446233389489525278305797441376350202598176868140794246405466660433438702380961687906758250650466175257003880442503576664576169994242705332173013315618901593667359420965063396903165950548639725829800214737507511974963928624712688275462581538292339732451089219745284674444592936397213730800316583112799882291891921182659058507";

// 解密对称密钥
std::string decrypt_rsa_bob(const std::string &in)
{
    CryptoPP::Integer c = RSA::encode_string(in);
    CryptoPP::Integer n(bob_n_str.c_str());
    CryptoPP::Integer d(bob_d_str.c_str());
    return RSA::decode_string((a_exp_b_mod_c(c, d, n)));
}

// aes解密
std::string deString(const std::vector<unsigned char> &k, const std::vector<unsigned char> &iv, std::string input)
{

    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};

    std::cout << "[字符串解密]" << std::endl;

    in.assign(input.begin(), input.end());
    out.assign(input.begin(), input.end());

    AES aes(AESMode::AES_128);

    // 由于输入的hash值刚好是aes分组倍数，解密时不要去补位
    out = aes.decrypt_CFB(in, k, iv, false);
    // out转string
    std::string out_str(out.begin(), out.end());
    return out_str;
}

void buffer_de(const std::vector<unsigned char> &k, const std::vector<unsigned char> &iv, string filename, std::string output_filename)
{
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};

    std::cout << "[文件解密]" << std::endl;

    AES aes(AESMode::AES_128);

    std::ifstream input_file(filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);
    cout << "[文件名] " << filename << endl;
    cout << "[输出文件名] " << output_filename << endl;

    char *buffer = new char[BUFFER_SIZE2];

    while (true)
    {
        input_file.read(buffer, BUFFER_SIZE2);
        std::streamsize size = input_file.gcount();
        std::cout << "解密大小: " << size << std::endl;
        if (size == 0)
        {
            break;
        }
        in.assign(buffer, buffer + size);
        if (size < BUFFER_SIZE2)
        {
            out = aes.decrypt_CFB(in, k, iv, true);
        }
        else
        {
            out = aes.decrypt_CFB(in, k, iv, false);
        }
        output_file.write(reinterpret_cast<const char *>(out.data()), out.size());
    }

    delete[] buffer;
    input_file.close();
    output_file.close();
}

// 读取证书
void parseInput(const std::string &input, CA &ca)
{
    size_t pos1, pos2;

    pos1 = input.find("<name>") + 6;
    pos2 = input.find("</name>");
    ca.name = input.substr(pos1, pos2 - pos1);

    pos1 = input.find("<id>") + 4;
    pos2 = input.find("</id>");
    ca.id = input.substr(pos1, pos2 - pos1);

    pos1 = input.find("<e>") + 3;
    pos2 = input.find("</e>");
    ca.e = input.substr(pos1, pos2 - pos1);

    pos1 = input.find("<n>") + 3;
    pos2 = input.find("</n>");
    ca.n = input.substr(pos1, pos2 - pos1);

    pos1 = input.find("<signature>") + 11;
    pos2 = input.find("</signature>");
    ca.signature = input.substr(pos1, pos2 - pos1);
    // assert(ca.signature.length() == 512);
}

// 验证证书
void verifyCertificate(const CA &a)
{
    // 计算hash
    std::string toHash = a.name + a.id + a.e + a.n;
    std::cout << "[toHash:] " << toHash << std::endl;
    char *toHashCopy = new char[toHash.length() + 1];
    std::strcpy(toHashCopy, toHash.c_str());
    char *hashCStr = strMd5(toHashCopy);
    std::string hash(hashCStr);

    std::cout << "[证书Hash:] " << hash << std::endl;

    // 验签
    CryptoPP::Integer e(ca_e_str.c_str());
    CryptoPP::Integer n(ca_n_str.c_str());
    CryptoPP::Integer signInt = RSA::encode_string(a.signature);
    CryptoPP::Integer hashInt = a_exp_b_mod_c(signInt, e, n);

    // 将hash转换为字符串
    std::string desig = RSA::decode_string(hashInt);
    std::cout << "[解密Hash] " << desig << std::endl;

    if (desig == hash)
    {
        std::cout << "\033[32m[Alice 证书验证成功] \n\033[0m" << std::endl;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "####### [验证失败] #######" << std::endl;
    }
    assert(desig == hash);
    delete[] toHashCopy;
}

// 验证文件hash签名
std::string verify_file_hash_sig(std::string sig_str, const CA &target)
{
    CryptoPP::Integer e(target.e.c_str());
    CryptoPP::Integer n(target.n.c_str());
    CryptoPP::Integer signInt = RSA::encode_string(sig_str);
    CryptoPP::Integer hashInt = a_exp_b_mod_c(signInt, e, n);
    string sig = RSA::decode_string(hashInt);
    return sig;
}

// 发给客户端的信息
char msg[] = "TCP连接成功，我是Bob";

int main()
{
    /***************   网络传输初始化  ***************/
    int sock_server, newsock;
    struct sockaddr_in addr;
    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];
    size_t msg_len{};

    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    int addr_len = sizeof(struct sockaddr_in);
    memset((void *)&addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    bind(sock_server, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock_server, 0);

    while (true)
    {
        cout << "[Bob上线]\n";

        /***************   1.接收连接  *********************/
        newsock = accept(sock_server, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        cout << "\033[32m[建立连接]\n\033[0m";

        /***************   2.建立TCP连接  *********************/
        int ret1_hello = send(newsock, msg, sizeof(msg), 0);
        if (ret1_hello == -1)
        {
            perror("[连接失败]\n");
            close(newsock);
            continue;
        }
        string filename;
        // 接收文件名
        recv(newsock, (char *)&msg_len, sizeof(msg_len), 0);
        memset(buffer, 0, sizeof(buffer));

        int ret1_filename = recv(newsock, buffer, msg_len, 0);
        if (ret1_filename == -1)
        {
            perror("[接收失败]\n");
            close(newsock);
            continue;
        }
        filename = string(buffer, ret1_filename);
        /***************  3.接收密钥  *********************/
        recv(newsock, (char *)&msg_len, sizeof(msg_len), 0);
        memset(buffer, 0, sizeof(buffer));

        int ret2_key = recv(newsock, buffer, msg_len, 0);
        if (ret2_key == -1)
        {
            perror("[接收 key 失败]\n");
            close(newsock);
            continue;
        }
        string k(buffer, ret2_key);
        string key_iv_str = decrypt_rsa_bob(k);
        cout << "\033[32m[接收 key 成功]\n\033[0m";

        // 将 key_iv_str 转换为 vector
        vector<unsigned char> key;
        for (int i = 0; i < 8; i++)
        {
            key.push_back(key_iv_str[i]);
        }
        vector<unsigned char> iv;
        for (int i = 8; i < 16; i++)
        {
            iv.push_back(key_iv_str[i]);
        }

        // 输出key 和 iv
        std::cout << "[recv_key:] ";
        for (auto i : key)
        {
            std::cout << std::hex << static_cast<int>(i);
        }
        std::cout << std::endl;
        std::cout << "[recv_iv: ] ";
        for (auto i : iv)
        {
            std::cout << std::hex << static_cast<int>(i);
        }
        std::cout << std::endl;

        /***************  4.接收文件hash的签名  *******************/
        recv(newsock, (char *)&msg_len, sizeof(msg_len), 0);
        memset(buffer, 0, sizeof(buffer));

        int ret3_sig = recv(newsock, buffer, msg_len, 0);
        if (ret3_sig == -1)
        {
            perror("[接收 sig 失败]");
            close(newsock);
            continue;
        }
        string en_sig(buffer, ret3_sig);
        std::cout << "\033[32m[接收签名成功]\033[0m" << std::endl;

        // 解密签名
        std::string sig = deString(key, iv, en_sig);

        /***************  5.接收加密文件  *********************/
        // 接收服务端将发送的文件的大小
        streampos fileSize;
        int size = recv(newsock, (char *)&fileSize, sizeof(fileSize), 0);
        if (size <= 0)
        {
            perror("[与alice断开连接]");
            close(newsock);
            return 0;
        }

        // 创建文件
        ofstream file("mid", ios::binary);
        // ofstream file;

        // 接收文件内容
        int remainingSize = fileSize;
        while (remainingSize > 0)
        {
            size = recv(newsock, buffer, min(remainingSize, BUFFER_SIZE), 0);
            if (size <= 0)
                perror("接收文件失败");
            else
            {
                file.write(buffer, size);
                remainingSize -= size;
            }
        }

        file.close();
        cout << "\033[32m[文件接收完成]\033[0m" << endl;

        // 解密文件
        buffer_de(key, iv, "mid", filename);

        /***************  6.接收alice加密证书  *********************/
        recv(newsock, (char *)&msg_len, sizeof(msg_len), 0);
        memset(buffer, 0, sizeof(buffer));

        int ret4_cert = recv(newsock, buffer, msg_len, 0);
        if (ret4_cert == -1)
        {
            perror("[接收 cert 失败]");
            close(newsock);
            continue;
        }
        string en_cert(buffer, ret4_cert);
        std::cout << "[证书长度:]" << en_cert.length() << std::endl;

        std::string cert = deString(key, iv, en_cert);

        // 解析证书
        CA ca;
        parseInput(cert, ca);
        // 验证证书
        verifyCertificate(ca);

        /***************  7.验证文件hash  *********************/
        cout << "[filename] "<< filename << endl;
        int length = std::strlen(filename.c_str());
        char *filename_copy = new char[length + 1];
        std::strcpy(filename_copy, filename.c_str());
        char *hash = fileMD5(filename_copy);
        std::string hash_str(hash);
        delete[] filename_copy;
        delete[] hash;

        verify_file_hash_sig(sig, ca);
        cout << "[alice hash:] " << hash_str << endl;
        cout << "\033[32m[接收成功]\n\033[0m";
        cout << "------------------------- \n\n";

        // 发送完成,结束本次TCP连接
        close(newsock);
    }
    // 关闭套接字
    close(sock_server);
    return 0;
}