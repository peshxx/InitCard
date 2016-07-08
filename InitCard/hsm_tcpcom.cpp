#include <stdio.h>
//#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <string.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//#include <pwd.h>
#include <signal.h>
//#include <dirent.h>
#include <errno.h>
#include <setjmp.h>

#include <stdlib.h>
//#include <dirent.h>
//#include <stdafx.h> 
#include <WinSock.h>
#include <windows.h>
#include <winbase.h>
#include <time.h>
#include <sys/timeb.h>

#include "stdafx.h"
#include "hsm_tcpsub.h"
#include "hsmcmd.h"


int HsmSendToSocket(int sockfd, unsigned char *buffer, int *length,int timeout)
{
	int	rc = -1;
	int	len = -1;
	struct 	timeval stTimeOut;
	fd_set	stSockReady;

	FD_ZERO( &stSockReady);
	FD_SET(sockfd, &stSockReady);


	if (timeout> 0) 
	{
		stTimeOut.tv_sec = timeout;
		stTimeOut.tv_usec = 0;

		select(sockfd+1, NULL, &stSockReady, NULL,&stTimeOut);
	} 
	else 
	{
		select(sockfd+1, NULL, &stSockReady, NULL,NULL);
	}

    if(!(FD_ISSET(sockfd,&stSockReady)))
	{
		rc = -1;

	} 
	else 
	{

		if((len = send(sockfd,(char*)buffer,*length,0))>0) 
		{
			rc = 0;
		}

		if(*length != len)	
		{
			*length = rc = -1;
			return rc;
		}

	}

	*length = len;

	return ( rc );

}

int comTcpSend(int sockfd, unsigned char *buffer, int *length,int timeout)
{
	return HsmSendToSocket(sockfd, buffer, length,timeout);
}


int HsmReceiveFromSocket(int sockfd, unsigned char *buffer, int *length,int timeout)
{
	int	rc = -1;
	int	rcvlen = -1;
	struct 	timeval stTimeOut;
	fd_set	stSockReady;

	FD_ZERO(&stSockReady);
	FD_SET(sockfd, &stSockReady);

 	
	if (timeout>0) 
	{
		stTimeOut.tv_sec = timeout;
		stTimeOut.tv_usec = 0;
		select(sockfd+1, &stSockReady, NULL,NULL,&stTimeOut);

	} 
	else 
	{
		select(sockfd+1, &stSockReady, NULL,NULL,NULL);
	}

    if( !(FD_ISSET(sockfd,&stSockReady)) )
	{
		rc = -1;
	} 
	else 
	{
		rcvlen = recv(sockfd,(char*)buffer,*length,0);

		if(rcvlen<=0)
		{
			rc = -1;
		} 
		else 
		{
			rc = 0;
		}
	}
	
	*length = rcvlen;

	return ( rc );

}

int comTcpReceive(int sockfd, unsigned char *buffer, int *length,int timeout)
{
	return HsmReceiveFromSocket(sockfd, buffer, length,timeout);
}

int comTcpCliSocketOpen( char *tcpaddr, int port,int timeout)
{
	int	sockfd;

	struct sockaddr_in serv_addr;
	struct linger tcp_linger;
	int    rc = 0;


	WSADATA  wsadata;

    if (WSAStartup(0x202, &wsadata))
	{
		return(INVALID_SOCKET);
	}


	memset (&serv_addr, 0x00,sizeof (struct sockaddr_in));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr (tcpaddr);
	serv_addr.sin_port = htons (port);

	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 ) 
	{
		rc = -1;
		return(rc);
	}

	rc = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in));

	if ( rc<0 ) 
	{
		rc = -1;
		return(rc);
	}

	tcp_linger.l_onoff	= 1; /* Linger On */
	tcp_linger.l_linger	= 0; /* 0 seconds */
	setsockopt(sockfd,SOL_SOCKET,SO_LINGER,(char *)&tcp_linger, sizeof(struct linger));

	return (sockfd);
}

