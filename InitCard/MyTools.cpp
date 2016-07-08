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
//���ܣ�ʮ������תΪʮ���� 
// 
//���룺const unsigned char *hex         ��ת����ʮ���������� 
//      int length                       ʮ���������ݳ��� 
// 
//����� 
// 
//���أ�int  rslt                        ת�����ʮ�������� 
// 
//˼·��ʮ������ÿ���ַ�λ����ʾ��ʮ�������ķ�Χ��0 ~255������Ϊ256 
//      ����8λ(<<8)�ȼ۳���256 
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
//���ܣ�ʮ����תʮ������ 
// 
//���룺int dec                     ��ת����ʮ�������� 
//      int length                  ת�����ʮ���������ݳ��� 
// 
//�����unsigned char *hex          ת�����ʮ���������� 
// 
//���أ�0    success 
// 
//˼·��ԭ��ͬʮ������תʮ���� 
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





