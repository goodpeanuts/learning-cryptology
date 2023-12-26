/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-25 20:45:38
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-26 12:54:20
 * @FilePath: /learning-cryptology/hash/md5/main.c
 * @Description:
 *
 * Copyright (c) 2023 by goodpeanuts, All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "md5.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

const char *welcome = "1. 计算文件md5\n2. 计算字符串md5\n3. 退出\n请输入操作: ";

int getFileMD5(char *filename, char *dest)
{
	int i;
	int filelen = 0;
	int read_len;
	char temp[8] = {0};
	char hexbuf[4096] = {0};
	unsigned char decrypt[16] = {0};
	unsigned char decrypt32[64] = {0};
	MD5_CTX md5;
	char fw_path[128];

	int fdf;

	fdf = open(filename, O_RDWR);
	if (fdf < 0)
	{
		printf("%s not exist\n", filename);
		return -1;
	}

	MD5Init(&md5);
	while (1)
	{
		read_len = read(fdf, hexbuf, sizeof(hexbuf));
		if (read_len < 0)
		{
			close(fdf);
			return -1;
		}
		if (read_len == 0)
		{
			break;
		}
		filelen += read_len;
		MD5Update(&md5, (unsigned char *)hexbuf, read_len);
	}

	MD5Final(&md5, decrypt);
	strcpy((char *)decrypt32, "");

	for (i = 0; i < 16; i++)
	{
		sprintf(temp, "%02x", decrypt[i]);
		strcat((char *)decrypt32, temp);
	}
	strcpy(dest, decrypt32);

	printf("md5:%s len=%d\n", dest, filelen);
	close(fdf);

	return filelen;
}

void fileMD5(char *filename)
{
	int filelen;
	char md5_str[64] = {0};
	char cmd[256] = {0};

	filelen = getFileMD5(filename, md5_str);
	if (filelen < 0)
	{
		printf("fileMD5 fail\n");
	}
}

void strMd5(char *str)
{
	int read_len;
	int i;
	char temp[8] = {0};
	unsigned char digest[16];
	unsigned char decrypt[16] = {0};
	unsigned char decrypt32[64] = {0};

	MD5_CTX md5c;

	MD5Init(&md5c);
	read_len = strlen(str);
	MD5Update(&md5c, (unsigned char *)str, read_len);

	MD5Final(&md5c, decrypt);
	strcpy((char *)decrypt32, "");

	for (i = 0; i < 16; i++)
	{
		sprintf(temp, "%02x", decrypt[i]);
		strcat((char *)decrypt32, temp);
	}
	printf("md5:%s\n", decrypt32);

	return;
}

int main(int argc, char *argv[])
{
	int op = 0;
	while (1)
	{
		printf("%s", welcome);
		scanf("%d", &op);
		switch (op)
		{
		case 1:
		{
			char filename[128] = {0};
			printf("请输入文件名: ");
			scanf("%s", filename);
			fileMD5(filename);
			break;
		}
		case 2:
		{
			char str[128] = {0};
			printf("请输入字符串: ");
			scanf("%s", str);
			strMd5(str);
			break;
		}
		case 3:
		{
			printf("Bye!\n");
			exit(0);
		}
		default:
			printf("无效输入，请重新输入\n");
		}
	}
	return 0;
}
