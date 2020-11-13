/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : diag_tcp.c
  Description     :
  History         :
     1.w00182550       2013-2-21   Draft Enact

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/

#include "vos.h"
#if(VOS_WIN32 == VOS_OS_VER)
#include <winsock2.h>
#include <windows.h>
#else
#if (VOS_VXWORKS == VOS_OS_VER)
#include <sockLib.h>
#include <inetLib.h>
#include <selectLib.h>
#elif (VOS_LINUX == VOS_OS_VER)
#include "hisocket.h"
#endif
#endif
#include <msp_errno.h>
#include <msp_cfg.h>
#include <osm.h>
#include "diag_debug.h"
#include "SCMProc.h"
#include "diag_port.h"

/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
typedef struct
{
	VOS_UINT32	d_ino;
	VOS_UINT32	d_off;
	VOS_UINT16	d_reclen;
	VOS_UCHAR  d_name[1];
}DMS_LINUX_DIRENT;


typedef struct
{
	VOS_SOCKET	  Tcp;	   // TCP socket ID for the channel;
	VOS_SOCKET	  listener;// TCP socket ID for the channel;
	VOS_UINT32 aulPortNum;
	VOS_UINT32 hSem;
	VOS_UINT32 ulConnectSta;
	VOS_UINT32 ulIsReCon;
} DIAG_TCPIP_PORT_STRU;


typedef struct
{
	DIAG_TCPIP_PORT_STRU astTcpIpPort[EN_TCP_PORT_FULL];
} DIAG_TCPIP_CTRL_INFO_STRU;

/*TCP */
#define INVALID_SOCKET  ERROR
#define SOCKET_ERROR    ERROR
#define closesocket(s)  shutdown(s, SHUT_RDWR)

#define IP_NULL         0
#define Ip_sockaddr     sockaddr
#define Ip_socklen_t    int
#define Ip_fd           int
#define Ip_fd_set       hi_fd_set
#define Ip_timeval      timeval

#define FD_ISSET(d, set)     (HI_FD_ISSET(d, set))
#define FD_SET(d, set)       (HI_FD_SET(d, set))
#define FD_CLR(d, set)       (HI_FD_CLR(d, set))
#define FD_ZERO(set)         (HI_FD_ZERO(set))

#define htons(xx)       (unsigned short)(((((xx) << 8) & 0xff00) | (((xx) >> 8) & 0x00ff)))

#define socket(domain,type,protocol)        hi_socket(domain, type, protocol)
#define shutdown(fd,how)                    do{\
													hi_shutdown((Ip_fd)fd, how);\
													hi_close((Ip_fd)fd);\
												}while(0)
#define bind(fd,addr,addrlen)               hi_bind((Ip_fd)fd, (struct Ip_sockaddr *)addr, (Ip_socklen_t)addrlen)
#define accept(fd,addr,addrlenp)            hi_accept((Ip_fd)fd, (struct Ip_sockaddr *)addr, (Ip_socklen_t *)addrlenp)
#define listen(fd,backlog)                  hi_listen((Ip_fd)fd, backlog)
#define select(nfds,rf,wf,ef,t)             hi_select(nfds, (Ip_fd_set *)rf, (Ip_fd_set *)wf, IP_NULL, t)
#define recv(fd,buf,len,flags)              hi_recv((Ip_fd)fd, (void *)buf, len, flags)
#define send(fd,msg,len,flags)              hi_send((Ip_fd)fd, (void *)msg, len, flags)
#define inet_addr(cp)                       hi_inet_addr(cp)

#if (VOS_WIN32== VOS_OS_VER)
	static DIAG_TCPIP_CTRL_INFO_STRU g_stDiagTcpIpInfo = {
		{{INVALID_SOCKET,INVALID_SOCKET,20248,NULL,0},{INVALID_SOCKET,INVALID_SOCKET,20249,NULL,0}}
	};
#else
	static DIAG_TCPIP_CTRL_INFO_STRU g_stDiagTcpIpInfo = {{INVALID_SOCKET,INVALID_SOCKET,20248,NULL,0}};
#endif
static VOS_UINT8 g_aucRecvBuf[DIAG_PORT_READ_LEN] = {0,};

VOS_UINT32 g_diagTcpPortLock =0,g_diagTcpPortUnLock=0;
VOS_INT32 g_fdCount=0;
VOS_UINT32 g_dmsPrintFlag=0;


