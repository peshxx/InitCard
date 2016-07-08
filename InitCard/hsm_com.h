#ifndef __HSM_COM__
#define __HSM_COM__

#define __TCP_MODE__

#include "hsmcmd.h"
#include "hsmdefs.h"
#include "hsm_tcpsub.h"

int InitHsmDevice(char *tcpaddr,int port,int timeout);

int CloseHsmDevice(int comid);

int HsmCmdRun(int comid,char *cmd, int cmdlen, char *rsp, int *rsplen);


#endif 