int comTcpSvrSocketOpen(char *tcpaddr, int port)
{
	int	sockfd = -1;

	int	namelen;

	int	value;

	struct sockaddr_in servaddr ; 

	WSADATA  wsadata;

    if (WSAStartup(0x202, &wsadata))
	{
		return(INVALID_SOCKET);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		return(-1);
	}

	memset((char *)&servaddr,0x00,sizeof(struct sockaddr_in));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	value = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value)) < 0) 
	{
       	return(-1);
   	}


	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) < 0)
	{
		return(-1);
	}

	namelen = sizeof(struct sockaddr_in);

	if (listen(sockfd, SOMAXCONN) < 0)
	{
		return(-1);
	}

	return sockfd;

}

int comTcpAccept(int sockfd,char *alCliaddr)
{

	struct	sockaddr_in cliaddr;   

	int	consockfd = -1;
	struct	linger tcp_linger;

	int	namelen;
	int	value;

	namelen = sizeof(struct sockaddr_in);

	memset((char *)&cliaddr,0x00,namelen);

	if((consockfd = accept(sockfd,(struct sockaddr*)&cliaddr,&namelen))<0)
	{
		if( errno != EINTR ) return(-1);
	}

	tcp_linger.l_onoff = 1;
    tcp_linger.l_linger = 0;
    setsockopt(consockfd,SOL_SOCKET,SO_LINGER,(char *)&tcp_linger,sizeof(struct linger));

	value = 8192;
    setsockopt(consockfd,SOL_SOCKET,SO_SNDBUF,(char *)&value,sizeof(value));
    setsockopt(consockfd,SOL_SOCKET,SO_RCVBUF,(char *)&value,sizeof(value));

	return (consockfd);
}

int comTcpSocketClose(int sockfd)
{
        int  rc = 0;

        shutdown (sockfd, 2);

        if((rc = closesocket(sockfd))<0)
        {
             rc = -1;
        }

		WSACleanup();

        return ( rc );
}

int InitHsmDevice(char *tcpaddr, int port, int timeout)
{

	return comTcpCliSocketOpen(tcpaddr, port, timeout);

}
int CloseHsmDevice(int comid)
{
	return comTcpSocketClose(comid);
}
int HsmCmdRun(int comid, char *cmd, int cmdlen, char *rsp, int *rsplen)

