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
	//����һ��Raw�׽���   
	int ret = WSAStartup(0x202, &wsaData);
	if( ret != 0)
	{
		m_Msg = "�˿ڴ���";
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

	//���������Ϣ   
	lpHost   =   gethostbyname(pstrHost);   
	if   (lpHost   ==   NULL)   
	{   
		nRet   =   closesocket(rawSocket);   
		::WSACleanup();   
		m_Msg.Format("Host not found: %s", pstrHost);		
		return   FALSE;   
	}   

	//����Ŀ���׽��ֵ�ַ��Ϣ   
	memset(&saDest,   0,   sizeof(sockaddr_in));   
	saDest.sin_addr.s_addr   =   *((u_long   FAR   *)   (lpHost->h_addr));   
	saDest.sin_family   =   AF_INET;   
	saDest.sin_port   =   0;   

	//����ICMP��Ӧ����   
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
		//��û�Ӧ   
		dwTimeSent   =   RecvEchoReply(rawSocket,   &saSrc,   &cTTL);   
		//����ʱ��   
		dwElapsed   =   GetTickCount()   -   dwTimeSent;   
		//::Sleep(1000);   
		destIP   =   inet_ntoa(saSrc.sin_addr);//��ÿͻ�IP��ַ   
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

//����ICMPECHO���ݰ�����   
int   CPing::SendEchoRequest(SOCKET   s,LPSOCKADDR_IN   lpstToAddr)     
{   
	static   ECHOREQUEST   echoReq;   
	static  int nSeq   =   1;   
	int   nRet;   

	//�����Ӧ����   
	echoReq.icmpHdr.Type =   ICMP_ECHOREQ;   
	echoReq.icmpHdr.Code =   0;   
	echoReq.icmpHdr.Checksum =   0;   
	echoReq.icmpHdr.ID =   (USHORT)GetCurrentThreadId();   
	echoReq.icmpHdr.Seq =   nSeq++;   

	for   (nRet   =   0;   nRet   <   REQ_DATASIZE;   nRet++)   
		echoReq.cData[nRet]   =   '   '+nRet;   

	//���淢��ʱ��   
	echoReq.dwTime   =   GetTickCount();   
	this->m_dwSendTime   =   echoReq.dwTime;   
	echoReq.icmpHdr.Checksum  =  in_cksum((u_short   *)&echoReq, 
                                               sizeof(ECHOREQUEST));   

	//��������   
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

//����ICMPECHO���ݰ���Ӧ   
DWORD   CPing::RecvEchoReply(SOCKET   s,   LPSOCKADDR_IN   lpsaFrom,   u_char   *pTTL)     
{   
	ECHOREPLY   echoReply;   
	int   nRet;   
	int   nAddrLen   =   sizeof(struct   sockaddr_in);   

	//���������Ӧ   
	nRet   =   recvfrom(s,   
		(LPSTR)&echoReply,   
		sizeof(ECHOREPLY),   
		0,   
		(LPSOCKADDR)lpsaFrom,   
		&nAddrLen);   

	//��鷵��ֵ   
	if   (nRet   ==   SOCKET_ERROR){   
		::closesocket(s);   
		::WSACleanup();   
		return   FALSE;   
	}   
	//���ط��͵�ʱ��   
	*pTTL   =   echoReply.ipHdr.TTL;   

	return(echoReply.echoRequest.dwTime);         
}   

//�ȴ���Ӧ   
int  CPing::WaitForEchoReply(SOCKET   s)   
{   
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 0;   // ��
	Timeout.tv_usec = 10;  // ����

	return(select(1, &readfds, NULL, NULL, &Timeout));
}   


//ת����ַ   
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

//������
CString CPing::WSAError(LPCSTR lpMsg)
{
	CString strMsg;
	strMsg.Format("%s - WSAError: %ld",lpMsg,WSAGetLastError());	
	//���ر�����Ϣ
	return strMsg;
}
