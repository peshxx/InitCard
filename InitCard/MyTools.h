#pragma once
#include <string.h>
#include "dcrf32.h"


void asc_to_bcd(unsigned char *bcd_buf, unsigned char *ascii_buf,
	int conv_len, unsigned char type);

void bcd_to_asc(unsigned char *ascii_buf, unsigned char *bcd_buf,
	int conv_len, unsigned char type);

unsigned long HextoDec(const unsigned char *hex, int length);

int DectoHex(int dec, unsigned char *hex, int length);

//int CardCommand(HANDLE icdev, unsigned char* sendbuffer, int *rlen, unsigned char *recvbuffer);

//int ExAuthen(HANDLE icdev);

//int InAuthen(HANDLE icdev);
