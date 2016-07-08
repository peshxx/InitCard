#ifndef ISO8583_H_
#define ISO8583_H_

struct	ISO_8583 {		/* ISO 8583 Message Structure Definitions */
	int    len;		/* data element max length */
	unsigned char	 type;	 /* bit 0--x, bit 1--n, bit 2--z bit 3--b*/
	unsigned char	 flag;	 /* length field length */
};

struct len_str {
	short  len;
	char   *str;
};

struct data_element_flag {
	short bitf;
	short len;
	char  *addr;
};

typedef struct	{
	struct	data_element_flag f[128];
	short	off;
	char	dbuf[1024];
	char	message_id[10];
} ISO_data;

struct element_struc {
	short bitno;	       /* element no */
	short type;	       /* 0--default value, 1,2--process function */
	short len;	       /* when type=0, len=length of defaule value */
	void  *pointer;        /* pointer of value or process function */
};

struct	trans_bitmap_struc {
	short trans_type;		 /* transaction type */
	char  message_id[10];
	short element_number;		 /* number of elememts */
	struct element_struc *element;	 /* transaction element ally */

};

int strtoiso(char *, ISO_data *);
int isotostr(char *, ISO_data *);
int setbitic(ISO_data *, int, char *, int);
int getbit(ISO_data *, int, char *);
void clearbit(ISO_data *);
void asc_to_bcd(unsigned char *, unsigned char *, int, unsigned char);
void bcd_to_asc(unsigned char *, unsigned char *, int, unsigned char);

#endif
