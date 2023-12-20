#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<string.h>
#include <math.h>
#include<algorithm>
using namespace std;
typedef long long ll;
// e是公钥
// d是私钥
 
ll e, d, n;
ll gcd(ll a, ll b)  //求最大公约数
{
    ll c = 0;
    if(a<b) swap(a,b);
    c = b;
    do
    {
        b = c;
        c = a%b;
        a = b;
    }
    while (c != 0);
    return b;
}
// 0不是 1是 
ll isPrime(ll i) //判断i是否是素数
{
    ll flag=0;
    for(ll a=2; a<i; a++)
    {
        if(i%a==0)
        {
            flag=1;
            break;
        }
    }
    if(flag==1) return 0;
    else return 1;
    
}
ll myPow(ll a, ll b, ll n)  //求a^b mod n
{
    ll y;
    /*使用二进制平方乘法计算 pow(a,b) % n*/
    y=1;
    while(b != 0)
    {
        /*对于b中的每个1，累加y*/
        if(b & 1)
            y = (y*a) % n;
        /*对于b中的每一位，计算a的平方*/
        a = (a*a) % n;
        /*准备b中的下一位*/
        b = b>>1;
    }
    return y;
    
}
void extgcd(ll a,ll b,ll& d,ll& x,ll& y) 
{
    if(!b)
    {
        d=a;
        x=1;
        y=0;
    }
    else
    {
        extgcd(b,a%b,d,y,x);
        y-=x*(a/b);
    }
}
ll ModularInverse(ll a,ll b)  //获取(1/a)mod b的结果
{
    ll d,x,y;
    extgcd(a,b,d,x,y);
    return d==1?(x+b)%b:-1;
    
}
void KeyGeneration()  //获取公钥密钥
{
    ll p, q;
    ll phi_n;
    do
    {
        do
            p = rand();
        while (p % 2 == 0);
    }
    while (!isPrime(p));  // 得到素数 p 
    do
    {
        do
            q = rand();
        while (q % 2 == 0);
    }
    while (!isPrime(q));  // 得到素数q 
    n = p * q;
    phi_n = (p - 1) * (q - 1);
    do
        e = rand() % (phi_n - 2) + 2; // 1 < e < phi_n
    while (gcd(e, phi_n) != 1);
    d = ModularInverse(e,phi_n);
}
// 一位一位地输出加密的结果 
ll Encryption(ll value)  //加密
{
    ll cipher;
    cipher = myPow(value, e, n);
    cout<<cipher;
    return cipher;
}
// 一位一位地输出解 密的结果 
void Decryption(ll value)  //解密
{
    ll decipher;
    decipher = myPow(value, d, n);
    cout<<decipher;
}
int main()
{
  /******
  
  对6位的数字进行稳定加密 
  ******/
  ll num;
  cout<<"请输入要加密的明文数字,ctrl+c结束"<<endl;
  while(cin>>num){
    ll de;
    cout<<"输入的明文为"<<num<<endl; 
      KeyGeneration();  //获取公钥密钥
      cout<<"加密密钥："<<e<<endl; 
      cout<<"加密结果为:";
    de = Encryption( num );
    cout<<"\n私钥为："<<d;
//    cout<<"de="<<de<<endl;
    cout<<"\n解密结果"; 
    Decryption(de); 
    cout<<"\n-------------"<<endl;
  }
    return 0;
}