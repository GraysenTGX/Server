#pragma once
#include <WinSock2.h>

#define BUFFER_SIZE 4*1024
namespace tmtgx {
	enum op
	{
		OP_NULL	 =  -1,
		OP_READ	 =	1,
		OP_WRITE =	2,
		OP_ACCEPT =	3
	};

	struct _PER_IO_DATA
	{
		OVERLAPPED		m_poverlap;					//this variable must put at the first place of _PER_IO_DATA
		SOCKET			m_socket;					//current option's socket
		WSABUF			m_wsabuf;					//wsa buffer
		char			m_buffer[BUFFER_SIZE];		//data buffer
		DWORD			m_recvlen;					//length of recv data
		int				m_op_type;					//option

		_PER_IO_DATA()
		{
			m_op_type      = OP_NULL;
			m_wsabuf.buf   = m_buffer;
			m_wsabuf.len   = BUFFER_SIZE;
			m_recvlen	   = 0UL;
			m_socket	   = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED /*this is necessary for IOCP*/);
			ZeroMemory(m_buffer, BUFFER_SIZE);
			ZeroMemory(&m_poverlap, sizeof(OVERLAPPED));
		}

		~_PER_IO_DATA()
		{
			if(m_socket != INVALID_SOCKET) {
				closesocket(m_socket);
				m_socket = INVALID_SOCKET;
			}
		}

		bool operator==(const _PER_IO_DATA& data) {
			return ( data.m_socket == this->m_socket && 
				   data.m_op_type == this->m_op_type);		}

		_PER_IO_DATA(const _PER_IO_DATA& data) {
			 this->m_socket = data.m_socket;
			 this->m_poverlap = data.m_poverlap; 
			 this->m_wsabuf = data.m_wsabuf;
			 this->m_op_type = data.m_op_type;
			 memcpy(this->m_buffer, data.m_buffer, BUFFER_SIZE);
		}

		_PER_IO_DATA & operator=(const _PER_IO_DATA& data) {
			if (this == &data) {
				return *this;
			}
			this->m_socket = data.m_socket;
			this->m_poverlap = data.m_poverlap; 
			this->m_wsabuf = data.m_wsabuf;
			this->m_op_type = data.m_op_type;
			memcpy(this->m_buffer, data.m_buffer, BUFFER_SIZE);
			return *this;
		}

		void Reset() {
			ZeroMemory(m_buffer, BUFFER_SIZE);
			m_recvlen = 0UL;
		}
	};
}