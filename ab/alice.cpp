/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-26 23:13:47
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-28 16:27:18
 * @FilePath: /learning-cryptology/ab/alice.cpp
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cryptopp/integer.h>
#include <random>
#include <vector>
#include <string>

#include "rsa.h"
#include "md5.h"
#include "aes.h"
#define PORT 55035
#define BUFFER_SIZE 4096
#define BUFFER_SIZE2 4194304
using namespace std;

// alice 的私钥
const std::string alice_d_str = "121305883625848036560778141668465183417312012603965510236219996196098547236552123473195427019155749376924332388013425839810235142470700828835241929797211376555497640638968730592314121729339006979563571216343548581091047868263177734285851152923732196344660114640557102987506719578398520268796847143543327007864545495356167659879848181747975288878326514557053319729277579435355172852081629161453031084898348815357202259377268589527535846836142179254157678543975018234188284532299646781012810910760200954451560367417903134375233586335088848479247160330791139303895811634072785669523859113763631048971238332462551840319212912961310893173718499386437731080275092938207118158874166681341568814272933231502646251812151007040691826678713222488659953900552736733158767221674299451833858903280574069074938772223601550756664665416003853290075838556967213420483260911554542930678534339784946858369815846258466611329116579641798341581159408317631919255350403546324763820229760425170662928834442646815011105509355831220670358913125592533687517318726117879760396637356663547849256178315959237658183760429644989941690965610704048475411449554694355644227186505633253261112353303720640934697994819045375861720316717788014784044110398037931474319064139";

const std::string alice_n_str = "579522468003058243993239718950711828500800749483577015429778065585043322873845982071284241150570367240755757029946959066571184411169766879234411047738154614362624231684848620979103805672910179736584815175517516924302006013577345525960267375378910249948358897175136227037095486748949371319105108888443645487166106677051165039985747428048143395159785162003610378281176670853681513423438724167721882099664216826247372819935724637013384717211914648415523188145358488874264426730418904258325424425927206901995811469019304499772781738016264778391642320236296292511692819568519068440644315519902509583506918576434426698938066133050353875506811860489411748932283886586747829955004674421553589305482396067109198865928635151892115955919165180712917957001470653115500769693295449245503918383677533432638802567192742450022596394344588691736551043591478947915020898572918614992062853797658233981443990019780382205282819066445767247097462193827555395633596968943860424115713638084962527519600866309506161169879272466594965920660279975116978823523822413096445435487121033286857118016309144418414699423056420427297625960588727329613160040048426043050130108980369419504037944766627814048677372966151817913803208942937954209562410731831288541256408699";

// ca 的公钥
const std::string ca_e_str = "48230234972912824395842558097639002513568074476425538103268507341286887459231";

const std::string ca_n_str = "619602684616718133248166089597571487511277402914611620850125903424668688104687670617378588775835487311063258626548554081827812655925357694818063993928526158345239445898782939091939534158590104237602533025009702708312473083110552254979005350786410296624529923063440890284652345112272600681355857616730143146815018336562190721398854580071921151308489054038089831835855652447214604080474324413176928326486156913321758465185674780798384482836956528975868405845522525584348905831801107722404011544272835142084995828581965045114648892493671003801328310152648415556547166218496006151999501918778575259950301418580325847216333782171847192858517438607670864898944012265229831003226596417986859654041555237089012297250957831824506518898039849667441035198614256630307837962864485936438192773058171655684737474296652245829932551480504038519905494148852150387567449487862996399079029669341374317456363373157341294071261687446233389489525278305797441376350202598176868140794246405466660433438702380961687906758250650466175257003880442503576664576169994242705332173013315618901593667359420965063396903165950548639725829800214737507511974963928624712688275462581538292339732451089219745284674444592936397213730800316583112799882291891921182659058507";

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
        std::cout << std::endl;
        std::cout << "======= [验证成功] =======" << std::endl;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "####### [验证失败] #######" << std::endl;
    }
    assert(desig == hash);
    delete[] toHashCopy;
}
// 计算文件hash并签名
std::string get_file_hash_sig(char *filename)
{
    char *hash = fileMD5(filename);
    std::string hash_str(hash);
    delete[] hash;

    // 对hash签名
    CryptoPP::Integer d(alice_d_str.c_str());
    CryptoPP::Integer n(alice_n_str.c_str());
    CryptoPP::Integer hashInt = RSA::encode_string(hash);
    CryptoPP::Integer signInt = a_exp_b_mod_c(hashInt, d, n);

    // 将签名转换为字符串
    std::string sig_str = RSA::decode_string(signInt);
    return sig_str;
}

