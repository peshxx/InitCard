#include "stdafx.h"
#include "MyTools.h"
#include "sjl22_ic_api.h"
#include "hsm_com.h"

void asc_to_bcd(unsigned char *bcd_buf, unsigned char *ascii_buf,
	int conv_len, unsigned char type)
{
	int    cnt;
	char   ch, ch1;

	if (conv_len & 0x01 && type) ch1 = 0;
	else ch1 = 0x55;
	for (cnt = 0; cnt < conv_len; ascii_buf++, cnt++) {
		if (*ascii_buf >= 'a') ch = *ascii_buf - 'a' + 10;
		else if (*ascii_buf >= 'A') ch = *ascii_buf - 'A' + 10;
		else if (*ascii_buf >= '0') ch = *ascii_buf - '0';
		else ch = 0;
		if (ch1 == 0x55) ch1 = ch;
		else {
			*bcd_buf++ = ch1 << 4 | ch;
			ch1 = 0x55;
		}
	}
	if (ch1 != 0x55) *bcd_buf = ch1 << 4;
}


void bcd_to_asc(unsigned char *ascii_buf, unsigned char *bcd_buf,
	int conv_len, unsigned char type)
{
	int cnt;

	if (conv_len & 0x01 && type) { cnt = 1; conv_len++; }
	else cnt = 0;
	for (; cnt < conv_len; cnt++, ascii_buf++) {
		*ascii_buf = ((cnt & 0x01) ? (*bcd_buf++ & 0x0f) : (*bcd_buf >> 4));
		*ascii_buf += ((*ascii_buf > 9) ? ('A' - 10) : '0');
	}
}

////////////////////////////////////////////////////////// 
// 
//功能：十六进制转为十进制 
// 
//输入：const unsigned char *hex         待转换的十六进制数据 
//      int length                       十六进制数据长度 
// 
//输出： 
// 
//返回：int  rslt                        转换后的十进制数据 
// 
//思路：十六进制每个字符位所表示的十进制数的范围是0 ~255，进制为256 
//      左移8位(<<8)等价乘以256 
// 
///////////////////////////////////////////////////////// 
unsigned long HextoDec(const unsigned char *hex, int length)
{
	int i;
	unsigned long rslt = 0;
	for (i = 0; i < length; i++)
	{
		rslt += (unsigned long)(hex[i]) << (8 * (length - 1 - i));

	}
	return rslt;
}

///////////////////////////////////////////////////////// 
// 
//功能：十进制转十六进制 
// 
//输入：int dec                     待转换的十进制数据 
//      int length                  转换后的十六进制数据长度 
// 
//输出：unsigned char *hex          转换后的十六进制数据 
// 
//返回：0    success 
// 
//思路：原理同十六进制转十进制 
////////////////////////////////////////////////////////// 
int DectoHex(int dec, unsigned char *hex, int length)
{
	int i;
	for (i = length - 1; i >= 0; i--)
	{
		hex[i] = (dec % 256) & 0xFF;
		dec /= 256;
	}
	return 0;
}





