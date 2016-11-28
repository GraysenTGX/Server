#pragma once
#include <WinSock2.h>


namespace tmtgx {
		
	struct _PER_SOCKET_HANDLE_DATA		//bind to IOCP PORT with socket handle together
	{
		SOCKET			m_cursocket;
		sockaddr_in		m_localaddr;
		sockaddr_in		m_remoteaddr;
		int				m_postSendEventNum;
		int				m_postRecvEventNum;
		BOOL			m_isclosed;

		_PER_SOCKET_HANDLE_DATA() {
			m_isclosed = false;
			m_postSendEventNum = 0;
			m_postRecvEventNum = 0;
		}

		~_PER_SOCKET_HANDLE_DATA() {
			if (!m_isclosed) {
				closesocket(m_cursocket);
				m_isclosed = true;
				m_cursocket = INVALID_SOCKET;
			}
		}
		bool operator==(const _PER_SOCKET_HANDLE_DATA& data) {
			return ( data.m_cursocket == this->m_cursocket && 
				data.m_localaddr.sin_addr.S_un.S_addr == this->m_localaddr.sin_addr.S_un.S_addr 
				&& data.m_remoteaddr.sin_addr.S_un.S_addr == this->m_remoteaddr.sin_addr.S_un.S_addr &&
				data.m_postRecvEventNum == this->m_postRecvEventNum && data.m_postSendEventNum == this->m_postSendEventNum);
		}

		_PER_SOCKET_HANDLE_DATA(const _PER_SOCKET_HANDLE_DATA& data) {
			 this->m_cursocket = data.m_cursocket;
			 this->m_localaddr.sin_addr.S_un.S_addr = data.m_localaddr.sin_addr.S_un.S_addr; 
			 this->m_remoteaddr.sin_addr.S_un.S_addr = data.m_remoteaddr.sin_addr.S_un.S_addr;
			 this->m_postRecvEventNum = data.m_postRecvEventNum;
			 this->m_postSendEventNum = data.m_postSendEventNum;
		}
		_PER_SOCKET_HANDLE_DATA & operator=(const _PER_SOCKET_HANDLE_DATA& data) {
			if (this == &data) {
				return *this;
			}
			this->m_cursocket = data.m_cursocket;
			this->m_localaddr.sin_addr.S_un.S_addr = data.m_localaddr.sin_addr.S_un.S_addr; 
			this->m_remoteaddr.sin_addr.S_un.S_addr = data.m_remoteaddr.sin_addr.S_un.S_addr;
			this->m_postRecvEventNum = data.m_postRecvEventNum;
			this->m_postSendEventNum = data.m_postSendEventNum;
			return *this;
		}
	};
}