/*****************************************************************************
  3 Function
*****************************************************************************/
VOS_UINT32 diag_GetTcpPort(VOS_UINT32 ulPort[EN_TCP_PORT_FULL])
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;

    ulPort[EN_TCP_PORT_DIAG] = 20248;
    ulPort[EN_TCP_PORT_AT]   = 20249;

    return ret;
}

DIAG_TCPIP_CTRL_INFO_STRU* diag_GetTcpInfo(VOS_VOID)
{
    return &g_stDiagTcpIpInfo;
}


VOS_VOID diag_SocketInfo_Init(VOS_VOID)
{
	//DIAGͨ��
	g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ucChanStat         =ACM_EVT_DEV_SUSPEND ;
	g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ucHdlcFlag         = EN_HDLC_DATA;
	g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ulCodeDesChanId    = SOCP_CODER_DST_LOM_IND;
	g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ulDecodeSrcChanId  =SOCP_DECODER_SRC_LOM ;
	g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ulRecivBufLen      = 0;
    g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].slPortHandle       = UDI_INVALID_HANDLE;
}


VOS_UINT32 diag_SocketInit(VOS_VOID)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT8 ulTcpPort = 0;
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;
    VOS_UINT32 aulPort[EN_TCP_PORT_FULL] = {0,};

    diag_SocketInfo_Init();

    if(BSP_MODULE_SUPPORT != DRV_GET_WIFI_SUPPORT())
    {
        return ERR_MSP_FAILURE;
    }

    diag_GetTcpPort(aulPort);

    pstDiagTcpInfo = diag_GetTcpInfo();

    for (ulTcpPort = 0; ulTcpPort < EN_TCP_PORT_FULL; ulTcpPort++)
    {
        pstDiagTcpInfo->astTcpIpPort[ulTcpPort].listener= INVALID_SOCKET;
        pstDiagTcpInfo->astTcpIpPort[ulTcpPort].Tcp = INVALID_SOCKET;

        VOS_SmBCreate(NULL, 1, VOS_SEMA4_FIFO,&pstDiagTcpInfo->astTcpIpPort[ulTcpPort].hSem);

        pstDiagTcpInfo->astTcpIpPort[ulTcpPort].aulPortNum = aulPort[ulTcpPort];
        if (pstDiagTcpInfo->astTcpIpPort[ulTcpPort].hSem == (VOS_UINT32)NULL)
        {
            ret = ERR_MSP_FAILURE;
            break;
        }
    }
    DIAG_PORT_INIT_STATE_SWITCH(EN_DIAG_SOCKET_BEARER_TCP_20248,EN_PORT_INIT_SUCC);

    return ret;
}


VOS_VOID diag_TcpPortLock(DIAG_TCPIP_PORT_ENUM enTcpPort)
{
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;

    pstDiagTcpInfo = diag_GetTcpInfo();

    VOS_SmP(pstDiagTcpInfo->astTcpIpPort[enTcpPort].hSem,0);
	g_diagTcpPortLock ++;

}

VOS_VOID diag_TcpPortUnLock(DIAG_TCPIP_PORT_ENUM enTcpPort)
{
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;

    pstDiagTcpInfo = diag_GetTcpInfo();

    VOS_SmV(pstDiagTcpInfo->astTcpIpPort[enTcpPort].hSem);

	g_diagTcpPortUnLock++;
}


static VOS_INT32 diag_ListenDoneNeedAcceptSet(Ip_fd_set *fdListen, VOS_SOCKET *maxSocket, DIAG_TCPIP_PORT_STRU *astTcpPort)
{
    VOS_INT32 fd_count = 0;
    VOS_UINT8 ulTcpPort;

    /* initialize socket set to monitor incoming connections */
    *maxSocket = 0;
    for (ulTcpPort = 0; ulTcpPort < EN_TCP_PORT_FULL; ulTcpPort++)
    {
        if (astTcpPort[ulTcpPort].listener == INVALID_SOCKET)  /* invalid listener */
        {
            continue;
        }

        /* add listener to monitor list*/
        DIAG_FD_SET(astTcpPort[ulTcpPort].listener, fdListen);
        fd_count++;
        if (*maxSocket < astTcpPort[ulTcpPort].listener)
        {
            *maxSocket = astTcpPort[ulTcpPort].listener;
        }
    }

    return fd_count;
}
static VOS_INT32 diag_AcceptDoneNeedRecvSet(Ip_fd_set *fdReceive, VOS_SOCKET *maxSocket, DIAG_TCPIP_PORT_STRU *astTcpPort)
{
    VOS_INT32 fd_count = 0;
    VOS_UINT8 ucTcpIpPort;

    /* initialize socket set to monitor incoming traffic*/
    *maxSocket = 0;
    for (ucTcpIpPort = 0; ucTcpIpPort < EN_TCP_PORT_FULL; ucTcpIpPort++)
    {
        if (astTcpPort[ucTcpIpPort].Tcp == INVALID_SOCKET)
        {
            continue;
        }

        /* add socket id to the reading set*/
        DIAG_FD_SET(astTcpPort[ucTcpIpPort].Tcp, fdReceive);
        fd_count++;
        if (*maxSocket < astTcpPort[ucTcpIpPort].Tcp)
        {
            *maxSocket = astTcpPort[ucTcpIpPort].Tcp;
        }
    }

    return fd_count;
}



