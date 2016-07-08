#ifndef	__HSM_DEFS__
#define	__HSM_DEFS__

#define STX		0x02	/* Start of Sentinal */
#define ETX		0x03	/* End of Sentinal */

#define SEND_TIMEOUT		5
#define RECV_TIMEOUT		5

#define _MSGHD_LEN      0

#define MAX_MSGDATA     4096


#define S_LEN   16
#define D_LEN   (2*S_LEN)
#define T_LEN   (3*S_LEN)

#define i_k_LEN 4
#define I_K_LEN 3

#define ZMK_TYPE        "000"
#define ZPK_TYPE        "001"
#define TPK_TYPE        "002"
#define TAK_TYPE        "003"
#define ZAK_TYPE        "008"
#define ZEK_TYPE        "00A"

/*	Error Code Definitions		*/

#define	HSM_ERROR		(-1)
#define	HSM_OK			0
#define	HSM_ERR_SYSERR		(-999)

#define	HSM_ERR_OPEN		(-100)
#define	HSM_ERR_CLOSE		(-101)
#define	HSM_ERR_OPENED		(-102)
#define	HSM_ERR_CLOSED		(-103)

#define	HSM_ERR_INVALID		(-200)
#define	HSM_ERR_NOTEXIST	(-201)
#define	HSM_ERR_FILE		(-202)
#define HSM_ERR_MEMFULL		(-203)
#define	HSM_ERR_SOCK		(-204)
#define	HSM_ERR_WRITE		(-205)
#define	HSM_ERR_READ		(-206)
#define	HSM_ERR_LINE		(-207)
#define	HSM_ERR_TOOLONG		(-208)
#define	HSM_ERR_CHKSUM		(-209)
#define	HSM_ERR_IOCTL		(-210)
#define	HSM_ERR_SEND		(-211)
#define	HSM_ERR_RECV		(-212)
#define	HSM_ERR_UNKNOWN		(-213)

#define	HSM_ERR_TIMEOUT		(-300)
#define	HSM_ERR_BUSY		(-301)
#define	HSM_ERR_ZEROLEN		(-302)
#define	HSM_ERR_LENGTH		(-303)

#define	HSM_ERR_PUTMSGQ		(-400)
#define	HSM_ERR_GETMSGQ		(-401)
#define	HSM_ERR_INITMSGQ	(-402)
#define	HSM_ERR_GETQID		(-403)

#define	HSM_ERR_MSGHD		(-500)
#define	HSM_ERR_MSGTR		(-501)
#define	HSM_ERR_CMDRSP		(-502)
#define	HSM_ERR_RSPERR		(-503)

/*	End of Error Code Definitions	*/
#endif /* __HSM_DEFS__ */

