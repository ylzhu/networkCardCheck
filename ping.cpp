//  
// THIS CODE IS BASED ON THE CODE FROM   
// THE BOOK WINSOCK 2.0 BY LEWIS NAPPER...  
//   
//  
  
#include "stdafx.h"  
#include "ping.h"  
  
  
BOOL   CPing::Ping(UINT   nNumIp, CString   pstrHost, CString &m_Msg )   
{  
    SOCKET    rawSocket;  
    LPHOSTENT lpHost;  
    UINT      nLoop;  
    int       nRet;  
    struct    sockaddr_in saDest;  
    struct    sockaddr_in saSrc;  
    DWORD     dwTimeSent;  
    DWORD     dwElapsed;  
    u_char    cTTL;  
	char   *destIP   =   NULL;   
	WSADATA   wsaData;   
	//创建一个Raw套节字   
	int ret = WSAStartup(0x202, &wsaData);
	if( ret != 0)
	{
		m_Msg = "端口错误";
		return FALSE;
	}

	rawSocket   =   socket(AF_INET,   SOCK_RAW,   IPPROTO_ICMP);   
	if   (rawSocket   ==   SOCKET_ERROR)     
	{   
		nRet   =   closesocket(rawSocket);   
		::WSACleanup();   
		m_Msg = WSAError("socket()");
		return   FALSE;   
	}   

	//获得主机信息   
	lpHost   =   gethostbyname(pstrHost);   
	if   (lpHost   ==   NULL)   
	{   
		nRet   =   closesocket(rawSocket);   
		::WSACleanup();   
		m_Msg.Format("Host not found: %s", pstrHost);		
		return   FALSE;   
	}   

	//构造目标套节字地址信息   
	memset(&saDest,   0,   sizeof(sockaddr_in));   
	saDest.sin_addr.s_addr   =   *((u_long   FAR   *)   (lpHost->h_addr));   
	saDest.sin_family   =   AF_INET;   
	saDest.sin_port   =   0;   

	//发送ICMP回应请求   
	SendEchoRequest(rawSocket,&saDest);   

	nRet   =   WaitForEchoReply(rawSocket);   
	if   (nRet   ==   SOCKET_ERROR){   
		nRet   =   closesocket(rawSocket);   
		::WSACleanup();   
		m_Msg = WSAError("select()");
		return   FALSE;   
	}   

	if(!nRet){   
		nRet   =   closesocket(rawSocket);   
		::WSACleanup();   
		m_Msg.Format("Request Timed Out");	
		return   FALSE;   
	}   
	else{   
		//获得回应   
		dwTimeSent   =   RecvEchoReply(rawSocket,   &saSrc,   &cTTL);   
		//计算时间   
		dwElapsed   =   GetTickCount()   -   dwTimeSent;   
		//::Sleep(1000);   
		destIP   =   inet_ntoa(saSrc.sin_addr);//获得客户IP地址   
		if(pstrHost   ==   destIP){   
			nRet   =   closesocket(rawSocket);   
			nRet   =   ::WSACleanup();  
			m_Msg.Format("OK!!!");	
			return   TRUE;   
		}   
		else{   
		    for(nLoop   =   0;   nLoop   <   nNumIp;   nLoop++){   
			dwTimeSent  =  RecvEchoReply(rawSocket,   &saSrc,   &cTTL);   
			destIP   =   inet_ntoa(saSrc.sin_addr);   
			if(pstrHost   ==   destIP){   
				nRet   =   closesocket(rawSocket);   
				nRet   =   ::WSACleanup();   
				m_Msg.Format("OK !!!");	
				return   TRUE;   
			}   
		    }   
		    nRet   =   closesocket(rawSocket);   
		    nRet   =   ::WSACleanup(); 
		    m_Msg.Format("ERROR !!!");	
		    return   FALSE;   
		}   
	}   
}   

//发送ICMPECHO数据包请求   
int   CPing::SendEchoRequest(SOCKET   s,LPSOCKADDR_IN   lpstToAddr)     
{   
	static   ECHOREQUEST   echoReq;   
	static  int nSeq   =   1;   
	int   nRet;   

	//构造回应请求   
	echoReq.icmpHdr.Type =   ICMP_ECHOREQ;   
	echoReq.icmpHdr.Code =   0;   
	echoReq.icmpHdr.Checksum =   0;   
	echoReq.icmpHdr.ID =   (USHORT)GetCurrentThreadId();   
	echoReq.icmpHdr.Seq =   nSeq++;   

	for   (nRet   =   0;   nRet   <   REQ_DATASIZE;   nRet++)   
		echoReq.cData[nRet]   =   '   '+nRet;   

	//保存发送时间   
	echoReq.dwTime   =   GetTickCount();   
	this->m_dwSendTime   =   echoReq.dwTime;   
	echoReq.icmpHdr.Checksum  =  in_cksum((u_short   *)&echoReq, 
                                               sizeof(ECHOREQUEST));   

	//发送请求   
	nRet   =   sendto(s,   
		(LPSTR)&echoReq,   
		sizeof(ECHOREQUEST),   
		0,   
		(LPSOCKADDR)lpstToAddr,     
		sizeof(SOCKADDR_IN));         

	if   (nRet   ==   SOCKET_ERROR){   
		::closesocket(s);   
		::WSACleanup();   
	}   
	return   (nRet);   
}   

//接收ICMPECHO数据包回应   
DWORD   CPing::RecvEchoReply(SOCKET   s,   LPSOCKADDR_IN   lpsaFrom,   u_char   *pTTL)     
{   
	ECHOREPLY   echoReply;   
	int   nRet;   
	int   nAddrLen   =   sizeof(struct   sockaddr_in);   

	//接收请求回应   
	nRet   =   recvfrom(s,   
		(LPSTR)&echoReply,   
		sizeof(ECHOREPLY),   
		0,   
		(LPSOCKADDR)lpsaFrom,   
		&nAddrLen);   

	//检查返回值   
	if   (nRet   ==   SOCKET_ERROR){   
		::closesocket(s);   
		::WSACleanup();   
		return   FALSE;   
	}   
	//返回发送的时间   
	*pTTL   =   echoReply.ipHdr.TTL;   

	return(echoReply.echoRequest.dwTime);         
}   

//等待回应   
int  CPing::WaitForEchoReply(SOCKET   s)   
{   
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 0;   // 秒
	Timeout.tv_usec = 10;  // 毫秒

	return(select(1, &readfds, NULL, NULL, &Timeout));
}   


//转换地址   
u_short   CPing::in_cksum(u_short   *addr,   int   len)   
{   
	register   int   nleft   =   len;   
	register   u_short   *w   =   addr;   
	register   u_short   answer;   
	register   int   sum   =   0;   

	while(   nleft   >   1   )     {   
		sum   +=   *w++;   
		nleft   -=   2;   
	}   

	if(   nleft   ==   1   )   {   
		u_short u   =   0;   

		*(u_char   *)(&u)   =   *(u_char   *)w   ;   
		sum   +=   u;   
	}   

	sum   =   (sum   >>   16)   +   (sum   &   0xffff);   
	sum   +=   (sum   >>   16);   
	answer   =   ~sum;   
	return   (answer);   
}   

//错误处理
CString CPing::WSAError(LPCSTR lpMsg)
{
	CString strMsg;
	strMsg.Format("%s - WSAError: %ld",lpMsg,WSAGetLastError());	
	//返回报错信息
	return strMsg;
}