static VOS_VOID diag_AcceptOnSocket(DIAG_TCPIP_PORT_ENUM ucTcpPort , DIAG_TCPIP_PORT_STRU *astTcpPort)
{
    struct sockaddr_in from;
    VOS_SOCKET newSocket;
    VOS_SOCKET *ListenerSocket;
    VOS_INT32 len;
    DIAG_PORT_CONNECT_STA_PFN pfnConn = DIAG_PORT_GET_CONN_CALLBACK();

    ListenerSocket = &(astTcpPort[ucTcpPort].listener);
    len = sizeof(struct sockaddr_in);
    newSocket = accept(*ListenerSocket, (struct sockaddr *)&from, (int *)&len);

    if (INVALID_SOCKET == newSocket)
    {
        closesocket(*ListenerSocket);
        *ListenerSocket = INVALID_SOCKET;
		diag_printf("diag_AcceptOnSocket INVALID_SOCKET == newSocket!\n");
        return;
    }

    /*����Diagͨ����������������*/
    else
    {
        if(astTcpPort[ucTcpPort].Tcp !=INVALID_SOCKET)
        {
            closesocket(astTcpPort[ucTcpPort].Tcp);
            astTcpPort[ucTcpPort].Tcp = INVALID_SOCKET;
            astTcpPort[ucTcpPort].ulIsReCon= 1;
			diag_printf("diag_AcceptOnSocket astTcpPort[ucTcpPort].ulIsReCon= 1;!\n");
            DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_TCP_RECONNECT, newSocket, 0, 0);
        }
    }

    DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_ACP, newSocket, 0, 0);

    diag_TcpPortLock(ucTcpPort);
    astTcpPort[ucTcpPort].Tcp = newSocket;
    diag_TcpPortUnLock(ucTcpPort);

#if (VOS_WIN32== VOS_OS_VER)
    /* close listening socket (allow one connection per channel)*/
  if(ucTcpPort == EN_TCP_PORT_AT)
    {
        closesocket(*ListenerSocket);
        *ListenerSocket = INVALID_SOCKET;
    }
#endif

    /* ��¼������״̬*/
	if(ucTcpPort == EN_TCP_PORT_DIAG)
	{

        DIAG_PORT_PORT_SWITCH(EN_DIAG_SOCKET_BEARER_TCP_20248);
        if (pfnConn != NULL)
        {
            pfnConn(DIAG_CONN);
			diag_printf("diag_AcceptOnSocket pfnConn(EN_DMS_CHANNEL_DIAG, DIAG_DISCONN)!\n");
        }
    }

    /* ��¼������״̬*/
    astTcpPort[ucTcpPort].ulConnectSta = DIAG_CONN;
}


VOS_VOID diag_CloseSocket(DIAG_TCPIP_PORT_ENUM enTcpPort)
{
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;

    pstDiagTcpInfo = diag_GetTcpInfo();

    diag_TcpPortLock(enTcpPort);

    if (INVALID_SOCKET != pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp)
    {
        shutdown(pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp, SHUT_RDWR);
        closesocket(pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp);

        DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_CLOSE, pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp, 0, 0);

        pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp = INVALID_SOCKET;
    }

    diag_TcpPortUnLock(enTcpPort);
}




