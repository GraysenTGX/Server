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
	
	BOOL CIOCPServerbase::Associate2CompletionHandle(HANDLE completionPort
													,boost::shared_ptr<_PER_SOCKET_HANDLE_DATA> per_socket_data)
	{
		if (NULL == per_socket_data) {
			return false;
		}
		return NULL == CreateIoCompletionPort((HANDLE)per_socket_data->m_cursocket, completionPort, (ULONG_PTR)(*per_socket_data) , 0) ? false : true;
	}


	bool CIOCPServerbase::Initialize()
	{
		if(m_is_inited) {
			return m_is_inited;
		}
		//init socket work environment
		m_is_inited = SocketEnvInitialize();
		return m_is_inited;
	}

	void CIOCPServerbase::RecvDataProc(boost::shared_ptr<_PER_SOCKET_HANDLE_DATA> per_handle_data,boost::shared_ptr<_PER_IO_DATA> per_io_data)
	{

	}

	void CIOCPServerbase::SendDataProc(boost::shared_ptr<_PER_SOCKET_HANDLE_DATA> per_handle_data,boost::shared_ptr<_PER_IO_DATA> per_io_data)
	{
	}

	void CIOCPServerbase::UnInitialize()
	{
		WSACleanup();
	}
		
	BOOL CIOCPServerbase::PostAcceptEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA> per_io_data)
	{
		if (NULL == per_io_data || NULL == m_AcceptEXAddress) {
			return false;
		} else {
			return FALSE == m_AcceptEXAddress(m_ListenSocket, per_io_data->m_socket, per_io_data->m_wsabuf.buf, 
				per_io_data->m_wsabuf.len - (sizeof(SOCKADDR_IN)+16)*2, sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16,
				&per_io_data->m_recvlen, &per_io_data->m_poverlap) ? true : false;
		}
	}

	BOOL CIOCPServerbase::PostRecvEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA> per_io_data)
	{
		if (NULL == per_io_data) {
			return false;
		} else {
			if (INVALID_SOCKET != per_io_data->m_socket) {
				DWORD flags = 0UL;
				::WSARecv(per_io_data->m_socket, &per_io_data->m_wsabuf, 1,	&per_io_data->m_recvlen, &flags,
					&per_io_data->m_poverlap, NULL);
				return true;
			} else {
				return false;
			}
		}
	}

	BOOL CIOCPServerbase::PostSendEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA> per_io_data)
	{
		if (NULL == per_io_data) {
			return false;
		} else {
			// we do not need the send event complete inform ,so 
		}
	}
}