// 解析证书读取对方公钥
CA parse_cert(const char *cert_name)
{
    // 读入证书
    std::cout << std::endl;
    std::cout << "======== 读入证书 ========" << std::endl;
    std::ifstream fin;
    fin.open(cert_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string input = buffer.str();
    fin.close();

    // 读入证书
    CA ca;
    parseInput(input, ca);

    // 验证证书
    verifyCertificate(ca);
    return ca;
}

// 生成随机的aes128 的 key 和 iv
std::string generate_aes_key_iv(std::vector<unsigned char> &key, std::vector<unsigned char> &iv)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < 8; i++)
    {
        key.push_back(static_cast<unsigned char>(dis(gen)));
    }
    for (int i = 0; i < 8; i++)
    {
        iv.push_back(static_cast<unsigned char>(dis(gen)));
    }
    // 将key 和 iv 转为字符串
    std::string key_str(key.begin(), key.end());
    std::string iv_str(iv.begin(), iv.end());
    std::string key_iv_str = key_str + iv_str;
    return key_iv_str;
}

// 加密对称密钥
std::string encrypt_k(string in, CA target)
{
    CryptoPP::Integer m = RSA::encode_string(in);
    CryptoPP::Integer e(target.e.c_str());
    CryptoPP::Integer n(target.n.c_str());
    CryptoPP::Integer c = a_exp_b_mod_c(m, e, n);
    cout << "加密前Int  " << m << endl;
    cout << "加密后Int  " << c << endl;
    string k = RSA::decode_string(c);
    std::cout << "[k_len]" << k.length() << std::endl;
    return k;
}

// aes加密
std::string enString(const std::vector<unsigned char> &k, const std::vector<unsigned char> &iv, std::string input)
{

    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};

    std::cout << "=========== 字符串加密 ===========" << std::endl;

    in.assign(input.begin(), input.end());
    out.assign(input.begin(), input.end());

    // 加密前
    std::cout << "加密前: " << std::endl;
    for (auto i : in)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;

    AES aes(AESMode::AES_128);
    out = aes.encrypt_CFB(in, k, iv);
    std::cout << "加密结果: " << std::endl;
    for (auto i : out)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;
    // out转string
    std::string out_str(out.begin(), out.end());
    return out_str;

}

void buffer_en(const std::vector<unsigned char> &k, const std::vector<unsigned char> &iv, string input_filename, std::string output_name)
{
    std::vector<unsigned char> in{};
    std::vector<unsigned char> out{};

    std::cout << "=========== 大文件加密 ===========" << std::endl;

    AES aes(AESMode::AES_128);

    std::ifstream input_file(input_filename, std::ios::binary);
    std::ofstream output_file(output_name, std::ios::binary);

    char *buffer = new char[BUFFER_SIZE2];

    while (true)
    {
        input_file.read(buffer, BUFFER_SIZE2);
        std::streamsize size = input_file.gcount();
        std::cout << "加密大小: " << size << std::endl;
        if (size == 0)
        {
            break;
        }
        in.assign(buffer, buffer + size);
        out = aes.encrypt_CFB(in, k, iv);
        output_file.write(reinterpret_cast<const char *>(out.data()), out.size());
    }

    delete[] buffer;
    input_file.close();
    output_file.close();
}