static VOS_VOID diag_TcpRcvData(VOS_SOCKET rxSocket, DIAG_TCPIP_PORT_ENUM enTcpPort,
                         DIAG_PORT_CONNECT_STA_PFN pfnConn)
{
	DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;
    VOS_INT32 len = -1;
    VOS_UINT32 Ret;

	DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_RECV, len, 0, 0);

    len = recv((int) rxSocket, (char*)g_aucRecvBuf, (int) DIAG_PORT_READ_LEN, 0);
    if ((SOCKET_ERROR == len) || (0 == len))
    {
    	diag_printf("diag_TcpRcvData  recv error !len=%d \n", len);
        DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_RECV_ERROR, len, 0, 0);
        diag_CloseSocket(enTcpPort);


        /* reset channel port connect as disconnect,when recv() error*/
        if (enTcpPort == EN_TCP_PORT_DIAG)
        {
        	DIAG_PORT_PORT_SWITCH(EN_DIAG_SOCKET_BEARER_TCP_20248);
            if (pfnConn != NULL)
            {
            	diag_printf("diag_TcpRcvData pfnConn(EN_DMS_CHANNEL_DIAG, DIAG_DISCONN) \n");
                pfnConn(DIAG_DISCONN);
            }
        }

		pstDiagTcpInfo = diag_GetTcpInfo();
		pstDiagTcpInfo->astTcpIpPort[enTcpPort].ulConnectSta = DIAG_DISCONN;
        return;
    }
    else
    {
        if(g_dmsPrintFlag == 1)
        {
    	    diag_printf("diag_TcpRcvData  recv ok ! len=%d ulTcpPort=%d \n" , len , enTcpPort);
        }
        /* send recv data to cdm decoder*/
        if (enTcpPort == EN_TCP_PORT_DIAG)
        {
           if(g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20248].ucHdlcFlag ==1)
            {


				Ret = SCM_SendDecoderSrc(g_diagPort[EN_DIAG_USB_BEARER_DIAG_CTRL].ulDecodeSrcChanId,g_aucRecvBuf,len );
                if(g_dmsPrintFlag == 1)
                {
				    diag_printf("diag_TcpRcvData  111 Ret= %d \n" , Ret);
                }
                return ;
            }
        }

#if (VOS_WIN32== VOS_OS_VER)
        else if (enTcpPort == EN_TCP_PORT_AT)
        {
            if(g_diagPort[EN_DIAG_SOCKET_BEARER_TCP_20249].ucHdlcFlag ==1)
            {
                return ;
            }
			else
			{
			}
        }
#endif
    }
}