{
	unsigned char   cmd_buf[MAX_MSGDATA + 1], *p;
	unsigned char   send_buf[MAX_MSGDATA + 1];
	unsigned char   ret_buf[MAX_MSGDATA + 1];

	int     cmd_len = 0, ret_len = 0, rc;


	/*
	// RACAL CMD = 2 Bytes Length + N bytes Message Header +
	// 2 bytes Command Code + CMD Messge
	*/
	p = cmd_buf;

	/*
	// 2 Bytes Length
	*/
	p = short2hex(cmdlen, p);

	memcpy(p, cmd, cmdlen);
	p += cmdlen;

	*p = 0;
	cmd_len = p - cmd_buf;

	memcpy(send_buf, cmd_buf, cmd_len);

#ifdef  __DEBUG__

	// TraceMessage("Command Request Buffer", (char *)send_buf, cmd_len);
#endif


	//cmd_len = MAX_MSGDATA;
	//if(cmd_len <2048)
	//cmd_len = 2048;

	rc = comTcpSend(comid, send_buf, &cmd_len, SEND_TIMEOUT);

	if (rc < 0)
	{
		return (HSM_ERR_SEND);
	}






	ret_len = sizeof(ret_buf);

	        if ((rc = comTcpReceive(comid,ret_buf,&ret_len, RECV_TIMEOUT)) < 0)	return(HSM_ERR_RECV);
	//if ((rc = comTcpReceive(comid, ret_buf, &ret_len)) < 0)	return (HSM_ERR_RECV);



	*(ret_buf + ret_len) = 0;


#ifdef  __DEBUG__

	// TraceMessage("Command Response buffer", (char *)ret_buf, ret_len);
#endif


	/*
	// Response: 2 Bytes Length + N bytes Message Header +
	// 2 bytes Command Response + 2 Bytes Error Code + Response Message
	*/

	*rsplen = (ret_len - (2 + _MSGHD_LEN + 2 + 2));

	*(rsp + *rsplen) = 0;

	/*
	// Check response length, if invalid ...
	*/
	if (ret_len != (int)(hex2short(ret_buf) + 2 /*bytes length*/))
	{
		return (HSM_ERR_LENGTH);
	}

	/*
	// Check message header, if consistent ...
	*/

	if (_MSGHD_LEN)
	{

		if (memcmp(cmd_buf + 2, ret_buf + 2, _MSGHD_LEN))
		{
			return (HSM_ERR_MSGHD);
		}
	}

	/*
	// Check Command Response, if invalid ...
	*/
	if ((cmd_buf[2 + _MSGHD_LEN + 1] + 1) != ret_buf[2 + _MSGHD_LEN + 1])
	{
		return (HSM_ERR_CMDRSP);
	}

	/*
	// Check response code, if no error ...
	*/
	if (!memcmp(&ret_buf[2 + _MSGHD_LEN + 2], "00", 2))
	{

		memcpy(rsp, (unsigned char *)&ret_buf[2 + _MSGHD_LEN + 2 + 2], *rsplen);

		return (0);

	}
	else
	{

		rc = (ret_buf[2 + _MSGHD_LEN + 2] - 48) * 10;

		rc = rc + (ret_buf[2 + _MSGHD_LEN + 3] - 48);

		rc = 0 - rc;

		//return (HSM_ERR_RSPERR);
		return (rc);
	}
}

//01, 400000, 0907101245,000006, 1001001001, 001,

//消费交易：01，金额：4, 000.00，日期时间：0907101245，流水号：000006，收费窗口号：1001001001，操作员号：001

int UnPack(char *tran_type, char *amount, char *datetime, char *tran_no, char *merchid, char *oper_id, char * buffer)
{
	char *p;

	p = strtok(buffer, ",");

	if (!p || strcmp(p, "01"))
		return 30;
	else
		strcpy(tran_type, p);


	p = strtok(NULL, ",");
	if (!p)
		return 30;
	else
		strcpy(amount, p);

	p = strtok(NULL, ",");
	if (!p || strlen(p) != 10)
		return 30;
	else
		strcpy(datetime, p);

	p = strtok(NULL, ",");
	if (!p || strlen(p) != 6)
		return 30;
	else
		strcpy(tran_no, p);

	p = strtok(NULL, ",");
	if (!p)
		return 30;
	else
		strcpy(merchid, p);

	p = strtok(NULL, ",");
	if (!p)
		return 30;
	else
		strcpy(oper_id, p);

	*p = 0x00;
	return 0;
}


int Pack(char *tran_type, char *resp_code, char *oper_id, char *merchid, char *card_no, char *amount, char *trace_no, char * buffer)
{
	char *p;
	p = buffer;
	memcpy(p, tran_type, strlen(tran_type));
	p += 2;
	*p++ = ',';

	memcpy(p, resp_code, strlen(resp_code));
	p += strlen(resp_code);
	*p++ = ',';

	memcpy(p, oper_id, strlen(oper_id));
	p += strlen(oper_id);
	*p++ = ',';

	memcpy(p, merchid, strlen(merchid));
	p += strlen(merchid);
	*p++ = ',';

	memcpy(p, card_no, strlen(card_no));
	p += strlen(card_no);
	*p++ = ',';

	memcpy(p, amount, strlen(amount));
	p += strlen(amount);
	*p++ = ',';

	memcpy(p, trace_no, strlen(trace_no));
	p += strlen(trace_no);
	*p++ = ',';

	*p = 0x00;

	//if (strlen(buffer) > 64 || strlen(buffer) < 1)
	//	return -1;
	return 0;
}