int main()
{
    // 建立客户端流式套接字
    int sock_client;
    struct sockaddr_in server_addr;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    size_t msg_len{};

    // 设置客户端套接字,使用IPv4地址族,TCP协议
    // 事先设定服务器地址 130.33.40.2
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    memset((void *)&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 填写Bob的IP地址

    /***************   1.连接Bob  *********************/
    if (connect(sock_client, (struct sockaddr *)&server_addr, addr_len) == -1)
    {
        perror("connect failed");
        close(sock_client);
        return -1;
    }

    /***************   2.接受回应  *********************/
    int ret1_hello = recv(sock_client, buffer, BUFFER_SIZE, 0);
    if (ret1_hello == 0 || ret1_hello == -1)
    {
        perror("【连接失败】\n");
        close(sock_client);
        return -1;
    }

    std::string file_sig = get_file_hash_sig("1.txt");
    CA target = parse_cert("bob.txt");

    // 初始化aes key 和 iv
    std::vector<unsigned char> key{};
    std::vector<unsigned char> iv{};
    // 生成key 和 iv
    std::string key_iv_str = generate_aes_key_iv(key, iv);
    std::string k = encrypt_k(key_iv_str, target);
    cout << "[k len2] " << k.length() << endl;

    /***************  3.发送密钥  *********************/
    msg_len = k.length();
    send(sock_client, (char *)&msg_len, sizeof(msg_len), 0);

    if (send(sock_client, k.c_str(), k.length(), 0) == -1)
    {
        std::cout << "发送失败\n";
        close(sock_client);
        return 0;
    }

    std::cout << "[gen_key: ]";
    for (auto i : key)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;
    std::cout << "[gen_iv:  ] ";
    for (auto i : iv)
    {
        std::cout << std::hex << static_cast<int>(i);
    }
    std::cout << std::endl;

    std::string en_sig = enString(key, iv, file_sig);
    /***************  4.发送en_sig  *********************/
    msg_len = en_sig.length();
    send(sock_client, (char *)&msg_len, sizeof(msg_len), 0);

    cout << "[en_sig_len]" << en_sig.length() << endl;
    if (send(sock_client, en_sig.c_str(), en_sig.length(), 0) == -1)
    {
        std::cout << "发送失败\n";
        close(sock_client);
        return 0;
    }
    // 加密文件
    buffer_en(key, iv, "q.mkv", "alice_mid_aes");

    // 发送文件
    ifstream file("alice_mid_aes", ios::binary | ios::ate);
    if (!file)
    {
        perror("### 无法打开文件 ###");
        close(sock_client);
        return -1;
    }

    /***************  5.发送加密文件  *********************/
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // 发送文件大小;若大小为0,发送失败
    int size = send(sock_client, (char *)&fileSize, sizeof(fileSize), 0);
    if (size == -1)
    {
        perror("### 发送文件失败 ###");
        close(sock_client);
        return -1;
    }
    cout << "[filesize ] " << fileSize << endl;
    cout << "发送文件大小,成功" << endl;

    // 发送文件内容
    while (fileSize > 0)
    {
        size = file.readsome(buffer, BUFFER_SIZE);
        if (size == 0)
        {
            perror("### 发送文件失败 ###");
            break;
        }
        else
        {
            size = send(sock_client, buffer, size, 0);
            if (size == -1)
            {
                perror("### 发送文件失败 ###");
                break;
            }
            fileSize -= size;
        }
    }

    /***************  5.发送加密证书  *********************/
    std::ifstream cert_file("alice.txt");
    if (!cert_file)
    {
        perror("### 无法打开文件 ###");
        close(sock_client);
        return -1;
    }
    std::string cert_str((std::istreambuf_iterator<char>(cert_file)),
                         std::istreambuf_iterator<char>());
    cert_file.close();
    std::string en_cert = enString(key, iv, cert_str);

    msg_len = en_cert.length();
    send(sock_client, (char *)&msg_len, sizeof(msg_len), 0);
    cout << "[en_cert_len]" << en_cert.length() << endl;

    if (send(sock_client, en_cert.c_str(), en_cert.length(), 0) == -1)
    {
        perror("### 发送文件失败 ###");
        close(sock_client);
        return 0;
    }

    cout << "    文件发送完成" << endl;
    cout << "---------------- \n"
         << endl;

    // 关闭套接字
    close(sock_client);
    return 0;
}