VOS_UINT32 diag_TcpSendData(DIAG_TCPIP_PORT_ENUM enTcpPort,VOS_UINT8 *pcData, VOS_UINT32 usDataLen, VOS_UINT32 * pusWrtLen)
{
    VOS_INT32 len = SOCKET_ERROR;
    DIAG_TCPIP_CTRL_INFO_STRU* pstTcpIpInfo = diag_GetTcpInfo();
    DIAG_PORT_CONNECT_STA_PFN pfnConn = DIAG_PORT_GET_CONN_CALLBACK();

	DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_SEND, enTcpPort,len, 0);

    if(pstTcpIpInfo->astTcpIpPort[enTcpPort].Tcp ==INVALID_SOCKET)
    {
    	diag_printf("diag_TcpSendData invalid socket!\n");
		DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_SEND_ERROR, len, 0, 0);
        return ERR_MSP_FAILURE;
    }

	SCM_INVALID_CACHE(pcData , usDataLen);
	len = send((int)(pstTcpIpInfo->astTcpIpPort[enTcpPort].Tcp), (VOS_CHAR*)pcData, (int)usDataLen, 0);
    if(g_dmsPrintFlag == 1)
    {
       diag_printf("***********diag_TcpSendData !! usDataLen = %d,enTcpPort=%d,pcData = 0x%x\n" ,usDataLen ,enTcpPort, pcData);
       diag_printf("***********diag_TcpSendData !! len=%d,astTcpIpPort[enTcpPort].Tcp=%d \n" ,len,pstTcpIpInfo->astTcpIpPort[enTcpPort].Tcp);
    }

    if (SOCKET_ERROR == len)
    {
        /*���������֮��ĵ�һ�η��ͣ���������������ȵ�ԭ����socket��ȡ����
 */
        /*������󲻿��Թر�socket��
 */
        if(pstTcpIpInfo->astTcpIpPort[enTcpPort].ulIsReCon==1)
        {
            diag_printf("pstTcpIpInfo->astTcpIpPort[enTcpPort].ulIsReCon==1\n");
            DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_SEND_ERROR, len, 0, 1);
            return ERR_MSP_FAILURE;
        }

		/*�ر�socket
 */
        diag_CloseSocket(enTcpPort);

		/*������״̬Ϊ�Ͽ�
 */
        if (enTcpPort == EN_TCP_PORT_DIAG)
        {
        	DIAG_PORT_PORT_SWITCH(EN_DIAG_SOCKET_BEARER_TCP_20248);
            if (pfnConn != NULL)
            {
            	DIAG_DEBUG_SDM_FUN(EN_DIAG_TCP_SOCKET_SEND_ERROR, len, 0, 2);
                pfnConn(DIAG_DISCONN);
            }
        }

		pstTcpIpInfo = diag_GetTcpInfo();
		pstTcpIpInfo->astTcpIpPort[enTcpPort].ulConnectSta = DIAG_DISCONN;
        return ERR_MSP_FAILURE;
    }

    /*�����ķ���ʧ�ܾͻ�ֱ�ӹر�socket
 */
    pstTcpIpInfo->astTcpIpPort[enTcpPort].ulIsReCon = 0;

    *pusWrtLen = len;

    return ERR_MSP_SUCCESS;
}


 VOS_VOID diag_BindListenTcpSocket(VOS_VOID)
{
    struct sockaddr_in local;
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;
    VOS_UINT32 ulTcpPort = 0;
	VOS_SOCKET *ListenerSocket=NULL;
	VOS_SOCKET *TcpSocket=NULL;

    pstDiagTcpInfo = diag_GetTcpInfo();
	ListenerSocket = &((pstDiagTcpInfo->astTcpIpPort[ulTcpPort]).listener);
	TcpSocket = &((pstDiagTcpInfo->astTcpIpPort[ulTcpPort]).Tcp);

    /* create listener sockets if necessary*/
    /*diag_printf("BindListenTcpSocket \n");*/
    for (ulTcpPort = 0; ulTcpPort < EN_TCP_PORT_FULL; ulTcpPort++)
    {
        if ((*ListenerSocket != INVALID_SOCKET) ||
            ((*TcpSocket != INVALID_SOCKET)))
        {
        	diag_printf("BindListenTcpSocket 11111\n");
            continue;
        }

        /* create listener socket*/
        *ListenerSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == *ListenerSocket)
        {
        	diag_printf("BindListenTcpSocket 2222222\n");
            continue;
        }

        if(g_dmsPrintFlag == 1)
        {
		    diag_printf("BindListenTcpSocket aulPortNum = %d \n" , (VOS_UINT16)(pstDiagTcpInfo->astTcpIpPort[ulTcpPort]).aulPortNum);
        }

        /* initialize socket address */
        local.sin_family      = AF_INET;
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        local.sin_port        = htons((VOS_UINT16)((pstDiagTcpInfo->astTcpIpPort[ulTcpPort]).aulPortNum));
        /*local.sin_len 		  = sizeof(struct sockaddr_in);*/

        /* bind listener socket*/
        if (bind(*ListenerSocket, (struct sockaddr*)&local, sizeof(struct sockaddr_in)) ==
            SOCKET_ERROR)
        {
        	diag_printf("BindListenTcpSocket 333333333333\n");
            closesocket(*ListenerSocket);
            *ListenerSocket = INVALID_SOCKET;
            continue;
        }

        /* listen on listener socket*/
        if (listen(*ListenerSocket, 1) == SOCKET_ERROR)
        {
        	diag_printf("BindListenTcpSocket 44444444444444\n");
            closesocket(*ListenerSocket);
            *ListenerSocket = INVALID_SOCKET;
            continue;
        }
    }
}
 VOS_VOID diag_AcceptRecvFromSocket(VOS_VOID)
{
    VOS_SOCKET maxAcceptSock;
    VOS_SOCKET maxRecvSock;
    VOS_SOCKET maxSocket;
    VOS_SOCKET rxSocket;
    Ip_fd_set fdRxSet,ReadySet,ErrorSet;
    VOS_INT32 ret;
    DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;
    DIAG_TCPIP_PORT_ENUM enTcpPort = 0;
    DIAG_PORT_CONNECT_STA_PFN pfnConn = DIAG_PORT_GET_CONN_CALLBACK();

    pstDiagTcpInfo = diag_GetTcpInfo();

    DIAG_FD_ZERO(&fdRxSet);
	DIAG_FD_ZERO(&ReadySet);
    g_fdCount = diag_ListenDoneNeedAcceptSet(&fdRxSet, &maxAcceptSock, pstDiagTcpInfo->astTcpIpPort);
    g_fdCount += diag_AcceptDoneNeedRecvSet(&fdRxSet, &maxRecvSock, pstDiagTcpInfo->astTcpIpPort);
    if (0 == g_fdCount)
    {
        /* socket set empty, nothing to be monitored*/
        diag_printf("diag_AcceptRecvFromSocket 0 ==fd_count! \n");
        return;
    }

    /* monitor incoming connection/data for TCP sockets*/
    maxSocket = (maxAcceptSock > maxRecvSock) ? maxAcceptSock : maxRecvSock;

	/*select�����Ӵ�fdRxSet��ѡȡ�Ѿ�׼���ÿɶ�������������ReadySet*/
    memcpy(&ReadySet, &fdRxSet, sizeof(Ip_fd_set));
    memcpy(&ErrorSet, &fdRxSet, sizeof(Ip_fd_set));
    ret = select((int) maxSocket + 1, &ReadySet, NULL, &ErrorSet, NULL);
    if (SOCKET_ERROR == ret)
    {
    	diag_printf("diag_AcceptRecvFromSocket 111 \n");
        return;
    }

    /* accept/receive on TCP sockets*/
    for (enTcpPort = 0; enTcpPort < EN_TCP_PORT_FULL; enTcpPort++)
    {
        /* check and accept incoming connection*/
        rxSocket = pstDiagTcpInfo->astTcpIpPort[enTcpPort].listener;
        if ((rxSocket != INVALID_SOCKET) && (DIAG_FD_ISSET(rxSocket, &ReadySet)))
        {
            if(g_dmsPrintFlag == 1)
            {
			    diag_printf("diag_AcceptRecvFromSocket acceptOnSocket \n");
            }
	     	diag_AcceptOnSocket(enTcpPort, pstDiagTcpInfo->astTcpIpPort);
            continue;  /* either accept() or recv(), not both*/
        }

        /* check and receive incoming traffic */
        rxSocket = pstDiagTcpInfo->astTcpIpPort[enTcpPort].Tcp;
        if ((rxSocket != INVALID_SOCKET) && (DIAG_FD_ISSET(rxSocket, &ReadySet)))
        {
            if(g_dmsPrintFlag == 1)
            {
        	    diag_printf("diag_AcceptRecvFromSocket tcpRcvData \n");
            }
            diag_TcpRcvData(rxSocket, enTcpPort, pfnConn);
        }
    }
}

