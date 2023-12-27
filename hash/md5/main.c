/*
 * @Author: goodpeanuts goddpeanuts@foxmail.com
 * @Date: 2023-12-25 20:45:38
 * @LastEditors: goodpeanuts goddpeanuts@foxmail.com
 * @LastEditTime: 2023-12-27 13:20:19
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
