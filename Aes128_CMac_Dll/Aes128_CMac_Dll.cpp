// Aes128_CMac_Dll.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "cmac.h"
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include "aes.h"
#include "Aes128_CMac_Dll.h"

void challenge_identification(uint8 appkey[APPKEY_LEN], uint64 appeui, uint32 appnonce, char*challenge)
{
	uint8 *ptmp = NULL;
	uint8 msg[16] = { 0 };
	uint8 tmp_challenge[16] = { 0 };
	//schar challenge[32 + 1] = { '\0' };
	AES_CMAC_CTX cmacctx;
	uint8 i, j;
	uint8 high, low;
	uint8 flag = 0;

	/*
	* 生成msg
	* msg = appeui|appnonce|0 (注：msg最后补32位的0，成128bit信息块)
	*/
	ptmp = (uint8 *)(&appeui);
	for (i = 0; i<8; i++)
	{
		msg[i] = ptmp[8 - 1 - i];
	}
	ptmp = (uint8 *)(&appnonce);
	for (i = 0; i<4; i++)
	{
		msg[i + 8] = ptmp[4 - 1 - i];
	}
	for (i = 0; i<4; i++)
	{
		msg[i + 12] = 0;
	}

	/*
	* aes_cmac 加密生成挑战字
	*/
	AES_CMAC_Init(&cmacctx);
	AES_CMAC_SetKey(&cmacctx, appkey);
	AES_CMAC_Update(&cmacctx, msg, 16);
	AES_CMAC_Final(tmp_challenge, &cmacctx);
	/*
	* 将挑战字转化为十六进制整数表示的字符串，(十六进制数字的字母部分采用大写，前面不加"0x")
	*/
	j = 0;
	for (i = 0; i<16; i++)
	{
		high = ((tmp_challenge[15 - i])&(0xF0)) >> 4;
		low = (tmp_challenge[15 - i])&(0x0F);
		if ((0 == flag) && ((0 == high) && (0 == low)))
		{
			continue;
		}
		if ((0 == flag) && (0 == high) && (0 != low))
		{
			if (low <= 9)
			{
				challenge[j] = '0' + low;
			}
			else
			{
				challenge[j] = 'A' + (low - 10);
			}
			j++;
			flag = 1;
			continue;
		}
		flag = 1;
		if (high <= 9)
		{
			challenge[j] = '0' + high;
		}
		else
		{
			challenge[j] = 'A' + (high - 10);
		}
		j++;
		if (low <= 9)
		{
			challenge[j] = '0' + low;
		}
		else
		{
			challenge[j] = 'A' + (low - 10);
		}
		j++;
	}
	challenge[32] = '\0';
#if 0
	FILE *fp;
	if (fp = fopen("challenge.dat", "wb"))
	{
		fwrite(challenge, 32, 1, fp);
		fclose(fp);
	}
	//memcpy(OutChallenge, challenge, 32);
#endif
}