VOS_VOID diag_SetPrintFlag(VOS_UINT32 Flag)
{
    g_dmsPrintFlag = Flag;
}

VOS_VOID diag_TcpHelp(VOS_VOID)
{
	DIAG_TCPIP_CTRL_INFO_STRU *pstDiagTcpInfo;
	pstDiagTcpInfo = diag_GetTcpInfo();

    printk("��ӡʹ�ܿ��� .................=%d\n",     (VOS_INT)g_dmsPrintFlag);

	printk("TCP�˿������� ................=%d\n",     (VOS_INT)g_diagTcpPortLock);
	printk("TCP�˿ڽ�������...............=%d\n",     (VOS_INT)g_diagTcpPortUnLock);
	printk("fdcount.......................=%d\n",     (VOS_INT)g_fdCount);

	printk("DIAG�˿�����״̬: ............=%d\n",     (VOS_INT)pstDiagTcpInfo->astTcpIpPort[EN_TCP_PORT_DIAG].ulConnectSta);
	printk("DIAG�˿�����״̬: ............=%d\n",     (VOS_INT)pstDiagTcpInfo->astTcpIpPort[EN_TCP_PORT_DIAG].ulIsReCon);

	printk("DIAG��Ӧ�˿ں�: ..............=%d\n",     (VOS_INT)pstDiagTcpInfo->astTcpIpPort[EN_TCP_PORT_DIAG].aulPortNum);

	printk("DIAG�˿� listen ������:.......=%d\n",     (VOS_INT)pstDiagTcpInfo->astTcpIpPort[EN_TCP_PORT_DIAG].listener);
	printk("DIAG�˿� accept ������:.......=%d\n",     (VOS_INT)pstDiagTcpInfo->astTcpIpPort[EN_TCP_PORT_DIAG].Tcp);

}










#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif






/*_END_C_FILE_
 */

