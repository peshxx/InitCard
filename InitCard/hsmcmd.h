#ifndef	__SJL22X_HSM__
#define	__SJL22X_HSM__

#include "hsmdefs.h"

#define HSM_MAX_SENDBUF		4096
#define HSM_MAX_RECVBUF		4096

int UnpackBCD( unsigned char *InBuf,  char *OutBuf, int Len );

int PackBCD( char *InBuf, unsigned char *OutBuf, int Len );

unsigned char *int_to_byte2(int i,unsigned char *p);

unsigned char *short2hex(unsigned short s, unsigned char *p);

unsigned char *long2hex(unsigned long l, unsigned char *p);

unsigned long hex2long(unsigned char *p);

unsigned short hex2short(unsigned char *p);

void set_msg_ht(char *buf, long pid, int len);

char *strupper(char *str);

int CheckIP(char *IP);

int CheckNum(char *str);

char HexToASCII(int ch);

#endif

