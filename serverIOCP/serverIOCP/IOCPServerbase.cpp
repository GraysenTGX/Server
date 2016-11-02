#include "stdafx.h"
#include "IOCPServerbase.h"
#include <Windows.h>
#include <boost/lexical_cast.hpp>

namespace tmtgx{

	CIOCPServerbase::CIOCPServerbase(void)
	{
		m_is_inited = false;
		m_IOCPort = NULL;
		m_AcceptEXAddress = NULL;
	}


	CIOCPServerbase::~CIOCPServerbase(void)
	{
	}


	BOOL CIOCPServerbase::SocketEnvInitialize()
	{
		WSADATA wsadata;
		if(!WSAStartup(MAKEWORD(2,2),&wsadata))
			return true;
		else
			return false;
	}

	BOOL CIOCPServerbase::ListenSocketInit(std::string ip, unsigned short port)
	{
		//use WSASocket create socket for use overlap struct
		m_ListenSocket = ::WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,NULL,WSA_FLAG_OVERLAPPED); //AF_INET:IPV4
		if(m_ListenSocket == INVALID_SOCKET)
			return false;
		//init addr
		sockaddr_in sock_addr;
		memset(&sock_addr,0,sizeof(sockaddr_in));
		sock_addr.sin_port = ::htons(port);
		sock_addr.sin_addr.S_un.S_addr = ::inet_addr(ip.c_str());

		//bind local port
		if(!::bind(m_ListenSocket,(const sockaddr*)&sock_addr,sizeof(sockaddr_in)))
			return false;
		return true;
	}

	BOOL CIOCPServerbase::CreateNewCompletionPort(DWORD CPUNum)
	{
		m_IOCPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,CPUNum); //CPUnum means IOCP can run how much threads one time
		if(!m_IOCPort)
			return false;
		return true;
	}

	BOOL CIOCPServerbase::GetAcceptEXFuncAddress()
	{
		GUID guid_acceptex = WSAID_ACCEPTEX;
		DWORD dwbytes;
		::WSAIoctl(m_ListenSocket,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid_acceptex,
				sizeof(guid_acceptex),
				m_AcceptEXAddress,
				sizeof(m_AcceptEXAddress),
				&dwbytes,
				NULL,
				NULL);
		if(!m_AcceptEXAddress)
			return false;
		return true;
	}
	
	BOOL CIOCPServerbase::Associate2CompletionHandle(HANDLE &completionPort, 
														HANDLE &newDevice, 
														DWORD &hCmpletionKey)
	{
		return true;
	}


	bool CIOCPServerbase::Initialize()
	{
		//init socket work environment
		if(SocketEnvInitialize())
		{
			//add log for init sucess
			m_is_inited = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	void CIOCPServerbase::RecvDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data,_PER_IO_DATA &per_io_data)
	{

	}

	void CIOCPServerbase::SendDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data,_PER_IO_DATA &per_io_data)
	{
	}

	void CIOCPServerbase::UnInitilize()
	{
		WSACleanup();
	}

}
